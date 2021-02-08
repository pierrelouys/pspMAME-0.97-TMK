#include "driver.h"
#include <dos.h>
#include <conio.h>

#include "syscall.h"

#define MAXVOLUME 0x8000

static int pga_handle[1];
static int pga_volume =MAXVOLUME;

/* audio related stuff */
#define SOUND_CHANNELS 2	/* left and right */
HAC hVoice[SOUND_CHANNELS];
LPAUDIOWAVE lpWave[SOUND_CHANNELS];
AUDIOINFO info;
AUDIOCAPS caps;

int nominal_sample_rate;
int soundcard,usestereo;
int attenuation = 0;
int sampleratedetect = 1;
static int master_volume =MAXVOLUME;

static int stream_playing;
static INT16 *stream_cache_data;
static int stream_cache_len;
static int stream_cache_stereo;

#define NUM_BUFFERS 3	/* raising this number should improve performance with frameskip, */
						/* but also increases the latency. */

static int voice_pos;
static int audio_buffer_length;

/* global sample tracking */
static double samples_per_frame;
static double samples_left_over;
static UINT32 samples_this_frame;

int msdos_init_sound(void)
{
	if( Machine->sample_rate == 0 )
	{
		return 0;
	}

	/* Initialize the audio library */
	if (AInitialize() != AUDIO_ERROR_NONE)
	{
		printf ("Unable to initialize SEAL\n");
		return (1);
	}

	nominal_sample_rate = Machine->sample_rate;

	stream_playing = 0;
	stream_cache_data = 0;
	stream_cache_len = 0;
	stream_cache_stereo = 0;

	osd_set_mastervolume(attenuation);	/* set the startup volume */

	return 0;
}


void msdos_shutdown_sound(void)
{
	if( Machine->sample_rate == 0 )
	{
		return;
	}

	if( soundcard == 0 )
	{
		return;
	}

	ACloseVoices();
	ACloseAudio();
}


int osd_start_audio_stream(int stereo)
{
	if( stereo != 0 )
	{
		/* make sure it's either 0 or 1 */
		stereo = 1;
	}

	stream_cache_stereo = stereo;

	/* determine the number of samples per frame */
	samples_per_frame = (double)Machine->sample_rate / (double)Machine->refresh_rate;

	/* compute how many samples to generate this frame */
	samples_left_over = samples_per_frame;
	samples_this_frame = (UINT32)samples_left_over;
	samples_left_over -= (double)samples_this_frame;

	audio_buffer_length = NUM_BUFFERS * samples_per_frame + 20;

	if( Machine->sample_rate == 0 )
	{
		return 0;
	}

	{
		int channel;

		/* initialize SEAL audio library */
		if( soundcard == 0 )
		{
			return samples_this_frame;
		}

		/* open audio device */
/*		info.nDeviceId = AUDIO_DEVICE_MAPPER;*/
		info.nDeviceId = soundcard;
		/* always use 16 bit mixing if possible - better quality and same speed of 8 bit */
		info.wFormat = AUDIO_FORMAT_16BITS | AUDIO_FORMAT_MONO | AUDIO_FORMAT_RAW_SAMPLE;

		/* use stereo output if supported */
		if (usestereo && stereo)
		{
			info.wFormat = AUDIO_FORMAT_16BITS | AUDIO_FORMAT_STEREO | AUDIO_FORMAT_RAW_SAMPLE;
		}

		info.nSampleRate = Machine->sample_rate;
		if (AOpenAudio(&info) != AUDIO_ERROR_NONE)
		{
			logerror("audio initialization failed\n");
			return 0;
		}

		AGetAudioDevCaps(info.nDeviceId,&caps);
		logerror("Using %s at %d-bit %s %u Hz\n",
				caps.szProductName,
				info.wFormat & AUDIO_FORMAT_16BITS ? 16 : 8,
				info.wFormat & AUDIO_FORMAT_STEREO ? "stereo" : "mono",
				info.nSampleRate);

		/* open and allocate voices, allocate waveforms */
		if (AOpenVoices(SOUND_CHANNELS) != AUDIO_ERROR_NONE)
		{
			logerror("voices initialization failed\n");
			return 0;
		}

		for (channel = 0; channel < SOUND_CHANNELS; channel++)
		{
			lpWave[channel] = 0;
		}

		/* update the Machine structure to reflect the actual sample rate */
		Machine->sample_rate = info.nSampleRate;

		logerror("set sample rate: %d\n",Machine->sample_rate);
		if (sampleratedetect)
		{
			cycles_t a,b;
			cycles_t cps = osd_cycles_per_second();
			LONG start,end;

			if (ACreateAudioVoice(&hVoice[0]) != AUDIO_ERROR_NONE)
			{
				logerror( "ACreateAudioVoice() failed\n" );
				return 0;
			}

			if ((lpWave[0] = (LPAUDIOWAVE)malloc(sizeof(AUDIOWAVE))) == 0)
			{
				logerror( "malloc() failed\n" );
				ADestroyAudioVoice(hVoice[0]);
				return 0;
			}

			lpWave[0]->wFormat = AUDIO_FORMAT_8BITS | AUDIO_FORMAT_MONO;
			lpWave[0]->nSampleRate = Machine->sample_rate;
			lpWave[0]->dwLength = 3*Machine->sample_rate;
			lpWave[0]->dwLoopStart = 0;
			lpWave[0]->dwLoopEnd = 3*Machine->sample_rate;
			if (ACreateAudioData(lpWave[0]) != AUDIO_ERROR_NONE)
			{
				logerror( "ACreateAudioData() failed\n" );
				free(lpWave[0]);
				lpWave[0] = 0;

				return 0;
			}

			memset(lpWave[0]->lpData,0,3*Machine->sample_rate);
			/* upload the data to the audio DRAM local memory */
			AWriteAudioData(lpWave[0],0,3*Machine->sample_rate);
			APrimeVoice(hVoice[0],lpWave[0]);
			ASetVoiceFrequency(hVoice[0],Machine->sample_rate);
			ASetVoiceVolume(hVoice[0],0);
			AStartVoice(hVoice[0]);

			a = osd_cycles();
			/* wait some time to let everything stabilize */
			do
			{
				AUpdateAudioEx(Machine->sample_rate / (double)Machine->refresh_rate);
				b = osd_cycles();
			} while (b-a < cps/10);

			a = osd_cycles();
			AGetVoicePosition(hVoice[0],&start);
			do
			{
				AUpdateAudioEx(Machine->sample_rate / (double)Machine->refresh_rate);
				b = osd_cycles();
			} while (b-a < cps);
			AGetVoicePosition(hVoice[0],&end);
			Machine->sample_rate = end - start;
			logerror("actual sample rate: %d\n",Machine->sample_rate);

			AStopVoice(hVoice[0]);
			ADestroyAudioData(lpWave[0]);
			free(lpWave[0]);
			lpWave[0] = 0;
			ADestroyAudioVoice(hVoice[0]);
		}

		for (channel = 0;channel <= stereo;channel++)
		{
			if (ACreateAudioVoice(&hVoice[channel]) != AUDIO_ERROR_NONE)
			{
				logerror( "ACreateAudioVoice() failed\n" );
				return 0;
			}

			if ((lpWave[channel] = (LPAUDIOWAVE)malloc(sizeof(AUDIOWAVE))) == 0)
			{
				logerror( "malloc() failed\n" );
				ADestroyAudioVoice(hVoice[channel]);
				return 0;
			}

			lpWave[channel]->wFormat = AUDIO_FORMAT_16BITS | AUDIO_FORMAT_MONO | AUDIO_FORMAT_LOOP;
			lpWave[channel]->nSampleRate = nominal_sample_rate;
			lpWave[channel]->dwLength = 2*audio_buffer_length;
			lpWave[channel]->dwLoopStart = 0;
			lpWave[channel]->dwLoopEnd = lpWave[channel]->dwLength;
			if (ACreateAudioData(lpWave[channel]) != AUDIO_ERROR_NONE)
			{
				logerror( "ACreateAudioData() failed\n" );
				free(lpWave[channel]);
				lpWave[channel] = 0;

				return 0;
			}

			memset(lpWave[channel]->lpData,0,lpWave[channel]->dwLength);
			APrimeVoice(hVoice[channel],lpWave[channel]);
			ASetVoiceFrequency(hVoice[channel],nominal_sample_rate);
		}

		if (stereo)
		{
			/* SEAL doubles volume for panned channels, so we have to compensate */
			ASetVoiceVolume(hVoice[0],32);
			ASetVoiceVolume(hVoice[1],32);

			ASetVoicePanning(hVoice[0],0);
			ASetVoicePanning(hVoice[1],255);

			AStartVoice(hVoice[0]);
			AStartVoice(hVoice[1]);
		}
		else
		{
			ASetVoiceVolume(hVoice[0],64);
			ASetVoicePanning(hVoice[0],128);
			AStartVoice(hVoice[0]);
		}
	}

	stream_playing = 1;
	voice_pos = 0;

	osd_sound_enable( 1 );

	return samples_this_frame;
}

//============================================================
//	sound_update_refresh_rate
//============================================================

void sound_update_refresh_rate(float newrate)
{
	samples_per_frame = (double)Machine->sample_rate / (double)Machine->refresh_rate;
}



void osd_stop_audio_stream(void)
{
	if( Machine->sample_rate == 0 )
	{
		return;
	}

	osd_sound_enable( 0 );

	{
		int i;

		if( soundcard == 0 )
		{
			return;
		}

		/* stop and release voices */
		for (i = 0;i < SOUND_CHANNELS;i++)
		{
			if (lpWave[i] != 0)
			{
				AStopVoice(hVoice[i]);
				ADestroyAudioData(lpWave[i]);
				free(lpWave[i]);
				lpWave[i] = 0;
				ADestroyAudioVoice(hVoice[i]);
			}
		}
	}

	stream_playing = 0;
}

static void updateaudiostream( int throttle )
{
	INT16 *data = stream_cache_data;
	int stereo = stream_cache_stereo;
	int len = stream_cache_len;
	int buflen;
	int start,end;

	if (!stream_playing)
	{
		/* error */
		return;
	}

	buflen = audio_buffer_length;
	start = voice_pos;
	end = voice_pos + len;
	if (end > buflen)
	{
		end -= buflen;
	}

	if (throttle)   /* sync with audio only when speed throttling is not turned off */
	{
		profiler_mark(PROFILER_IDLE);
		for (;;)
		{
			LONG curpos;

			AGetVoicePosition(hVoice[0],&curpos);
			if (start < end)
			{
				if (curpos < start || curpos >= end)
				{
					break;
				}
			}
			else
			{
				if (curpos < start && curpos >= end)
				{
					break;
				}
			}
			AUpdateAudioEx(samples_per_frame);
		}
		profiler_mark(PROFILER_END);
	}

	if (stereo)
	{
		INT16 *bufL,*bufR;
		int p;

		bufL = (INT16 *)lpWave[0]->lpData;
//		bufR = (INT16 *)lpWave[1]->lpData;
		p = start;
		while (p != end)
		{
			if (p >= buflen)
			{
				p -= buflen;
			}
//			bufL[p] = *data++;
//			bufR[p] = *data++;
			bufL[p*2] = *data++;
			bufL[p*2+1] = *data++;
			p++;
		}
	}
	else
	{
		INT16 *buf;
		int p;

		buf = (INT16 *)lpWave[0]->lpData;
		p = start;
		while (p != end)
		{
			if (p >= buflen)
			{
				p -= buflen;
			}
//			buf[p] = *data++;
			buf[p*2] =buf[p*2+1] = *data++;
			p++;
		}
	}

	if (start < end)
	{
		sceAudioOutputPanned(pga_handle[0], pga_volume, pga_volume, &lpWave[0]->lpData[start *2]);
//		AWriteAudioData(lpWave[0],2*start,2*len);
//		if (stereo)
//		{
//			AWriteAudioData(lpWave[1],2*start,2*len);
//		}
	}
	else
	{
		int remain = buflen-start;
		AWriteAudioData(lpWave[0],2*start,2*remain);
		AWriteAudioData(lpWave[0],0,2*(len-remain));
		if (stereo)
		{
			AWriteAudioData(lpWave[1],2*start,2*remain);
			AWriteAudioData(lpWave[1],0,2*(len-remain));
		}
	}

	voice_pos = end;
	if (voice_pos == buflen)
	{
		voice_pos = 0;
	}
}


int osd_update_audio_stream(INT16 *buffer)
{
	stream_cache_data = buffer;
	stream_cache_len = samples_this_frame;

	/* compute how many samples to generate next frame */
	samples_left_over += samples_per_frame;
	samples_this_frame = (UINT32)samples_left_over;
	samples_left_over -= (double)samples_this_frame;

	return samples_this_frame;
}


int msdos_update_audio( int throttle )
{
	if( Machine->sample_rate == 0 || stream_cache_data == 0 )
	{
		return 0;
	}

	if( soundcard == 0 )
	{
		return 0;
	}

	profiler_mark(PROFILER_MIXER);

	AUpdateAudioEx(samples_per_frame);

	updateaudiostream( throttle );

	profiler_mark(PROFILER_END);

	return 1;
}


/* attenuation in dB */
void osd_set_mastervolume(int _attenuation)
{
	float volume;

	if (_attenuation > 0)
	{
		_attenuation = 0;
	}
	if (_attenuation < -32)
	{
		_attenuation = -32;
	}

	attenuation = _attenuation;

 	volume = MAXVOLUME;	/* range is 0-0x8000 */
	while (_attenuation++ < 0)
	{
		volume /= 1.122018454;	/* = (10 ^ (1/20)) = 1dB */
	}

	pga_volume =master_volume =volume;

//	ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
	sceAudioChangeChannelVolume(pga_handle[0], master_volume, master_volume);
}


int osd_get_mastervolume(void)
{
	return attenuation;
}


void osd_sound_enable(int enable_it)
{
	if( soundcard == 0 )
	{
		return;
	}
	if (enable_it)
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
		pga_volume =master_volume;
		sceAudioChangeChannelVolume(pga_handle[0], master_volume, master_volume);
	}
	else
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,0);
		pga_volume =0;
		sceAudioChangeChannelVolume(pga_handle[0], 0, 0);
	}
}
