
#ifndef _NAMCO86_SOUND_H_
#define _NAMCO86_SOUND_H_

/*****************************************************************************
*
*	NAMCO SYSTEM86 sound driver (YM2151 + NAMCO CUS30 + NAMCO 63701X)
*
******************************************************************************/

//#define USE_THE_SEGAPCM 1
//#ifdef fantasy_zone
//	#undef	USE_THE_SEGAPCM
	#define USE_THE_SEGAPCM 0
//#endif //fantasy_zone

#include <math.h>
#include "driver.h"
#include "psp_sound/namcos86.h"

//#include "sound/namco8s.c"
//#include "sound/namco8o.c"
//#include "./../sound/segapcm.h"

/* the sound render setting for host emulate computer. */

/* Gens_FM */

/* Rolling Thunder (new version [SOUND FM] #02 が鳴らない) */

//#include "./../psp/psp_menu.h"

////////////////
typedef INT16 FMSAMPLE;
typedef INT32 FMSAMPLE_MIX;
////////////////
static UINT8 sr;
static int sr_length;
void sr_set(int snd_rate)
{
	sr=snd_rate;
	if(0 != sr){
		sr_length=(PSP_SOUND_SAMPLES/(/*1+*/sr));
	}else
	{
		sr_length=0;
	}
}
////////////////

#if (1==USE_THE_SEGAPCM)
#include "SEGAPCM_test0.c"
#endif //(1==USE_THE_SEGAPCM)


#if 1
#include "CUS30_test0.c"
#include "HD63701_test0.c"
#endif



#include "YM2151_test0.c"

static void YM2151Update_stream(s16 *mixing_buffer, int length)
{
	int i;
//	int j;

//	for (i = 0; i < length; i++){	mixing_buffer[i] =0;	}
	memset(mixing_buffer, 0, (length+length) );

//	ym2151->eg_timer += (ym2151->eg_timer_add * length);
	ym2151->eg_timer += (ym2151->eg_timer_add_length);
	advance_eg();

//	ym2151->lfo_timer += (ym2151->lfo_timer_add * length);
	ym2151->lfo_timer += (ym2151->lfo_timer_add_length);
	advance_lfo();

i=ym2151->opm_ch[0].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[0],mixing_buffer,length);}
i=ym2151->opm_ch[1].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[1],mixing_buffer,length);}
i=ym2151->opm_ch[2].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[2],mixing_buffer,length);}
i=ym2151->opm_ch[3].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[3],mixing_buffer,length);}
i=ym2151->opm_ch[4].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[4],mixing_buffer,length);}
i=ym2151->opm_ch[5].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[5],mixing_buffer,length);}
i=ym2151->opm_ch[6].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[6],mixing_buffer,length);}
i=ym2151->opm_ch[7].ALGO_pan;	if((PAN_L|PAN_R)&(i)) { chan_calc_at_algo[(i&7)](&ym2151->opm_ch[7],mixing_buffer,length);
								if(ym2151->noise & 0x80){	chan_7_noise_mix(&ym2151->opm_ch[7],mixing_buffer,length);	}	}
//		chan7_calc();


	/* CSM is calculated *after* the phase generator calculations (verified on real chip)
	* CSM key-on line seems to be ORed with the KO line inside of the chip.
	* The result is that it only works when KO (register 0x08) is off, ie. 0
	*
	* Interesting effect is that when timer A is set to 1023, the KEY ON happens
	* on every sample, so there is no KEY OFF at all - the result is that
	* the sound played is the same as after normal KEY ON.
	*/

	if (ym2151->csm_req)			/* CSM KEYON/KEYOFF seqeunce request */
	{
		int cc;
		if (ym2151->csm_req == 2)	/* KEY ON */
		{	ym2151->csm_req = 1;	}
		else					/* KEY OFF */
		{	ym2151->csm_req = 0;	}
		for(cc=0;cc<8;cc++){
		OPM_CH	*chan;
			chan = &ym2151->opm_ch[cc];
//			op = &ym2151->oper[0];	/* CH 0 M1 */
//			i = 32;
//			do
//			{
			{
				FM_SLOT *op;
				op = &chan->SLOT[0];
				if(ym2151->csm_req)
				{
				//	FM_KEYON(op, 2);
				//	FM_KEYON(op+1, 2);
				//	FM_KEYON(op+2, 2);
				//	FM_KEYON(op+3, 2);
{
	if (!chan->SLOT[0].key)
	{
		chan->slot0_phase = 0;			/* clear phase */
		chan->SLOT[0].state = EG_ATT;		/* KEY ON = attack */
	}
	chan->SLOT[0].key |= /*key_set*/2;
}
{
	if (!chan->SLOT[1].key)
	{
		chan->slot1_phase = 0;			/* clear phase */
		chan->SLOT[1].state = EG_ATT;		/* KEY ON = attack */
	}
	chan->SLOT[1].key |= /*key_set*/2;
}
{
	if (!chan->SLOT[2].key)
	{
		chan->slot2_phase = 0;			/* clear phase */
		chan->SLOT[2].state = EG_ATT;		/* KEY ON = attack */
	}
	chan->SLOT[2].key |= /*key_set*/2;
}
{
	if (!chan->SLOT[3].key)
	{
		chan->slot3_phase = 0;			/* clear phase */
		chan->SLOT[3].state = EG_ATT;		/* KEY ON = attack */
	}
	chan->SLOT[3].key |= /*key_set*/2;
}
				}
				else
				{
					FM_KEYOFF(op, ~2);
					FM_KEYOFF(op+1, ~2);
					FM_KEYOFF(op+2, ~2);
					FM_KEYOFF(op+3, ~2);
				}
			}// while (i);
		}
	}

	/////////
#if 1
	namco_cus30_update(length);
	if (n63701x->rom) n63701x_update(length);
	for (i = 0; i < length; i++)
	{
	FMSAMPLE_MIX out_MONO;
		out_MONO  = namcos1->buffer[i];
		out_MONO += n63701x->buffer[i];

		PSP_SOUND_Limit(out_MONO, MAXOUT, MINOUT);
		mixing_buffer[i] += out_MONO;
	}
#endif
	/////////
#if 0
	#if (1==USE_THE_SEGAPCM)
	SEGAPCM_Update_stream(length);
	#endif //(1==USE_THE_SEGAPCM)
//	namco_cus30_update(length);
//	if (n63701x->rom) n63701x_update(length);
	for (i = 0; i < length; i++)
	{
	FMSAMPLE_MIX out_MONO;

		out_MONO  = mixing_buffer[i];
		out_MONO /=2;/* 50% FM */
	#if (1==USE_THE_SEGAPCM)
		out_MONO += spcm->buffer[i];
	#endif //(1==USE_THE_SEGAPCM)
	//	out_MONO  = namcos1->buffer[i];
	//	out_MONO += n63701x->buffer[i];

		PSP_SOUND_Limit(out_MONO, MAXOUT, MINOUT);
		mixing_buffer[i] = out_MONO;
	}
#endif
}





/***************************************************************************

  2151intf.c

  Support interface YM2151(OPM) + CUS30(NAMCO) + MCU_HD63701(PCM)

***************************************************************************/

//#include "driver.h"
//#include "sound/namcos86.h"

#if 0
void sndintrf_init(void){}/* src/psp/osd_config.c */
void mame_sound_frame_update(int p){}//void sound_frame_update(void)/* src/mame.c */
#endif //0


//============================================================
//
//	sndintrf.c - Sound interfece
//
//============================================================

//#include "psp_main.h"
//#include "driver.h"
//#include "sound/ns86snd.h"


/* 0:使わん, 1:使う 44100専用ミキサ－使用可否 */
#define USE_FIXED44100 0
//#define USE_FIXED44100 0

#if 1
/*	 */
static void ns86_sound_update(int p)
{
	int i;
	s16 *buffer = (s16 *)p;
	s16 mixing_buffer[PSP_SOUND_SAMPLES];
	#if (1==USE_FIXED44100)
	/* いずれにせよ44100 は未だ実用域ではない */
	if(SND_44100==(sr))
	{
		YM2151Update_stream(mixing_buffer, PSP_SOUND_SAMPLES);
		for (i = 0; i < PSP_SOUND_SAMPLES; i++)
		{
			*buffer++ = mixing_buffer[i];
		}
	}
	else
	#endif // (1==USE_FIXED44100)
	{
		YM2151Update_stream(mixing_buffer, sr_length);
	//	SEGAPCM_Update_stream(mixing_buffer, sr_length);
		for (i = 0; i < sr_length; i++)
		{volatile s16 v;
			v = mixing_buffer[i];
			switch (sr)
			{
			case SND_05512: 	*buffer++ = v;/*not_break;*//*d= 92*/
			case SND_06300: 	*buffer++ = v;/*not_break;*//*d=105*/
			case SND_07350: 	*buffer++ = v;/*not_break;*//*d=123*/
			case SND_08820: 	*buffer++ = v;/*not_break;*//*d=147*/
			case SND_11025: 	*buffer++ = v;/*not_break;*//*d=184*/
			case SND_14700: 	*buffer++ = v;/*not_break;*//*d=245*/
			#if (1==USE_FIXED44100)
			default:
			#endif // (1==USE_FIXED44100)
			case SND_22050: 	*buffer++ = v;/*not_break;*//*d=368*/
			#if (0==USE_FIXED44100)
		//	default:
/**/		case SND_44100:/**/
			#endif // (0==USE_FIXED44100)
								*buffer++ = v;/*not_break;*//*d=735*//*735[samples/flame]==44100[samples/sec]/60[flame] */
			}
		}
	}
}
#else
/* jumptable式 */
static void ns86_sound_update(int p)
{
	int i;
	s16 *buffer = (s16 *)p;
	s16 mixing_buffer[PSP_SOUND_SAMPLES];
	#if (1==USE_FIXED44100)
	/* いずれにせよ 44100 は未だ実用域ではない */
	if(SND_44100==(sr))
	{
		YM2151Update_stream(mixing_buffer, PSP_SOUND_SAMPLES);
		for (i = 0; i < PSP_SOUND_SAMPLES; i++)
		{
			*buffer++ = mixing_buffer[i];
		}
	}
	else
	#endif // (1==USE_FIXED44100)
	{
		YM2151Update_stream(mixing_buffer, sr_length);
	//	SEGAPCM_Update_stream(mixing_buffer, sr_length);
		for (i = 0; i < sr_length; i++)
		{
		const void *rate_JumpTable[] =
		{
			&&snd0,&&snd1,&&snd2,&&snd3,
			&&snd4,&&snd5,&&snd6,&&snd7,
			&&snd8
		};
			volatile s16 v;
			v = mixing_buffer[i];
		//	switch (sr)
		goto *rate_JumpTable[(sr)];
			{
	snd8:/* case SND_05512: */	*buffer++ = v;/*not_break;*//*d= 92*/
	snd7:/* case SND_06300: */	*buffer++ = v;/*not_break;*//*d=105*/
	snd6:/* case SND_07350: */	*buffer++ = v;/*not_break;*//*d=123*/
	snd5:/* case SND_08820: */	*buffer++ = v;/*not_break;*//*d=147*/
	snd4:/* case SND_11025: */	*buffer++ = v;/*not_break;*//*d=184*/
	snd3:/* case SND_14700: */	*buffer++ = v;/*not_break;*//*d=245*/
		//	#if (1==USE_FIXED44100)
		//	default:
		//	#endif // (1==USE_FIXED44100)
	snd2:/* case SND_22050: */	*buffer++ = v;/*not_break;*//*d=368*/
		//	#if (0==USE_FIXED44100)
		//	default:
/**/snd1:/* case SND_44100:*//**/
		//	#endif // (0==USE_FIXED44100)
								*buffer++ = v;/*not_break;*//*d=735*//*735[samples/flame]==44100[samples/sec]/60[flame] */
	snd0:	;					/*	 OFF */
			}
		}
	}
}
#endif

//============================================================
//	mame_sound_start
//============================================================

int mame_sound_start(void)
{
//	namcos86_sh_start();
/*------------------------------------------------------
	Start NAMCO SYSTEM86 sound emulation
 -----------------------------------------------------*/
//void namcos86_sh_start(void)
	int samplerate = 736 * 60;

#if 0
	switch (machine->sample_rate)
	{
	#if 00
	case 11025: samplerate >>= 2; break;
	case 22050: samplerate >>= 1; break;
	#else
	case  5512: samplerate /= 8; break;
	case  6300: samplerate /= 7; break;
	case  7350: samplerate /= 6; break;
	case  8820: samplerate /= 5; break;
	case 11025: samplerate /= 4; break;
	case 14700: samplerate /= 3; break;
	case 22050: samplerate /= 2; break;
/*	case 44100: samplerate /= 1; break;*/
	#endif
	}
#else
	if (sr) {samplerate /= sr; }/* if (0==sr) when, devision by zero.*/
#endif
	psp_sound->stack	= 0x10000;
	psp_sound->stereo	= 0;
	psp_sound->callback = ns86_sound_update;

#if 1
	YM2151Init(3579545, samplerate, NULL);
	namco_cus30_init(24000, samplerate, 8);
	n63701x_init(6000000, samplerate);
#endif

#if 0
	YM2151Init(4000000, samplerate, NULL);// FZもoutrunも4[MHz]
	#if (1==USE_THE_SEGAPCM)
	segapcm_start(15625, samplerate);
	#endif //(1==USE_THE_SEGAPCM)
#endif

	return psp_sound_start();
}



//============================================================
//	mame_sound_stop
//============================================================

void mame_sound_stop(void)
{
	//namcos86_sh_stop();
/*------------------------------------------------------
	Stop NAMCO SYSTEM86 sound emulation
 -----------------------------------------------------*/
//void namcos86_sh_stop(void){}

	psp_sound_stop();
}



//============================================================
//	mame_sound_reset
//============================================================

void mame_sound_reset(void)
{
//	namcos86_sh_reset();
//static void namcos86_sh_reset(void)
	YM2151Reset();
	n63701x_reset();
//	segapcm_reset();

	psp_sound_enable(1);
}



//============================================================
//	mame_sound_enable
//============================================================

//void mame_sound_enable(int enable_it)
//{
//	psp_sound_enable(enable_it);
//}
//////
/*
YM2151(test C) 4.000000 MHz
Sega PCM 15.625 kHz

*/
int sndnum_clock(int sndnum)
{
	switch(sndnum)
	{
	case 0: return 4000000; /* 4.000[MHz] */  // FZもoutrunも4[MHz]
	case 1: return 24000;	/* 15.625[kHz] */
	case 2: return 6000000;
//	case 3: return 12345;
//	case 4: return 77777;
	}
	return 0;
}
const char *sndnum_get_info_string(int sndnum, UINT32 state)
{
	switch(sndnum)
	{
	case 0: return "YM2151";	// FZもoutrunも2151.
	case 1: return "Namco CUS30"/*"SEGA PCM"*/;	// outrunのみ.
	case 2: return "Namco 63701X";
//	case 3: return "TEST 12345";
//	case 4: return "TEST 77777";
	}
	return ""/*"dummy_STR"*/;
}
//////よくワカンナイ。
#if 0
int sound_get_user_gain_count(void)
{
	return 0;
}
float sound_get_default_gain(int index)
{
	return 1.0;
}
float sound_get_user_gain(int index)
{
	return 1.0;
}
#endif

#endif // _NAMCO86_SOUND_H_

