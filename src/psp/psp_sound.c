#include "driver.h"
//#include "pg.h"
//#include "syscall.h"

//unsigned char sound_buf[512 *2 *2 *2]; // *short *streo *buff
#define PGA_SAMPLES 1024
extern int pga_handle[];
//extern int pga_volume;//<-- MODEL1

int pga_volume =0x8000; //<-- MODEL1

int nominal_sample_rate;
int soundcard,usestereo;
int attenuation = 0;
int sampleratedetect = 1;
static int master_volume = 256;

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


#define SND1_MAXSLOT 16

typedef struct {
	unsigned long channels;
	unsigned long samplerate;
	unsigned long samplecount;
	unsigned long datalength;
	char *wavdata;
	unsigned long rateratio;		// samplerate / 44100 * 0x10000
	unsigned long playptr;
	unsigned long playptr_frac;
	int playloop;
} wavout_wavinfo_t;

wavout_wavinfo_t wavinfo_bg;
wavout_wavinfo_t *wavout_snd0_wavinfo=0;
int wavout_snd0_ready=0;
int wavout_snd0_playend=0;
unsigned long wavout_snd0_playptr=0;
int wavout_snd1_playing[SND1_MAXSLOT];
unsigned long cur_play =0;
unsigned long cur_nloop;

static INT16 *cache_data;
int cache_len;

extern INT16 *finalmix;
extern int finalmixLen, finalmixCur, finalmixCurNow, finalmixCounter;

/**/
//#define PGA_CHANNELS 3
#define PGA_CHANNELS 1		// tmk 1個でよさげ？
//#define PGA_SAMPLES 512
#define PGA_SAMPLES 1024
#define MAXVOLUME 0x8000

int pga_ready=0;
int pga_handle[PGA_CHANNELS];
//int pga_volume =0x8000;

short pga_sndbuf[PGA_CHANNELS][2][PGA_SAMPLES][2];


void (*pga_channel_callback[PGA_CHANNELS])(void *buf, unsigned long reqn);

int pga_threadhandle[PGA_CHANNELS];


volatile int pga_terminate=0;

extern short int *finalmix;
extern int finalmixLen, finalmixCur, finalmixCurNow, finalmixCounter;
extern unsigned long cur_play;
extern int cache_len;

int sceAudio_38553111(unsigned short samples, unsigned short freq, char unknown);

//unsigned long osd_cycles(void);
static int pga_channel_thread(int args, void *argp)
{
static short int *lptr =0x00;

	int channel=*(int *)argp;

    //logWriteX("ARRANCO LA THREAD DE AUDIO!","","",0);

	while (pga_terminate==0) {
		if (0x00 ==finalmix) {
			sceDisplayWaitVblankStart();
			continue;
		}
		if (PGA_SAMPLES >cache_len) {
//			if (pga_ready && !sceAudioGetChannelRestLen(pga_handle[channel]))
//				sceAudioOutputPanned(pga_handle[channel], pga_volume, pga_volume, lptr);
//			sceDisplayWaitVblankStart();
			sceKernelDelayThread(1000000/60/10);
			continue;
		}
		cache_len -=PGA_SAMPLES;

		unsigned int ptr =cur_play;
		unsigned int nextptr =ptr +PGA_SAMPLES *2;
		if (nextptr >=finalmixLen) nextptr=0;
		lptr =&finalmix[ptr];
//		if (pga_ready && (cache_len*2) <finalmixLen)
		if (pga_ready && cache_len <PGA_SAMPLES*2)
//		if (pga_ready)
			sceAudioSRCOutputBlocking(pga_volume,lptr);
			//sceAudioOutputPannedBlocking(pga_handle[channel], pga_volume, pga_volume, lptr);
		cur_play =nextptr;
	}
	sceKernelExitThread(0);
	return 0;
}


static int __pga_channel_thread(int args, void *argp)
{
	volatile int bufidx=0;
	int channel=*(int *)argp;

	while (pga_terminate==0) {
		void *bufptr=&pga_sndbuf[channel][bufidx];
		void (*callback)(void *buf, unsigned long reqn);
		callback=pga_channel_callback[channel];
		if (callback) {
			callback(bufptr,PGA_SAMPLES);
		} else {
			unsigned long *ptr=bufptr;
			int i;
			for (i=0; i<PGA_SAMPLES; ++i) *(ptr++)=0;
		}
		pgaOutBlocking(channel, pga_volume, pga_volume, bufptr);
		bufidx=(bufidx?0:1);
	}
	sceKernelExitThread(0);
	return 0;
}


void pga_channel_thread_callback(int channel, void *buf, unsigned long reqn)
{
	void (*callback)(void *buf, unsigned long reqn);
	callback=pga_channel_callback[channel];
}


void pgaSetChannelCallback(int channel, void *callback)
{
	pga_channel_callback[channel]=callback;
}

/******************************************************************************************************
 * MINI PG FROM MODEL1 BEGIN
 ******************************************************************************************************/

int getChannelInUse(void)
{
int h;
char dbg3[128];

  for(h=0;h<PSP_AUDIO_CHANNEL_MAX;h++){
     if( sceAudioChReserve(h,PGA_SAMPLES,0) <0){
      sprintf(dbg3,"canal en uso: '%d'",h);
      logWriteX("PSP_SOUND: ",dbg3,"",0);
      return h;
     }else {
       sceAudioChRelease(h);
      }
   }
return -1;
}

typedef int (*pg_threadfunc_t)(int args, void *argp);

int pgaInit(void)
{
	int i,ret;
	int failed=0;
	char str[32];
    int errorReserving;

	pga_terminate=0;
	pga_ready=0;

    /*if (sceAudioSRCChReserve(PGA_SAMPLES, 22050, 2)){
        logWriteX("FUCKING HELL!!!","","",0);
        }*/

    logWriteX("entro en pgaInit...","","",666);

	for (i=0; i<PGA_CHANNELS; i++) {
		pga_handle[i]=-1;
		pga_threadhandle[i]=-1;
		pga_channel_callback[i]=0;
	}

	/*
	for (i=0; i<PGA_CHANNELS; i++) {
		// par3 - 0x10:mono
		if ((pga_handle[i]=sceAudioChReserve(-1,PGA_SAMPLES,0))<0) failed=1;
		//if ((pga_handle[i]=sceAudioSRCChReserve(-1,PGA_SAMPLES,0))<0) failed=1;
	}*/

    char dbg4[128];
    sprintf(dbg4,"Machine->sample_rate=='%d'",Machine->sample_rate);
    logWriteX("pgaInit: a punto de reservar audio: , ",dbg4,"",666);

    /*if(errorReserving = */sceAudioSRCChReserve(PGA_SAMPLES,Machine->sample_rate,2); /*){
      failed=1;
      char errRes[128];

      sprintf(errRes,"error Reservando audio: error code =='%d'",errorReserving);
      logWriteX("pgaInit: peno sceAudioSRCChReserve , ",errRes,"",666);
    }*/
    pga_handle[0]= 0;
	/*if( (pga_handle[0]= getChannelInUse() ) == -1){
	  failed=1;
	  logWriteX("pgaInit: peno getChannelInUse","","",666);
	}*/

	if (failed) {
		for (i=0; i<PGA_CHANNELS; i++) {
			if (pga_handle[i]!=-1) sceAudioChRelease(pga_handle[i]);
			pga_handle[i]=-1;
		}

		logWriteX("pgaInit: CAGO PA LA MIERDA WEON!","","",666);
		return -1;
	}
	pga_ready=1;

    logWriteX("pgaInit: stage 0","","",666);

	strcpy(str,"pgasnd0");
	for (i=0; i<PGA_CHANNELS; i++) {
		str[6]='0'+i;


        //KERNEL -original-
		pga_threadhandle[i] =sceKernelCreateThread(str, (pg_threadfunc_t)&pga_channel_thread, 0x08/*0x12*/, 0x10000, 0, 0);
		//USER
		//pga_threadhandle[i] =sceKernelCreateThread(str, (pg_threadfunc_t)&pga_channel_thread, 0x12, 0x10000, PSP_THREAD_ATTR_USER, 0);

    logWriteX("pgaInit: stage 1 - createThread done...","","",666);

		if (pga_threadhandle[i]<0) {
			pga_threadhandle[i]=-1;
			failed=1;
			break;
		}
		ret =sceKernelStartThread(pga_threadhandle[i], sizeof(i), &i);

		logWriteX("pgaInit: stage 1 - startThread done...","","",666);

		if (ret!=0) {
			failed=1;

			logWriteX("pgaInit: stage 1 - createThread failed!...","","",666);

			break;
		}
	}
	if (failed) {
		pga_terminate=1;
		for (i=0; i<PGA_CHANNELS; i++) {
			if (pga_threadhandle[i]!=-1) {
				sceKernelWaitThreadEnd(pga_threadhandle[i], 0);
				sceKernelDeleteThread(pga_threadhandle[i]);
			}
			pga_threadhandle[i]=-1;
		}
		pga_ready=0;
		return -1;
	}


	logWriteX("pgaInit: stage 2 - all OK!...","","",666);
	return 0;
}

void pgaTermPre(void)
{
	pga_ready=0;
	pga_terminate=1;
}

void pgaTerm(void)
{
	int i;
	pga_ready=0;
	pga_terminate=1;

	for (i=0; i<PGA_CHANNELS; i++) {
		if (pga_threadhandle[i]!=-1) {
			sceKernelWaitThreadEnd(pga_threadhandle[i], 0);
			sceKernelDeleteThread(pga_threadhandle[i]);
		}
		pga_threadhandle[i]=-1;
	}

	for (i=0; i<PGA_CHANNELS; i++) {
		if (pga_handle[i]!=-1) {
			sceAudioChRelease(pga_handle[i]);
			pga_handle[i]!=-1;
		}
	}

	sceAudioSRCChRelease();
}

int pgaOutBlocking(unsigned long channel,unsigned long vol1,unsigned long vol2,void *buf)
{
	if (!pga_ready) return -1;
	if (channel>=PGA_CHANNELS) return -1;
	if (vol1>MAXVOLUME) vol1=MAXVOLUME;
	if (vol2>MAXVOLUME) vol2=MAXVOLUME;
	return sceAudioOutputPannedBlocking(pga_handle[channel], vol1, vol2, buf);
}

int pgaOut(unsigned long channel,unsigned long vol1,unsigned long vol2,void *buf)
{
	if (!pga_ready) return -1;
	if (channel>=PGA_CHANNELS) return -1;
	if (vol1>MAXVOLUME) vol1=MAXVOLUME;
	if (vol2>MAXVOLUME) vol2=MAXVOLUME;
	return sceAudioOutputPanned(pga_handle[channel], vol1, vol2, buf);
}


/******************************************************************************************************
 * MINI PG FROM MODEL1 ENDS
 ******************************************************************************************************/
static inline void __memcpy4a(unsigned long *d, unsigned long *s, unsigned long c)
{
	unsigned long wk,counter;

	asm volatile (
		"		.set	push"				"\n"
		"		.set	noreorder"			"\n"

		"		move	%1,%4 "				"\n"
		"1:		lw		%0,0(%3) "			"\n"
		"		addiu	%1,%1,-1 "			"\n"
		"		addiu	%3,%3,4 "			"\n"
		"		sw		%0,0(%2) "			"\n"
		"		bnez	%1,1b "				"\n"
		"		addiu	%2,%2,4 "			"\n"

		"		.set	pop"				"\n"

			:	"=&r" (wk),		// %0
				"=&r" (counter)	// %1
			:	"r" (d),		// %2
				"r" (s),		// %3
				"r" (c)			// %4
	);
}

void pgWaitVn(unsigned long count)
{
	for (; count>0; --count) {
		sceDisplayWaitVblankStart();
	}
}


void pgWaitV()
{
	sceDisplayWaitVblankStart();
}



static void wavout_snd0_callback(short *_buf, unsigned long _reqn)
{
	if (0x00 ==finalmix || 1024 >cache_len)
	{
		memset(_buf, 0, _reqn*4);
		return;
	}
	cache_len -=1024;

	unsigned int ptr =cur_play;
	unsigned int nextptr =ptr +_reqn;
	if (nextptr >=finalmixLen) nextptr=0;
	__memcpy4a((unsigned long *)_buf, (unsigned long *)&finalmix[ptr*2], _reqn);
	cur_play =nextptr;
}

static void __wavout_snd0_callback(short *_buf, unsigned long _reqn)
{
	static int power[128];

	unsigned long i;
	unsigned long ptr,frac,rr,max;
	int channels;
	char *src;
	short *buf=_buf;
	unsigned long reqn =_reqn;

	wavout_wavinfo_t *wi =wavout_snd0_wavinfo;

	if (wi==0) {
		wavout_snd0_ready =1;
		memset(buf, 0, reqn*4);
		return;
	}

	wavout_snd0_ready=0;

	ptr=wi->playptr;
	frac=wi->playptr_frac;
	rr=wi->rateratio;
	max=wi->samplecount;
	channels=wi->channels;
	src=wi->wavdata;

	cur_nloop=0;
	if (channels==1) {
		for (; reqn>0; --reqn) {
			frac+=rr;
			ptr+=(frac>>16);
			cur_play = ptr*4;
			frac&=0xffff;
			if (ptr>=max) {
				if (wi->playloop) {
					ptr=0;
					cur_nloop++;
				} else {
					for (; reqn>0; --reqn) {
						*(buf++)=0;
						*(buf++)=0;
					}
					goto playend;
				}
			}

			buf[0]=buf[1]=*(short *)(src+ptr*2);
			buf+=2;
		}
	} else {
		for (; reqn>0; --reqn) {
			frac+=rr;
			ptr+=(frac>>16);
			cur_play = ptr*4;
			frac&=0xffff;
			if (ptr>=max) {
				if (wi->playloop) {
					ptr=0;
					cur_nloop++;
				} else {
					for (; reqn>0; --reqn) {
						*(buf++)=0;
						*(buf++)=0;
					}
					goto playend;
				}
			}

			buf[0]=*(short *)(src+ptr*4);
			buf[1]=*(short *)(src+ptr*4+2);
			buf+=2;
		}
	}

	//powercalc(_buf);	//単にwaveを出すだけなら不要

	wavout_snd0_playptr=ptr;
	wi->playptr=ptr;
	wi->playptr_frac=frac;
	return;

playend:
	wavout_snd0_playend=1;
	return;
}

//return 0 if success
int wavoutInit()
{
	int i;

	wavout_snd0_wavinfo=0;

	for (i=0; i<SND1_MAXSLOT; i++) {
		wavout_snd1_playing[i]=0;
	}

	pgaSetChannelCallback(0,wavout_snd0_callback);
	//pgaSetChannelCallback(1,wavout_snd1_callback);
	return 0;
}

void wavoutStopPlay0()
{
	if (wavout_snd0_wavinfo!=0) {
		while (wavout_snd0_ready) pgWaitV();
		wavout_snd0_wavinfo=0;
		while (!wavout_snd0_ready) pgWaitV();
	}
}

void wavoutStartPlay0(wavout_wavinfo_t *wi)
{
	wavoutStopPlay0();
	while (!wavout_snd0_ready) pgWaitV();
	wavout_snd0_playptr=0;
	wavout_snd0_playend=0;
	wavout_snd0_wavinfo=wi;
	while (wavout_snd0_ready) pgWaitV();
}

int wavoutWaitEnd0()
{
	if (wavout_snd0_wavinfo==0) return -1;
	if (wavout_snd0_wavinfo->playloop) return -1;
	while (!wavout_snd0_playend) pgWaitV();
	return 0;
}


int psp_init_sound(void)
{
	int i;
    char dbg[256];

	logWriteX("psp_init_sound: stage 0","","",666);

	stream_playing = 0;
	stream_cache_data = 0;
	stream_cache_len = 0;
	stream_cache_stereo = 0;

	/* initialize SEAL audio library */
	if( soundcard == 0 || Machine->sample_rate == 0 )
	{

		sprintf(dbg," soundcard=='%d',Machine->sample_rate =='%d'",soundcard,Machine->sample_rate);
		logWriteX("psp_init_sound: stage 1 ... soundcard==0 || Machine->sample_rate == 0 ","","",666);

		return 0;
	}

    logWriteX("psp_init_sound: stage 1","","",666);

	pgaInit();
	wavoutInit();
//	pga_handle[0] =-1;

	nominal_sample_rate = Machine->sample_rate;

	osd_set_mastervolume(attenuation);	/* set the startup volume */

    logWriteX("psp_init_sound: stage 2","","",666);

	return 0;
}


void psp_shutdown_sound(void)
{
	if( soundcard == 0 || Machine->sample_rate == 0 )
	{
		return;
	}

	wavout_snd0_wavinfo =0x00;
	pgaTerm();
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

	int sceAudioSample =(int)samples_per_frame;
//	sceAudioSample =sceAudioSample /2 *2;
//	sceAudioSample =PGA_SAMPLES;
//	if (0 >(pga_handle[0] =sceAudioChReserve(-1, sceAudioSample, 0))) {
//		char buf[100];
//		sprintf(buf, "%d =sceAudioChReserve(-1, %d, 0)", pga_handle[0], sceAudioSample);
//		printf_(buf);
//		return 0;
//	}
//		sceAudioChRelease(pga_handle[0]);

//	if (0x00 ==(sound_buf =malloc(audio_buffer_length *2 *2))) // 16bit,streo
//		return 0;
//	memset(sound_buf, 0 ,audio_buffer_length *2 *2);
//	memset(sound_buf, 0 ,sizeof(sound_buf));

	wavinfo_bg.channels =stream_cache_stereo+1;
	wavinfo_bg.samplerate =Machine->sample_rate;
	wavinfo_bg.samplecount=samples_this_frame;
	wavinfo_bg.datalength=sizeof(audio_buffer_length*2 *2);
//	wavinfo_bg.wavdata=sound_buf;
	wavinfo_bg.rateratio=((wavinfo_bg.samplerate *0x4000)/11025);
	wavinfo_bg.playptr=0;
	wavinfo_bg.playptr_frac=0;
	wavinfo_bg.playloop=1;

	wavout_snd0_wavinfo=&wavinfo_bg;

//	char buftmk[20];
//	sprintf(buftmk, "samples_per_frame[%i]", (int)samples_per_frame);
//	printf_(buftmk);

	cache_data =0x00;
	cache_len =0;
	cur_play =0;

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

//	if (-1 !=pga_handle[0]) {
//		int sceAudioSample =(int)samples_per_frame;
//		sceAudioSample =sceAudioSample /2 *2;
//		sceAudioSetChannelDataLen(pga_handle[0], sceAudioSample);
//	}
}



void osd_stop_audio_stream(void)
{
	if( soundcard == 0 || Machine->sample_rate == 0 )
	{
		return;
	}

	osd_sound_enable( 0 );

	wavout_snd0_wavinfo =0x00;

//	if (-1 !=pga_handle[0])
//		sceAudioChRelease(pga_handle[0]);

//	if (0x00 !=sound_buf)
//		free(sound_buf);

//	if (pga_handle[0]!=-1) {
//		sceAudioChRelease(pga_handle[0]);
//		pga_handle[0]=-1;
//	}

	stream_playing = 0;
}

static void updateaudiostream( int throttle )
{
	if (!stream_playing)
	{
		/* error */
		return;
	}

//	if (throttle)   /* sync with audio only when speed throttling is not turned off */
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


int osd_update_audio_stream(INT16 *buffer)
{
	stream_cache_data = buffer;
	stream_cache_len = samples_this_frame;

	cache_len +=samples_this_frame;

	/* compute how many samples to generate next frame */
	samples_left_over += samples_per_frame;
	samples_this_frame = (UINT32)samples_left_over;
	samples_left_over -= (double)samples_this_frame;

	return samples_this_frame;
}


int msdos_update_audio( int throttle )
{
	if( soundcard == 0 || Machine->sample_rate == 0 || stream_cache_data == 0 )
	{
		return 0;
	}

	profiler_mark(PROFILER_MIXER);

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

 	volume = 256.0;	/* range is 0-256 */
	while (_attenuation++ < 0)
	{
		volume /= 1.122018454;	/* = (10 ^ (1/20)) = 1dB */
	}

	master_volume = volume;

#ifdef USE_SEAL
	ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
#endif
}


int osd_get_mastervolume(void)
{
	return attenuation;
}


void osd_sound_enable(int enable_it)
{
	if( soundcard == 0 || Machine->sample_rate == 0 )
		return;

	if (enable_it)
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,master_volume);
		wavout_snd0_wavinfo=&wavinfo_bg;

		pga_volume =0x8000;
//		if (-1 !=pga_handle[0])
//			sceAudioChangeChannelVolume(pga_handle[0], 0x8000, 0x8000);
	}
	else
	{
//		ASetAudioMixerValue(AUDIO_MIXER_MASTER_VOLUME,0);
		wavout_snd0_wavinfo =0x00;

		pga_volume =0x0000;
//		if (-1 !=pga_handle[0])
//			sceAudioChangeChannelVolume(pga_handle[0], 0x0000, 0x0000);
	}
}
