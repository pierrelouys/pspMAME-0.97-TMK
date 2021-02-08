//============================================================
//
//	psp_sound.c - PSP sound functions
//
//============================================================

#include "driver.h"
//#include "psp_main.h"
//#include "pg.h"
//#include "syscall.h"


//============================================================
//	LOCAL VARIABLES
//============================================================

static volatile int sound_active;
static int sound_handle;
static SceUID/*int*/ sound_thread;
static /*unsigned char*/int sound_enable/*sound_disable*/;

//#define PGA_SAMPLES 512
#define PGA_SAMPLES 1024
//#define MAXVOLUME 0x8000

//static int sound_disable=1/*0*/;


//int pga_volume =0x8000;/* used from src\psp\sound.c */
//static unsigned char sound_disable;



#ifdef PSP_DIRECT_STREAM
static struct sound_t sound_info;

//============================================================
//	GLOBAL VARIABLES
//============================================================

struct sound_t *psp_sound = &sound_info;

static int sound_volume;
static s16 sound_buffer[2][PSP_SOUND_BUFFER_SIZE/*2048*/];
//static int psp_sound_flip = 0;

#endif //PSP_DIRECT_STREAM

//unsigned long osd_cycles(void);
//#define PGA_VOLUME (0xffff)


#if 0
//static wavout_wavinfo_t wavinfo_bg;
//static wavout_wavinfo_t *wavout_snd0_wavinfo=0;
//int wavout_snd0_ready=0;
//int wavout_snd0_playend=0;
//unsigned long wavout_snd0_playptr=0;
unsigned long cur_play =0;
unsigned long cur_nloop;

static INT16 *cache_data;
#endif



#ifdef PSP_DIRECT_STREAM

//#define sound_volume (0xffff)
//============================================================
//	psp_sound_thread
//============================================================

static int psp_sound_thread(SceSize/*int*/ args, void *argp)
{
	int flip = 0;

	v_sync();

	while (sound_active)
	{
		if (psp_sleep)
		{
			do
			{
				sceKernelDelayThread(5000000);
			} while (psp_sleep);
		}

		if (sound_enable)//if (sound_disable)
		{
			(*psp_sound->callback)((int)sound_buffer[flip]);
			sceAudioOutputPannedBlocking(sound_handle, sound_volume, sound_volume, (char *)sound_buffer[flip]);
		}
		else
		{
			memset(sound_buffer[flip], 0, PSP_SOUND_BUFFER_SIZE);
			sceAudioOutputPannedBlocking(sound_handle, 0, 0, (char *)sound_buffer[flip]);
		}
		flip ^= 1;
	}
	return 0;
}

#else

//============================================================
//	psp_sound_thread
//============================================================
#if (0==PSP_MAME_STREAMING_MODE)
/* TMK 互換 */
//extern INT16 *finalmix;
extern short int *finalmix;
extern int finalmixLen;
unsigned long cur_play/* =0*/;
int cache_len;
//extern unsigned long cur_play;
//extern int cache_len;
//extern int finalmixCur;
//extern int finalmixCurNow;
//extern int finalmixCounter;

static int psp_sound_thread(SceSize/*int*/ args, void *argp)
{
static short int *lptr =0x00;

	//int channel=*(int *)argp;
	v_sync();

	while (sound_active)
	{
		if (psp_sleep)
		{
			do
			{
				sceKernelDelayThread(5000000);
			} while (psp_sleep);
		}
#if 1
		if (PGA_SAMPLES >cache_len)
		{
//			if (!sound_disable && !sceAudioGetChannelRestLen(sound_handle))
//				sceAudioOutputPanned(sound_handle, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, lptr);
			sceKernelDelayThread(1000000/60/10);
			continue;
		}
		cache_len -=PGA_SAMPLES;
#endif
		unsigned int ptr =cur_play;
		unsigned int nextptr =ptr +PGA_SAMPLES *2;
		if (nextptr >=finalmixLen) nextptr=0;
		lptr =&finalmix[ptr];

//		if (sound_disable && (cache_len*2) <finalmixLen)
		if(sound_enable)
		{
			if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, lptr);}
		}
		else
		{
			if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, 0, 0, lptr);}
		}
	//	psp_sound_flip ^= 1;
		cur_play =nextptr;
	}
	return 0;
}
#endif //(0==PSP_MAME_STREAMING_MODE)


#if (1==PSP_MAME_STREAMING_MODE)

static int psp_sound_thread(SceSize/*int*/ args, void *argp)
{
//static short int *lptr =0x00;

	//int channel=*(int *)argp;
	v_sync();

	while (sound_active)
	{
		if (psp_sleep)
		{
			do
			{
				sceKernelDelayThread(5000000);
			} while (psp_sleep);
		}
#if 0
		if (PGA_SAMPLES >cache_len)
		{
//			if (!sound_disable && !sceAudioGetChannelRestLen(sound_handle))
//				sceAudioOutputPanned(sound_handle, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, lptr);
			sceKernelDelayThread(1000000/60/10);
			continue;
		}
		cache_len -=PGA_SAMPLES;
#endif
	//	unsigned int ptr =cur_play;
	//	unsigned int nextptr =ptr +PGA_SAMPLES *2;
	//	if (nextptr >=finalmixLen) nextptr=0;
	//	lptr =&finalmix[ptr];

//		if (sound_disable && (cache_len*2) <finalmixLen)
		if(sound_enable)
		{
		//	if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, PSP_AUDIO_VOLUME_MAX/*PGA_VOLUME*/, (char *)psp_sound_buffer[psp_sound_flip]);}
		}
		else
		{
		//	if (cache_len <PGA_SAMPLES*2)
			{		sceAudioOutputPannedBlocking(sound_handle, 0, 0, (char *)psp_sound_buffer[psp_sound_flip]);}
		}
		psp_sound_flip ^= 1;
	//	cur_play =nextptr;
	}
	return 0;
}
#endif //(1==PSP_MAME_STREAMING_MODE)


#endif //(1==PSP_DIRECT_STREAM)

#ifdef PSP_DIRECT_STREAM
//============================================================
//	psp_sound_set_volume
//============================================================

void psp_sound_set_volume(void)
{
	if (sound_active)
		sound_volume = PSP_AUDIO_VOLUME_MAX;
}
#endif //PSP_DIRECT_STREAM
/*--------------------------------------------------------
	サウンド有効/無効切り替え
--------------------------------------------------------*/

void psp_sound_enable(unsigned char set_to_enable)
{
	if (sound_active)
	{
		sound_enable = set_to_enable;//sound_disable = !enable;
#ifdef PSP_DIRECT_STREAM
		if (sound_enable) //if (!sound_disable)
			psp_sound_set_volume();
		else
			sound_volume = 0;
#endif //PSP_DIRECT_STREAM
	}
}

void osd_sound_enable(int enable_it)
{
 if(enable_it)
   psp_sound_enable(1);
else psp_sound_enable(0);
}

//============================================================
//	psp_sound_init
//============================================================

//int psp_init_sound(void)
void psp_sound_init(void)
{
#ifdef PSP_DIRECT_STREAM
	sound_active  = 0;
	sound_thread  = -1;
	sound_handle  = -1;
	sound_volume  = 0;
	sound_enable  = 0;//sound_disable = 1;
#endif //PSP_DIRECT_STREAM

//	stream_playing = 0;
//	stream_cache_data = 0;
//	stream_cache_len = 0;
//	stream_cache_stereo = 0;

	/* initialize SEAL audio library */
//	if( soundcard == 0 || machine->sample_rate == 0 )
//	{
//		return 0;
//	}

//	psp_sound_start();//pgaInit();
//	pga_handle[0] =-1;

//	nominal_sample_rate = machine->sample_rate;
	//osd_set_mastervolume(attenuation);	/* set the startup volume */
//	return 0;
}


//============================================================
//	psp_sound_start
//============================================================

int psp_sound_start(void)//int pgaInit(void)
{

	logWriteX("psp_sound_start: stage 0","","",666);

	sound_active  = 0;
	sound_thread  = -1;
	sound_handle  = -1;
	sound_enable  = 0;//sound_disable = 1;
#ifdef PSP_DIRECT_STREAM

	logWriteX("psp_sound_start: stage 0.1","","",666);


	memset((void *)sound_buffer[0], 0, PSP_SOUND_BUFFER_SIZE);
	memset((void *)sound_buffer[1], 0, PSP_SOUND_BUFFER_SIZE);

	if (psp_sound->stereo)
		sound_handle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, PSP_SOUND_SAMPLES, PSP_AUDIO_FORMAT_STEREO);
	else
		sound_handle = sceAudioChReserve(PSP_AUDIO_NEXT_CHANNEL, PSP_SOUND_SAMPLES, PSP_AUDIO_FORMAT_MONO);
	if (sound_handle < 0)
	{
		return 0;
	}

	sound_thread = sceKernelCreateThread("Sound thread", psp_sound_thread, 0x08, psp_sound->stack, 0, NULL);
	if (sound_thread < 0)
	{
		sceAudioChRelease(sound_handle);
		sound_handle = -1;
		return 0;
	}

	sound_active = 1;
	sceKernelStartThread(sound_thread, 0, 0);

	psp_sound_set_volume();

	return 1;
#else
	logWriteX("psp_sound_start: stage 1","","",666);

	sound_handle = sceAudioChReserve(/*-1*/PSP_AUDIO_NEXT_CHANNEL,PGA_SAMPLES,/*0*/PSP_AUDIO_FORMAT_STEREO);	// par3 - 0x10:mono
	if (sound_handle < 0)
	{
	//	sound_handle=-1;
		return 0/*-1*/;
	}

    logWriteX("psp_sound_start: stage 2","","",666);

	sound_thread = sceKernelCreateThread("Sound thread", psp_sound_thread, /*0x12*/0x08, 0x10000, 0, NULL);

	logWriteX("psp_sound_start: stage 3","","",666);

	if (sound_thread < 0)
	{
	//	sound_thread=-1;
		sceAudioChRelease(sound_handle);
		sound_handle=-1;
		return 0/*-1*/;
	}
//	sound_enable=1;//	sound_disable=0/*1*/;

	logWriteX("psp_sound_start: stage 4","","",666);

	sound_active=1;
	sceKernelStartThread(sound_thread, 0, 0);

	logWriteX("psp_sound_start: stage 5","","",666);

	return 1/*0*/;
#endif
}



//============================================================
//	psp_sound_stop
//============================================================

void psp_sound_stop(void)
{
	if (sound_thread >= 0)
	{
#ifdef PSP_DIRECT_STREAM
		sound_volume = 0;
#endif //PSP_DIRECT_STREAM
		sound_enable = 0;//sound_disable = 1;

		sound_active = 0;
		sceKernelWaitThreadEnd(sound_thread, NULL);

		sceKernelDeleteThread(sound_thread);
		sound_thread = -1;

		sceAudioChRelease(sound_handle);
		sound_handle = -1;
	}
}


//============================================================
//	psp_sound_exit
//============================================================

void psp_sound_exit(void)
{
//	if( soundcard == 0 || machine->sample_rate == 0 ){		return;	}
	//wavout_snd0_wavinfo =0x00;
	psp_sound_stop();
}


#if 0
//============================================================
//	psp_sound_set_priority
//============================================================

void psp_sound_set_priority(int priority)
{
	sceKernelChangeThreadPriority(sound_thread, priority);
}
#endif


//バッファは64バイト境界じゃなくても大丈夫みたい
//[0]が左、[1]が右
//サンプル速度は44100
//vol1が左


//unsigned char sound_buf[512 *2 *2 *2]; // *short *streo *buff
//#define PGA_SAMPLES 1024
//extern int pga_handle[];
//extern int pga_volume;

//int nominal_sample_rate;
//int soundcard;
//int usestereo;
//int attenuation = 0;
//int sampleratedetect = 1;
//static int master_volume = 256;

//static int stream_playing;
//static INT16 *stream_cache_data;
//static int stream_cache_len;
//static int stream_cache_stereo;

//#define NUM_BUFFERS 3	/* raising this number should improve performance with frameskip, */
						/* but also increases the latency. */

//static int voice_pos;
//static int audio_buffer_length;

/* global sample tracking */
static float samples_per_frame;
static float samples_left_over;
static UINT32 samples_this_frame;



//typedef struct {
//	unsigned long channels;
//	unsigned long samplerate;
//	unsigned long samplecount;
//	unsigned long datalength;
//	char *wavdata;
//	unsigned long rateratio;		// samplerate / 44100 * 0x10000
//	unsigned long playptr;
//	unsigned long playptr_frac;
//	int playloop;
//} wavout_wavinfo_t;


//static inline void memcpy4x( unsigned long* dest, unsigned long* src, short ct4x )
//{
//	for( ; ct4x>0 ; ct4x-- ){ *dest++ = *src++ ; } // dest++; src++ ; }
//}


extern SETTING setting;
#if 0
extern void psp_sound_enable(unsigned char enable);
/*static*/ void psp_sound_enable(int enable_it)/*mame.c*/
{
	if(
		///*soundcard*/setting.sound_ON_OFF == 0
		(SND_OFF==setting.sound_rate)
		|| Machine->sample_rate == 0 )
		return;

	if (enable_it)
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
		//wavout_snd0_wavinfo=&wavinfo_bg;

		//pga_volume =0x8000;
		psp_sound_enable(1);
//		if (-1 !=pga_handle[0])
//			sceAudioChangeChannelVolume(pga_handle[0], 0x8000, 0x8000);
	}
	else
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,0);
		//wavout_snd0_wavinfo =0x00;

		//pga_volume =0x0000;
		psp_sound_enable(0);
//		if (-1 !=pga_handle[0])
//			sceAudioChangeChannelVolume(pga_handle[0], 0x0000, 0x0000);
	}
}
#endif
//============================================================
//	sound_update_refresh_rate
//============================================================
#if 0
void sound_update_refresh_rate(float newrate)
{
	samples_per_frame = (float)Machine->sample_rate / (float)Machine->refresh_rate;

//	if (-1 !=pga_handle[0]) {
//		int sceAudioSample =(int)samples_per_frame;
//		sceAudioSample =sceAudioSample /2 *2;
//		sceAudioSetChannelDataLen(pga_handle[0], sceAudioSample);
//	}
}
#endif
int osd_start_audio_stream(int stereo)
{
	if( stereo != 0 )
	{
		/* make sure it's either 0 or 1 */
		stereo = 1;
	}

//	stream_cache_stereo = stereo;

	/* determine the number of samples per frame */
	if(0==Machine->refresh_rate)
	{
		samples_per_frame = 0;
	}
	else
	{
		samples_per_frame = (float)Machine->sample_rate / (float)Machine->refresh_rate;
	}

	/* compute how many samples to generate this frame */
	samples_left_over = samples_per_frame;
	samples_this_frame = (UINT32)samples_left_over;
	samples_left_over -= (float)samples_this_frame;

//	audio_buffer_length = NUM_BUFFERS * samples_per_frame + 20;

	if( Machine->sample_rate == 0 )
	{
		return 0;
	}

	//int sceAudioSample =(int)samples_per_frame;
//	sceAudioSample =sceAudioSample /2 *2;
//	sceAudioSample =PGA_SAMPLES;
//	if (0 >(pga_handle[0] =sceAudioChReserve(-1, sceAudioSample, 0))) {
//		char buf[100];
//		sprintf(buf, "%d =sceAudioChReserve(-1, %d, 0)", pga_handle[0], sceAudioSample);
//		psp_printf(buf);
//		return 0;
//	}
//		sceAudioChRelease(pga_handle[0]);

//	if (0x00 ==(sound_buf =malloc(audio_buffer_length *2 *2))) // 16bit,streo
//		return 0;
//	memset(sound_buf, 0 ,audio_buffer_length *2 *2);
//	memset(sound_buf, 0 ,sizeof(sound_buf));
#if 000
	wavinfo_bg.channels =stream_cache_stereo+1;
	wavinfo_bg.samplerate =Machine->sample_rate;
	wavinfo_bg.samplecount=samples_this_frame;
	wavinfo_bg.datalength=sizeof(audio_buffer_length*2 *2);
//	wavinfo_bg.wavdata=sound_buf;
	wavinfo_bg.rateratio=((wavinfo_bg.samplerate *0x4000)/11025);
	wavinfo_bg.playptr=0;
	wavinfo_bg.playptr_frac=0;
	wavinfo_bg.playloop=1;
#endif
	//wavout_snd0_wavinfo=&wavinfo_bg;

//	char buftmk[20];
//	sprintf(buftmk, "samples_per_frame[%i]", (int)samples_per_frame);
//	psp_printf(buftmk);
#if 0
	cache_data =0x00;
#endif

#ifdef PSP_DIRECT_STREAM


#else

#if (0==PSP_MAME_STREAMING_MODE)
/* TMK 互換 */
	cur_play =0;
	cache_len =0;
#endif //(0==PSP_MAME_STREAMING_MODE)
#endif //(1==PSP_DIRECT_STREAM)

//	stream_playing = 1;
//	voice_pos = 0;

	psp_sound_enable(1);

	return samples_this_frame;
}




void osd_stop_audio_stream(void)
{
	if(	// /*soundcard*/setting.sound_ON_OFF == 0
		(SND_OFF==setting.sound_rate)
		|| Machine->sample_rate == 0 )
	{	return;	}

	psp_sound_enable(0);
	//wavout_snd0_wavinfo =0x00;

//	if (-1 !=pga_handle[0])
//		sceAudioChRelease(pga_handle[0]);

//	if (0x00 !=sound_buf)
//		free(sound_buf);

//	if (pga_handle[0]!=-1) {
//		sceAudioChRelease(pga_handle[0]);
//		pga_handle[0]=-1;
//	}

//	stream_playing = 0;
}

#ifdef PSP_DIRECT_STREAM


#else
//#if 1
int osd_update_audio_stream(void/*INT16 *buffer*/)
{
//	stream_cache_data = buffer;
//	stream_cache_len = samples_this_frame;
#if (0==PSP_MAME_STREAMING_MODE)
/* TMK 互換 */
//	cache_len +=samples_this_frame;
	cache_len +=(samples_this_frame*(/*1+*/((setting.sound_rate)/*&7*/)));
#endif // (0==PSP_MAME_STREAMING_MODE)
	/* compute how many samples to generate next frame */
	samples_left_over += samples_per_frame;
	samples_this_frame = (UINT32)samples_left_over;
	samples_left_over -= (float)samples_this_frame;

	return samples_this_frame;
}
#endif //(1==PSP_DIRECT_STREAM)



//////////////////
////////不要？？？








#if 000
static void updateaudiostream( int throttle )
{
	if (!stream_playing){	return;	}	/* error */

//	if (throttle)	/* sync with audio only when speed throttling is not turned off */
	if (0)
	{
		profiler_mark(PROFILER_IDLE);
		for (;;)
		{
			if (cache_len <PGA_SAMPLES)
				break;
			sceKernelDelayThread(1000000/60/10);
		}
		profiler_mark(PROFILER_END);
	}

//	wavinfo_bg.samplecount =stream_cache_len;
//	wavinfo_bg.wavdata =stream_cache_data;
//	wavinfo_bg.playptr =0;				// ポインタをリセット
//	wavinfo_bg.playptr_frac =0;

/*	static int cache_len =0;
	static INT16 *cache_data =0;
	cache_len +=stream_cache_len;
	if (cache_data >stream_cache_data || 0 ==cache_data) {
		cache_data =stream_cache_data;
		cache_len =stream_cache_len;
	}
	if (-1 !=pga_handle[0])
		if (!sceAudioGetChannelRestLen(pga_handle[0]) && PGA_SAMPLES <=cache_len) {
			sceAudioOutputPanned(pga_handle[0], pga_volume, pga_volume, cache_data);
			cache_len -=PGA_SAMPLES;
			cache_data +=PGA_SAMPLES*2;
		}
*/

}
#endif //000

#if 0
int msdos_update_audio( int throttle )
{
	if( soundcard == 0 || Machine->sample_rate == 0 || stream_cache_data == 0 )
	{
		return 0;
	}
#if 000
	profiler_mark(PROFILER_MIXER);
	updateaudiostream( throttle );
	profiler_mark(PROFILER_END);
#endif //000
	return 1;
}
#endif


/* attenuation in dB */
//void osd_set_mastervolume(int _attenuation)
//{
//	float volume;
//	if (_attenuation > 0){		_attenuation = 0;	}
//	if (_attenuation < -32){	_attenuation = -32;	}
//	attenuation = _attenuation;
//	volume = 256.0; /* range is 0-256 */
//	while (_attenuation++ < 0)
//	{
//		volume /= 1.122018454;	/* = (10 ^ (1/20)) = 1dB */
//	}

//	master_volume = volume;

//#ifdef USE_SEAL
//	ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
//#endif
//}


int osd_get_mastervolume(void)
{
//	return attenuation;
}




