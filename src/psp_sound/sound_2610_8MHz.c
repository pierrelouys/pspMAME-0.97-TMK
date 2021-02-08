

#ifndef _METAL_BLACK_SOUND_H_
#define _METAL_BLACK_SOUND_H_

/*****************************************************************************
*
*	NAMCO SYSTEM86 sound driver (YM2151 + NAMCO CUS30 + NAMCO 63701X)
*
******************************************************************************/



#include <math.h>
#include "driver.h"

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





/***************************************************************************

  2610_Aintf.c (test A)

  The YM2610 emulator supports up to 2 chips.
  Each chip has the following connections:
  - Status Read / Control Write A
  - Port Read / Data Write A
  - Control Write B
  - Data Write B

***************************************************************************/
/*
**
** File: fm.c -- software implementation of Yamaha FM sound generator
**
** Copyright (C) 2001, 2002, 2003 Jarek Burczynski (bujar at mame dot net)
** Copyright (C) 1998 Tatsuyuki Satoh , MultiArcadeMachineEmulator development
**
** Version 1.4 (final beta)
**
*/

/*
** History:
**
** 03-08-2003 Jarek Burczynski:
**	- fixed YM2608 initial values (after the reset)
**	- fixed flag and irqmask handling (YM2608)
**	- fixed BUFRDY flag handling (YM2608)
*/
/************************************************************************/
/*	  comment of hiro-shi(Hiromitsu Shioya) 							*/
/*	  YM2610(B) = OPN-B 												*/
/*	  YM2610  : PSG:3ch FM:4ch ADPCM(18.5KHz):6ch DeltaT ADPCM:1ch		*/
/*	  YM2610B : PSG:3ch FM:6ch ADPCM(18.5KHz):6ch DeltaT ADPCM:1ch		*/
/************************************************************************/

/***************************************************************************

  ay8910.c

  Emulation of the AY-3-8910 / YM2149 sound chip.

  Based on various code snippets by Ville Hallik, Michael Cuddy,
  Tatsuyuki Satoh, Fabrice Frances, Nicola Salmoria.

***************************************************************************/
/*
**
** File: ymdeltat.c
**
** YAMAHA DELTA-T adpcm sound emulation subroutine
** used by fmopl.c (Y8950) and fm.c (YM2608 and YM2610/B)
**
** Base program is YM2610 emulator by Hiromitsu Shioya.
** Written by Tatsuyuki Satoh
** Improvements by Jarek Burczynski (bujar at mame dot net)
**
**
** History:
**
** 03-08-2003 Jarek Burczynski:
**	- fixed BRDY flag implementation.
**
**
** Version 0.72
**
** sound chips that have this unit:
** YM2608	OPNA
** YM2610/B OPNB
** Y8950	MSX AUDIO
**
*/




#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>


#include "driver.h"
//#include "state.h"


//#if (BUILD_YM2608||BUILD_YM2610||BUILD_YM2610B)
//	#include "ymdeltat.h"/* include external DELTA-T unit  */
//#endif
#ifndef __YMDELTAT_H_
#define __YMDELTAT_H_

#define ADPCMB_SHIFT 	(16)

#define ADPCMB_EMULATION_MODE_NORMAL 0
#define ADPCMB_EMULATION_MODE_YM2610 1


typedef void (*STATUS_CHANGE_HANDLER)(/*void *chip,*/ UINT8 status_bits);


/* DELTA-T (adpcm type B) struct */
typedef struct deltat_adpcm_state		/* AT: rearranged and tigntened structure */
{
	UINT8	*pcmbufB/*memory*/;
	INT32	*output_pointer;/* pointer of output pointers	*/
	INT32	*pan;			/* pan : &output_pointer[pan]	*/
	double	freqbase;
#if 0
	double	write_time; 	/* Y8950: 10 cycles of main clock; YM2608: 20 cycles of main clock */
	double	read_time;		/* Y8950: 8 cycles of main clock;  YM2608: 18 cycles of main clock */
#endif
	UINT32	pcmsizeB/*memory_size*/;
	int output_range;
	UINT32	now_addr;		/* current address		*/
	UINT32	now_step;		/* currect step 		*/
	UINT32	step;			/* step 				*/
	UINT32	start;			/* start address		*/
	//UINT32	limit;		/* limit address(not have YM2610)*/
	UINT32	end;			/* end address			*/
	UINT32	delta;			/* delta scale			*/
	INT32	volume; 		/* current volume		*/
	INT32	acc;			/* shift Measurement value*/
	INT32	adpcmd; 		/* next Forecast		*/
	INT32	adpcml; 		/* current value		*/
	INT32	prev_acc;		/* leveling value		*/
	UINT8	now_data;		/* current rom data 	*/
	UINT8	CPU_data;		/* current data from reg 08 */
	UINT8	portstate;		/* port status			*/
//1	UINT8	control2;		/* control reg: SAMPLE, DA/AD, RAM TYPE (x8bit / x1bit), ROM/RAM */
//8	UINT8	portshift;		/* address bits shift-left:
//							** 8 for YM2610,
//							** 5 for Y8950 and YM2608 */

//0	UINT8	DRAMportshift;	/* address bits shift-right:
///							** 0 for ROM and x8bit DRAMs,
//							** 3 for x1 DRAMs */

//2	UINT8	memread;		/* needed for reading(2)/writing(0) external memory */

	/* handlers and parameters for the status flags support */
//	STATUS_CHANGE_HANDLER	status_set_handler;//YM2610_deltat_status_set()
//	STATUS_CHANGE_HANDLER	status_reset_handler;//YM2610_deltat_status_reset();

	/* note that different chips have these flags on different
	** bits of the status register
	*/
//	void *	status_change_which_chip;	/* this chip id */
	UINT8	status_change_EOS_bit;		/* 1 on End Of Sample (record/playback/cycle time of AD/DA converting has passed)*/
//0	UINT8	status_change_BRDY_bit; 	/* 1 after recording 2 datas (2x4bits) or after reading/writing 1 data */
	UINT8	status_change_ZERO_bit; 	/* 1 if silence lasts for more than 290 miliseconds on ADPCM recording */

	/* neither Y8950 nor YM2608 can generate IRQ when PCMBSY bit changes, so instead of above,
	** the statusflag gets ORed with PCM_BSY (below) (on each read of statusflag of Y8950 and YM2608)
	*/
	UINT8	PCM_BSY;		/* 1 when ADPCM is playing; Y8950/YM2608 only */

//	UINT8	reg[16];		/* adpcm registers		*/
//	UINT8	emulation_mode; /* which chip we're emulating *///ADPCMB_EMULATION_MODE_YM2610
}ADPCMB;

/*void ADPCMB_BRDY_callback(ADPCMB *adpcmb);*/

//static UINT8 ADPCMB_ADPCM_Read(ADPCMB *adpcmb);
//static void OPNB_ADPCMB_write(ADPCMB *adpcmb,int r,int v);
//void ADPCMB_ADPCM_Reset(ADPCMB *adpcmb,int pan,int emulation_mode);
//void OPNB_ADPCMB_calc(ADPCMB *adpcmb);

//void ADPCMB_postload(ADPCMB *adpcmb,UINT8 *regs);
//void ADPCMB_savestate(const char *statename,int num,ADPCMB *adpcmb);

#endif




//#include "ay8910ym.h"
#include "sound/2610intf.h"

#if 0
#include "fm.h"
#else
#define BUILD_YM2608  (HAS_YM2608)		/* build YM2608(OPNA)  emulator */
#define BUILD_YM2610  (HAS_YM2610)		/* build YM2610(OPNB)  emulator */
#define BUILD_YM2610B (HAS_YM2610B)		/* build YM2610B(OPNB?)emulator */

#define FM_SAMPLE_BITS 16
#define FM_INTERNAL_TIMER 0
#define FM_BUSY_FLAG_SUPPORT 1
#define FM_GET_TIME_NOW() timer_get_time()
//typedef stream_sample_t FMSAMPLE;
typedef void (*FM_TIMERHANDLER_2610)(/*void *param,*/int c,int cnt,double stepTime);
typedef void (*FM_IRQHANDLER_2610)(/*void *param,*/int irq);

#endif // 0

#ifndef PI
#define PI 3.14159265358979323846
#endif






/* shared function building option */
#define BUILD_OPN (BUILD_YM2608||BUILD_YM2610||BUILD_YM2610B)

#define ADPCMB_DELTA_MAX (24576)
#define ADPCMB_DELTA_MIN (127)
#define ADPCMB_DELTA_DEF (127)

#define ADPCMB_DECODE_RANGE 32768
#define ADPCMB_DECODE_MIN (-(ADPCMB_DECODE_RANGE))
#define ADPCMB_DECODE_MAX ((ADPCMB_DECODE_RANGE)-1)





//#include "ym2610_Ainc.c"

//#define YM2610B_WARNING
//#undef YM2610B_WARNING



//undef (44100)
//2==RENDER_DIV_TYPE  // cheat div2 render. // normal.(22050)
//3==RENDER_DIV_TYPE  // cheat div3 render. // lowfi+.(14700)

//#define RENDER_DIV_TYPE 3
#define RENDER_DIV_TYPE 2



//#define RENDER_DIV72_MODE 0
  #define RENDER_DIV72_MODE 1

#if (1==RENDER_DIV72_MODE)
//	#define CLOCK_DIV72 (clock/72)/* MAME1.16ではこうなってた(ok?) */
//	#define CLOCK_DIV72_OPN ((clock/72)/RENDER_DIV_TYPE)/*(clock/(72*RENDER_DIV_TYPE))同じかと*//*((clock/72)/RENDER_DIV_TYPE)*//* この辺で勘弁。 */

	#if 1
		#define CLOCK_DIV72_OPN        (/*rate*/machine->sample_rate/*machine->sample_rate*/)/* */
		#define CLOCK_DIV72_OPN_STREAM (/*rate*/machine->sample_rate/*machine->sample_rate*/)/* */
	#else
		#define CLOCK_DIV72_OPN        (/*rate*/rate/*machine->sample_rate*/)/* */
		#define CLOCK_DIV72_OPN_STREAM (/*rate*/rate/*machine->sample_rate*/)/* */
	#endif

//	#define CLOCK_DIV72_SSG ((clock/(72*RENDER_DIV_TYPE))*(machine->sample_rate/44100))/*((clock/72)/RENDER_DIV_TYPE)*//* ダメ */
#else
	#define CLOCK_DIV72 (machine->sample_rate)/* MAME0.97ではこうなってた(mistake?) */
//	#define CLOCK_DIV72 ((machine->sample_rate)/RENDER_DIV_TYPE)/*low-fi過ぎる...*/
#endif

/*
  8000000.000 (8M[Hz])
   111111.111 ( 111.1k[Hz]==8M[Hz]/72  @44100)
    44100.0   (  44.1k[Hz])
    55555.555 (  55.5k[Hz]==8M[Hz]/72  @22050)
*/
#if 0
static struct  ym2610_info_t
{
	//sound_stream *	stream;
	mame_timer *	timer[2];
//	void *			chip;
//	void *			psg;
//	const struct YM2610interface *intf;
//struct ym2610_info/*Z_AY8910*/
//{
//	UINT8 Regs[16];
//	UINT8 Hold;
//	UINT8 Alternate;
//	UINT8 Attack;
//	UINT8 Holding;
//	INT32 RNG;
//	unsigned int VolTable[32];

//	INT32 lastEnable;//force write 2610

//1	/*int*/UINT8 streams;
//	/*int*/UINT8 index;
//	int ready;
//#if 0
//	sound_stream *Channel;
//#endif
//	int SampleRate;//==machine->sample_rate
//	read8_handler PortAread;
//	read8_handler PortBread;
//	write8_handler PortAwrite;
//	write8_handler PortBwrite;
//	INT32 register_latch;
//};
} my_ym2610_info_aaa/*YM2610_INFO*/;
//static struct ym2610_info_t *my_ym2610_info_aaa;
//	my_ym2610_info_aaa = auto_malloc(sizeof(*my_ym2610_info_aaa));
#endif



#define SSG_MAX_OUTPUT 0x7fff

#define SSG_STEP 0x8000

/* SSG register ID */
#define SSG_AFINE		(0)
#define SSG_ACOARSE 	(1)
#define SSG_BFINE		(2)
#define SSG_BCOARSE 	(3)
#define SSG_CFINE		(4)
#define SSG_CCOARSE 	(5)
#define SSG_NOISEPER	(6)
#define SSG_ENABLE		(7)
#define SSG_AVOL		(8)
#define SSG_BVOL		(9)
#define SSG_CVOL		(10)
#define SSG_EFINE		(11)
#define SSG_ECOARSE 	(12)
#define SSG_ESHAPE		(13)

#define SSG_PORTA		(14)
#define SSG_PORTB		(15)







/* SSG struct */
static struct SSG_t
{
	UINT32 UpdateStep/*step*/;//Update Step;

	INT32 PeriodA;
	INT32 PeriodB;
	INT32 PeriodC;
	INT32 PeriodN;
	INT32 PeriodE;

	INT32 CountA;
	INT32 CountB;
	INT32 CountC;
	INT32 CountN;
	INT32 CountE;
	UINT16/*32*/ VolA;
	UINT16/*32*/ VolB;
	UINT16/*32*/ VolC;
	UINT16/*32*/ VolE;
	UINT8 OutputA;
	UINT8 OutputB;
	UINT8 OutputC;
	UINT8 OutputN;
	UINT8 EnvelopeA;
	UINT8 EnvelopeB;
	UINT8 EnvelopeC;
	INT8 count_env;//Count_Env;
//
	UINT8 Hold;
	UINT8 Alternate;
	UINT8 Attack;
	UINT8 Holding;
} SSG;


/* globals */
#define TYPE_SSG	0x01	/* SSG support			*/
#define TYPE_LFOPAN 0x02	/* OPN type LFO and PAN */
#define TYPE_6CH	0x04	/* FM 6CH / 3CH 		*/
#define TYPE_DAC	0x08	/* YM2612's DAC device	*/
#define TYPE_ADPCM	0x10	/* two ADPCM units		*/


#define TYPE_YM2203 (TYPE_SSG)
#define TYPE_YM2608 (TYPE_SSG |TYPE_LFOPAN |TYPE_6CH |TYPE_ADPCM)
#define TYPE_YM2610 (TYPE_SSG |TYPE_LFOPAN |TYPE_6CH |TYPE_ADPCM)
#define TYPE_YM2612 (TYPE_DAC |TYPE_LFOPAN |TYPE_6CH)

#define NO_USE_2203 1


#define FREQ_SH 		16	/* 16.16 fixed point (frequency calculations) */
#define EG_SH			16	/* 16.16 fixed point (envelope generator timing) */
#define LFO_SH			24	/*	8.24 fixed point (LFO calculations) 	  */
#define TIMER_SH		16	/* 16.16 fixed point (timers calculations)	  */

#define FREQ_MASK		((1<<FREQ_SH)-1)

#define ENV_BITS		10
#define ENV_LEN 		(1<<ENV_BITS)
#define ENV_STEP		(128.0/ENV_LEN)

#define MAX_ATT_INDEX	(ENV_LEN-1) /* 1023 */
#define MIN_ATT_INDEX	(0) 		/* 0 */

#define EG_ATT			4
#define EG_DEC			3
#define EG_SUS			2
#define EG_REL			1
#define EG_OFF			0

#define SIN_BITS		10
#define SIN_LEN 		(1<<SIN_BITS)
#define SIN_MASK		(SIN_LEN-1)

#define TL_RES_LEN		(256) /* 8 bits addressing (real chip) */


#if (FM_SAMPLE_BITS==16)
//	#define FINAL_SH	(0)
	#define MAXOUT		(+32767)
	#define MINOUT		(-32768)
#else
//	#define FINAL_SH	(8)
	#define MAXOUT		(+127)
	#define MINOUT		(-128)
#endif


/*	TL_TAB_LEN is calculated as:
*	13 - sinus amplitude bits	  (Y axis)
*	2  - sinus sign bit 		  (Y axis)
*	TL_RES_LEN - sinus resolution (X axis)
*/
#define TL_TAB_LEN (13*2*TL_RES_LEN)
static signed int tl_tab[TL_TAB_LEN];

#define ENV_QUIET		(TL_TAB_LEN>>3)

/* sin waveform table in 'decibel' scale */
static unsigned int sin_tab[SIN_LEN];

/* sustain level table (3dB per step) */
/* bit0, bit1, bit2, bit3, bit4, bit5, bit6 */
/* 1,	 2,    4,	 8,    16,	 32,   64	(value)*/
/* 0.75, 1.5,  3,	 6,    12,	 24,   48	(dB)*/

/* 0 - 15: 0, 3, 6, 9,12,15,18,21,24,27,30,33,36,39,42,93 (dB)*/
#define SC(db) (UINT32) ( db * (4.0/ENV_STEP) )
static const UINT32 sl_table[16]={
 SC( 0),SC( 1),SC( 2),SC(3 ),SC(4 ),SC(5 ),SC(6 ),SC( 7),
 SC( 8),SC( 9),SC(10),SC(11),SC(12),SC(13),SC(14),SC(31)
};
#undef SC


#define RATE_STEPS (8)
static const UINT8 eg_inc[19*RATE_STEPS]={

/*cycle:0 1  2 3  4 5  6 7*/

/* 0 */ 0,1, 0,1, 0,1, 0,1, /* rates 00..11 0 (increment by 0 or 1) */
/* 1 */ 0,1, 0,1, 1,1, 0,1, /* rates 00..11 1 */
/* 2 */ 0,1, 1,1, 0,1, 1,1, /* rates 00..11 2 */
/* 3 */ 0,1, 1,1, 1,1, 1,1, /* rates 00..11 3 */

/* 4 */ 1,1, 1,1, 1,1, 1,1, /* rate 12 0 (increment by 1) */
/* 5 */ 1,1, 1,2, 1,1, 1,2, /* rate 12 1 */
/* 6 */ 1,2, 1,2, 1,2, 1,2, /* rate 12 2 */
/* 7 */ 1,2, 2,2, 1,2, 2,2, /* rate 12 3 */

/* 8 */ 2,2, 2,2, 2,2, 2,2, /* rate 13 0 (increment by 2) */
/* 9 */ 2,2, 2,4, 2,2, 2,4, /* rate 13 1 */
/*10 */ 2,4, 2,4, 2,4, 2,4, /* rate 13 2 */
/*11 */ 2,4, 4,4, 2,4, 4,4, /* rate 13 3 */

/*12 */ 4,4, 4,4, 4,4, 4,4, /* rate 14 0 (increment by 4) */
/*13 */ 4,4, 4,8, 4,4, 4,8, /* rate 14 1 */
/*14 */ 4,8, 4,8, 4,8, 4,8, /* rate 14 2 */
/*15 */ 4,8, 8,8, 4,8, 8,8, /* rate 14 3 */

/*16 */ 8,8, 8,8, 8,8, 8,8, /* rates 15 0, 15 1, 15 2, 15 3 (increment by 8) */
/*17 */ 16,16,16,16,16,16,16,16, /* rates 15 2, 15 3 for attack */
/*18 */ 0,0, 0,0, 0,0, 0,0, /* infinity rates for attack and decay(s) */
};


#define O(a) (a*RATE_STEPS)

/*note that there is no O(17) in this table - it's directly in the code */
static const UINT8 eg_rate_select[32+64+32]={	/* Envelope Generator rates (32 + 64 rates + 32 RKS) */
/* 32 infinite time rates */
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),
O(18),O(18),O(18),O(18),O(18),O(18),O(18),O(18),

/* rates 00-11 */
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),
O( 0),O( 1),O( 2),O( 3),

/* rate 12 */
O( 4),O( 5),O( 6),O( 7),

/* rate 13 */
O( 8),O( 9),O(10),O(11),

/* rate 14 */
O(12),O(13),O(14),O(15),

/* rate 15 */
O(16),O(16),O(16),O(16),

/* 32 dummy rates (same as 15 3) */
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16),
O(16),O(16),O(16),O(16),O(16),O(16),O(16),O(16)

};
#undef O

/*rate	0,	  1,	2,	 3,   4,   5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15*/
/*shift 11,   10,	9,	 8,   7,   6,  5,  4,  3,  2, 1,  0,  0,  0,  0,  0 */
/*mask	2047, 1023, 511, 255, 127, 63, 31, 15, 7,  3, 1,  0,  0,  0,  0,  0 */

#define O(a) (a*1)
static const UINT8 eg_rate_shift[32+64+32]={	/* Envelope Generator counter shifts (32 + 64 rates + 32 RKS) */
/* 32 infinite time rates */
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),
O(0),O(0),O(0),O(0),O(0),O(0),O(0),O(0),

/* rates 00-11 */
O(11),O(11),O(11),O(11),
O(10),O(10),O(10),O(10),
O( 9),O( 9),O( 9),O( 9),
O( 8),O( 8),O( 8),O( 8),
O( 7),O( 7),O( 7),O( 7),
O( 6),O( 6),O( 6),O( 6),
O( 5),O( 5),O( 5),O( 5),
O( 4),O( 4),O( 4),O( 4),
O( 3),O( 3),O( 3),O( 3),
O( 2),O( 2),O( 2),O( 2),
O( 1),O( 1),O( 1),O( 1),
O( 0),O( 0),O( 0),O( 0),

/* rate 12 */
O( 0),O( 0),O( 0),O( 0),

/* rate 13 */
O( 0),O( 0),O( 0),O( 0),

/* rate 14 */
O( 0),O( 0),O( 0),O( 0),

/* rate 15 */
O( 0),O( 0),O( 0),O( 0),

/* 32 dummy rates (same as 15 3) */
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),
O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0),O( 0)

};
#undef O

static const UINT8 dt_tab[4 * 32]={
/* this is YM2151 and YM2612 phase increment data (in 10.10 fixed point format)*/
/* FD=0 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
/* FD=1 */
	0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2,
	2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7, 8, 8, 8, 8,
/* FD=2 */
	1, 1, 1, 1, 2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5,
	5, 6, 6, 7, 8, 8, 9,10,11,12,13,14,16,16,16,16,
/* FD=3 */
	2, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 6, 6, 7,
	8 , 8, 9,10,11,12,13,14,16,17,19,20,22,22,22,22
};


/* OPN key frequency number -> key code follow table */
/* fnum higher 4bit -> keycode lower 2bit */
static const UINT8 opn_fktable[16] = {0,0,0,0,0,0,0,1,2,3,3,3,3,3,3,3};








/*There are 8 different LFO PM depths available, they are:
  0, 3.4, 6.7, 10, 14, 20, 40, 80 (cents)

  Modulation level at each depth depends on F-NUMBER bits: 4,5,6,7,8,9,10
  (bits 8,9,10 = FNUM MSB from OCT/FNUM register)

  Here we store only first quarter (positive one) of full waveform.
  Full table (lfo_pm_table) containing all 128 waveforms is build
  at run (init) time.

  One value in table below represents 4 (four) basic LFO steps
  (1 PM step = 4 AM steps).

  For example:
   at LFO SPEED=0 (which is 108 samples per basic LFO step)
   one value from "lfo_pm_output" table lasts for 432 consecutive
   samples (4*108=432) and one full LFO waveform cycle lasts for 13824
   samples (32*432=13824; 32 because we store only a quarter of whole
			waveform in the table below)
*/
static const UINT8 lfo_pm_output[7*8][8]={ /* 7 bits meaningful (of F-NUMBER), 8 LFO output levels per one depth (out of 32), 8 LFO depths */
/* FNUM BIT 4: 000 0001xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 2 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 3 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 4 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 5 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 6 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 7 */ {0,	0,	 0,   0,   1,	1,	 1,   1},

/* FNUM BIT 5: 000 0010xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 2 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 3 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 4 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 5 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 6 */ {0,	0,	 0,   0,   1,	1,	 1,   1},
/* DEPTH 7 */ {0,	0,	 1,   1,   2,	2,	 2,   3},

/* FNUM BIT 6: 000 0100xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 2 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 3 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 4 */ {0,	0,	 0,   0,   0,	0,	 0,   1},
/* DEPTH 5 */ {0,	0,	 0,   0,   1,	1,	 1,   1},
/* DEPTH 6 */ {0,	0,	 1,   1,   2,	2,	 2,   3},
/* DEPTH 7 */ {0,	0,	 2,   3,   4,	4,	 5,   6},

/* FNUM BIT 7: 000 1000xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 2 */ {0,	0,	 0,   0,   0,	0,	 1,   1},
/* DEPTH 3 */ {0,	0,	 0,   0,   1,	1,	 1,   1},
/* DEPTH 4 */ {0,	0,	 0,   1,   1,	1,	 1,   2},
/* DEPTH 5 */ {0,	0,	 1,   1,   2,	2,	 2,   3},
/* DEPTH 6 */ {0,	0,	 2,   3,   4,	4,	 5,   6},
/* DEPTH 7 */ {0,	0,	 4,   6,   8,	8, 0xa, 0xc},

/* FNUM BIT 8: 001 0000xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   1,	1,	 1,   1},
/* DEPTH 2 */ {0,	0,	 0,   1,   1,	1,	 2,   2},
/* DEPTH 3 */ {0,	0,	 1,   1,   2,	2,	 3,   3},
/* DEPTH 4 */ {0,	0,	 1,   2,   2,	2,	 3,   4},
/* DEPTH 5 */ {0,	0,	 2,   3,   4,	4,	 5,   6},
/* DEPTH 6 */ {0,	0,	 4,   6,   8,	8, 0xa, 0xc},
/* DEPTH 7 */ {0,	0,	 8, 0xc,0x10,0x10,0x14,0x18},

/* FNUM BIT 9: 010 0000xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   2,	2,	 2,   2},
/* DEPTH 2 */ {0,	0,	 0,   2,   2,	2,	 4,   4},
/* DEPTH 3 */ {0,	0,	 2,   2,   4,	4,	 6,   6},
/* DEPTH 4 */ {0,	0,	 2,   4,   4,	4,	 6,   8},
/* DEPTH 5 */ {0,	0,	 4,   6,   8,	8, 0xa, 0xc},
/* DEPTH 6 */ {0,	0,	 8, 0xc,0x10,0x10,0x14,0x18},
/* DEPTH 7 */ {0,	0,0x10,0x18,0x20,0x20,0x28,0x30},

/* FNUM BIT10: 100 0000xxxx */
/* DEPTH 0 */ {0,	0,	 0,   0,   0,	0,	 0,   0},
/* DEPTH 1 */ {0,	0,	 0,   0,   4,	4,	 4,   4},
/* DEPTH 2 */ {0,	0,	 0,   4,   4,	4,	 8,   8},
/* DEPTH 3 */ {0,	0,	 4,   4,   8,	8, 0xc, 0xc},
/* DEPTH 4 */ {0,	0,	 4,   8,   8,	8, 0xc,0x10},
/* DEPTH 5 */ {0,	0,	 8, 0xc,0x10,0x10,0x14,0x18},
/* DEPTH 6 */ {0,	0,0x10,0x18,0x20,0x20,0x28,0x30},
/* DEPTH 7 */ {0,	0,0x20,0x30,0x40,0x40,0x50,0x60},

};

/* all 128 LFO PM waveforms */
static INT32 lfo_pm_table[128*8*32]; /* 128 combinations of 7 bits meaningful (of F-NUMBER), 8 LFO depths, 32 LFO output levels per one depth */





/* register number to channel number , slot offset */
#define OPN_CHAN(N) (N&3)
#define OPN_SLOT(N) ((N>>2)&3)

/* slot number */
#define SLOT1 0
#define SLOT2 2
#define SLOT3 1
#define SLOT4 3

/* bit0 = Right enable , bit1 = Left enable */
#define OUTD_RIGHT	1
#define OUTD_LEFT	2
#define OUTD_CENTER 3




/* struct describing a single operator (SLOT) */
typedef struct
{
	INT32	*DT;		/* detune		   :dt_tab[DT] */
	UINT8/*32*/ ar; 		/* attack rate	*/
	UINT8/*32*/ d1r;		/* decay rate	*/
	UINT8/*32*/ d2r;		/* sustain rate */
	UINT8/*32*/ rr; 		/* release rate */
	UINT8	ksr;		/* key scale rate  :kcode>>(3-KSR) */
	UINT8	KSR;		/* key scale rate  :3-KSR */

	/* Envelope Generator */
	UINT8	ssg;		/* SSG-EG waveform */
	UINT8	ssgn;		/* SSG-EG negated output */

	UINT32	mul;		/* multiple 	   :ML_TABLE[ML] */

	/* Phase Generator */
	UINT32	phase;		/* phase counter */
	UINT32	Incr;		/* phase step */

	/* Envelope Generator */
	UINT16/*32*/	tl; 		/* total level: TL << 3 */
	INT16/*32*/ volume; 	/* envelope counter */
	UINT32	sl; 		/* sustain level:sl_table[SL] */
	UINT32	vol_out;	/* current output from EG circuit (without AM from LFO) */
	UINT8	state;		/* phase type */

	UINT8	eg_sh_ar;	/*	(attack state) */
	UINT8	eg_sel_ar;	/*	(attack state) */
	UINT8	eg_sh_d1r;	/*	(decay state) */
	UINT8	eg_sel_d1r; /*	(decay state) */
	UINT8	eg_sh_d2r;	/*	(sustain state) */
	UINT8	eg_sel_d2r; /*	(sustain state) */
	UINT8	eg_sh_rr;	/*	(release state) */
	UINT8	eg_sel_rr;	/*	(release state) */

	UINT32	key;		/* 0=last key was KEY OFF, 1=KEY ON */

	/* LFO */
	UINT16/*32*/	AMmask; 	/* AM enable flag */
} FM_SLOT;

typedef struct
{
	FM_SLOT SLOT[4];	/* four SLOTs (operators) */

	UINT8	ALGO;		/* algorithm */
	UINT8	FB; 		/* feedback shift */
	INT32	op1_out[2]; /* op1 output for feedback */

	INT32	*connect1;	/* SLOT1 output pointer */
	INT32	*connect3;	/* SLOT3 output pointer */
	INT32	*connect2;	/* SLOT2 output pointer */
	INT32	*connect4;	/* SLOT4 output pointer */

	INT32	*mem_connect;/* where to put the delayed sample (MEM) */
	INT32	mem_value;	/* delayed sample (MEM) value */

	INT32	pms;		/* channel PMS */
	UINT8	ams;		/* channel AMS */

	UINT32	fc; 		/* fnum,blk:adjusted to sample rate */
	UINT8	kcode;		/* key code:						*/
	UINT32	block_fnum; /* current blk/fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
} FM_CH;


typedef struct
{
//	void *	param;		/* this chip parameter	*/
	int 	clock;		/* master clock  (Hz)	*/
	int 	rate_OPN;		/* sampling rate (Hz)	*/
//	int 	rate_SSG;		/* sampling rate (Hz)	*/
	double	freqbase_OPN;	/* frequency base		*/
//	double	freqbase_SSG;	/* frequency base		*/
	double	TimerBase;	/* Timer base time		*/
#if FM_BUSY_FLAG_SUPPORT
	double	BusyExpire; /* ExpireTime of Busy clear */
#endif
	UINT8	address;	/* address register 	*/
	UINT8	irq;		/* interrupt level		*/
	UINT8	irqmask;	/* irq mask 			*/
	UINT8	status; 	/* status flag			*/
	UINT32	mode;		/* mode  CSM / 3SLOT	*/
	UINT8	prescaler_sel;/* prescaler selector */
	UINT8	fn_h;		/* freq latch			*/
	int 	TA; 		/* timer a				*/
	int 	TAC;		/* timer a counter		*/
	UINT8	TB; 		/* timer b				*/
	int 	TBC;		/* timer b counter		*/
	/* local time tables */
	INT32	dt_tab[8][32];/* DeTune table		*/
	/* Extention Timer and IRQ handler */
	FM_TIMERHANDLER_2610 Timer_Handler;
	FM_IRQHANDLER_2610	IRQ_Handler;
//	const struct ssg_callbacks *SSG;
} FM_ST;



/***********************************************************/
/* OPN unit 											   */
/***********************************************************/

/* OPN 3slot struct */
typedef struct
{
	UINT32	fc[3];			/* fnum3,blk3: calculated */
	UINT8	fn_h;			/* freq3 latch */
	UINT8	kcode[3];		/* key code */
	UINT32	block_fnum[3];	/* current fnum value for this slot (can be different betweeen slots of one channel in 3slot mode) */
} FM_3SLOT;

/* OPN/A/B common state */
typedef struct
{
	UINT8	type;			/* chip type */
	FM_ST	ST; 			/* general state */
	FM_3SLOT SL3;			/* 3 slot mode state */
	FM_CH	*P_CH;			/* pointer of CH */
	unsigned int pan[6*2];	/* fm channels output masks (0xffffffff = enable) */

	UINT32	eg_cnt; 		/* global envelope generator counter */
	UINT32	eg_timer;		/* global envelope generator counter works at frequency = chipclock/64/3 */
	UINT32	eg_timer_add;	/* step of eg_timer */
	UINT32	eg_timer_overflow;/* envelope generator timer overlfows every 3 samples (on real chip) */


	/* there are 2048 FNUMs that can be generated using FNUM/BLK registers
		but LFO works with one more bit of a precision so we really need 4096 elements */

	UINT32	fn_table[4096]; /* fnumber->increment counter */


	/* LFO */
	UINT32	lfo_cnt;
	UINT32	lfo_inc;

	UINT32	lfo_freq[8];	/* LFO FREQ table */
} FM_OPN;



/* current chip state */
//static void	*cur_chip = 0;	/* pointer of current chip struct */
//static FM_ST	*State; 		/* basic status */
static FM_CH	*cch[8];		/* pointer of FM channels */

static INT32	m2,c1,c2;		/* Phase Modulation input for operators 2,3,4 */
static INT32	mem;			/* one sample delay memory */

static INT32	out_fm[8];		/* outputs of working channels */
static INT32	out_ssg;		/* outputs of monoral for SSG one samples. */


#if (BUILD_YM2608||BUILD_YM2610||BUILD_YM2610B)
static INT32	out_adpcm[4];	/* channel output NONE,LEFT,RIGHT or CENTER for YM2608/YM2610 ADPCM */
static INT32	out_delta[4];	/* channel output NONE,LEFT,RIGHT or CENTER for YM2608/YM2610 DELTAT*/
#endif

static UINT32	LFO_AM; 		/* runtime LFO calculations helper */
static INT32	LFO_PM; 		/* runtime LFO calculations helper */


/* log output level */
#define LOG_ERR  3		/* ERROR	   */
#define LOG_WAR  2		/* WARNING	   */
#define LOG_INF  1		/* INFORMATION */
#define LOG_LEVEL LOG_INF



/* ADPCM type A channel struct */
typedef struct
{
	UINT8		flag;			/* port state				*/
	UINT8		flagMask;		/* arrived flag mask		*/
	UINT8		now_data;		/* current ROM data 		*/
	UINT32		now_addr;		/* current ROM address		*/
	UINT32		now_step;
	UINT32		step;
	UINT32		start;			/* sample data start address*/
	UINT32		end;			/* sample data end address	*/
	UINT8		IL; 			/* Instrument Level 		*/
	INT32		adpcm_acc;		/* accumulator				*/
	INT32		adpcm_step; 	/* step 					*/
	INT32		adpcm_out;		/* (speedup) hiro-shi!! 	*/
	INT8		vol_mul;		/* volume in "0.75dB" steps */
	UINT8		vol_shift;		/* volume in "-6dB" steps	*/
	INT32		*pan;			/* &out_adpcm[OPN_xxxx] 	*/
} ADPCM_CH;

/* here's the virtual YM2610 */
/*typedef*/ static struct ym2610_t
{
//#ifdef _STATE_H
	UINT8		REGS[512];			/* registers			*/
//	UINT32		adpcmreg[0x30]; 	/* registers(0x100-0x12f)			*/
//#endif
	FM_OPN		OPN;				/* OPN state			*/
	FM_CH		CH[6];				/* channel state		*/
	UINT8		addr_A1;			/* address line A1		*/

	/* ADPCM-A unit */
	UINT8		adpcmTL;			/* adpcmA total level	*/
	ADPCM_CH	adpcm[6];			/* adpcm channels		*/
	UINT8		adpcm_arrivedEndAddress;

	UINT8	*pcmbufA;	//	*pcmbuf;			/* pcm rom buffer		*/
	UINT32	pcmsizeA;	//	pcm_size;			/* size of pcm rom		*/

	ADPCMB	deltaT; 			/* Delta-T ADPCM unit	*/

	mame_timer *my_ym2610_info_aaa_timer[2];
//	UINT8		flagmask;			/* YM2608 only */
//	UINT8		irqmask;			/* YM2608 only */
} YM2610;

/* here is the virtual YM2608 */
//typedef YM2610 YM2608;


/**** YM2610 ADPCM defines ****/
#define ADPCM_SHIFT    (16) 	 /* frequency step rate   */
#define ADPCMA_ADDRESS_SHIFT 8	 /* adpcm A address shift */

//static UINT8 *pcmbufA;
//static UINT32 pcmsizeA;










static const UINT16 PSG_VolTable[32]={
(int)(     0),//(int)(      0  ),//    0,//     0(int)(        0) //(     0) //(     0) //15:00
(int)(     0),//(int)(      0  ),//    0,//     0(int)(        0) //(     0) //(     0) //15:00
(int)(  1*24),//(int)(  1*128-1),//   28,//  2844(int)(  2*14.22),//(  2*15),//(  2*14),//14:02
(int)(  2*24),//(int)(  2*128-1),//   28,//  2844(int)(  2*14.22),//(  2*15),//(  2*14),//14:02
(int)(  4*24),//(int)(  4*128-1),//   57,//  5688(int)(  4*14.22),//(  4*15),//(  4*14),//13:04
(int)(  4*24),//(int)(  4*128-1),//   57,//  5688(int)(  4*14.22),//(  4*15),//(  4*14),//13:04
(int)(  5*24),//(int)(  5*128-1),//   71,//  7110(int)(  5*14.22),//(  5*15),//(  5*14),//12:05
(int)(  5*24),//(int)(  5*128-1),//   71,//  7110(int)(  5*14.22),//(  5*15),//(  5*14),//12:05
(int)(  6*24),//(int)(  6*128-1),//   99,//  9954(int)(  7*14.22),//(  7*15),//(  7*14),//11:07
(int)(  7*24),//(int)(  7*128-1),//   99,//  9954(int)(  7*14.22),//(  7*15),//(  7*14),//11:07
(int)(  8*24),//(int)(  8*128-1),//  142,// 14220(int)( 10*14.22),//( 10*15),//( 10*14),//10:0a
(int)( 10*24),//(int)( 10*128-1),//  142,// 14220(int)( 10*14.22),//( 10*15),//( 10*14),//10:0a
(int)( 12*24),//(int)( 12*128-1),//  199,// 19908(int)( 14*14.22),//( 14*15),//( 14*14),// 9:0e
(int)( 14*24),//(int)( 14*128-1),//  199,// 19908(int)( 14*14.22),//( 14*15),//( 14*14),// 9:0e
(int)( 16*24),//(int)( 16*128-1),//  270,// 27018(int)( 19*14.22),//( 19*15),//( 19*14),// 8:13
(int)( 19*24),//(int)( 19*128-1),//  270,// 27018(int)( 19*14.22),//( 19*15),//( 19*14),// 8:13
(int)( 24*24),//(int)( 24*128-1),//  384,// 38394(int)( 27*14.22),//( 27*15),//( 27*14),// 7:1b
(int)( 27*24),//(int)( 27*128-1),//  384,// 38394(int)( 27*14.22),//( 27*15),//( 27*14),// 7:1b
(int)( 32*24),//(int)( 32*128-1),//  526,// 52614(int)( 37*14.22),//( 37*15),//( 37*14),// 6:25
(int)( 37*24),//(int)( 37*128-1),//  526,// 52614(int)( 37*14.22),//( 37*15),//( 37*14),// 6:25
(int)( 48*24),//(int)( 48*128-1),//  739,// 73944(int)( 52*14.22),//( 52*15),//( 52*14),// 5:34
(int)( 52*24),//(int)( 52*128-1),//  739,// 73944(int)( 52*14.22),//( 52*15),//( 52*14),// 5:34
(int)( 64*24),//(int)( 64*128-1),//  995,// 99540(int)( 70*14.22),//( 70*15),//( 70*14),// 4:46
(int)( 70*24),//(int)( 70*128-1),//  995,// 99540(int)( 70*14.22),//( 70*15),//( 70*14),// 4:46
(int)( 96*24),//(int)( 96*128-1),// 1422,//142200(int)(100*14.22),//(100*15),//(100*14),// 3:64
(int)(100*24),//(int)(100*128-1),// 1422,//142200(int)(100*14.22),//(100*15),//(100*14),// 3:64
(int)(128*24),//(int)(128*128-1),// 1891,//189216(int)(133*14.22),//(133*15),//(133*14),// 2:85
(int)(133*24),//(int)(133*128-1),// 1891,//189216(int)(133*14.22),//(133*15),//(133*14),// 2:85
(int)(160*24),//(int)(160*128-1),// 2730,//273024(int)(192*14.22),//(192*15),//(192*14),// 1:c0
(int)(192*24),//(int)(192*128-1),// 2730,//273024(int)(192*14.22),//(192*15),//(192*14),// 1:c0
(int)(216*24),//(int)(216*128-1),// 3640,//364032(int)(256*14.22),//(256*15),//(256*14),// 0:ff
(int)(256*24),//(int)(256*128-1),// 3640,//364032(int)(256*14.22),//(256*15),//(256*14),// 0:ff
};

/* SSG */

void SSG_write(
//	struct ym2610_info/*Z_AY8910*/ *PSG,
	int r, /*int*/UINT8 v)
{
	int old;
	#if 0
	/*サイズが大きくなる？*/
//	/*PSG->Regs*/YM2610.REGS[r] = v;
	#endif
	/* A note about the period of tones, noise and envelope: for speed reasons,*/
	/* we count down from the period to 0, but careful studies of the chip	   */
	/* output prove that it instead counts up from 0 until the counter becomes */
	/* greater or equal to the period. This is an important difference when the*/
	/* program is rapidly changing the period to modulate the sound.		   */
	/* To compensate for the difference, when the period is changed we adjust  */
	/* our internal counter.												   */
	/* Also, note that period = 0 is the same as period = 1. This is mentioned */
	/* in the YM2203 data sheets. However, this does NOT apply to the Envelope */
	/* period. In that case, period = 0 is half as period = 1. */
/*
  8000000.000 (8M[Hz])
   111111.111 ( 111.1k[Hz]==8M[Hz]/72  @44100)
    44100.0   (  44.1k[Hz])
    55555.555 (  55.5k[Hz]==8M[Hz]/72  @22050)
*/
/*
sample rate  44100.0

clock      8000000.0
clock/72 =  111111.1111
clock/8  = 1000000.0

111111.11111111111111111/44100.0=2.51952632905013857394809775762132
*/
//#define ADJ_PERIOD *=8
#define ADJ_PERIOD *=2.5

#define HACK_PERIOD 0
	switch( r )
	{
	case SSG_AFINE:
	case SSG_ACOARSE:
		/*PSG->Regs*/YM2610.REGS[SSG_ACOARSE] &= 0x0f;
		old = SSG.PeriodA;
		SSG.PeriodA = (/*PSG->Regs*/YM2610.REGS[SSG_AFINE] + 256 * /*PSG->Regs*/YM2610.REGS[SSG_ACOARSE]) * SSG.UpdateStep;
#if (1==HACK_PERIOD)
SSG.PeriodA ADJ_PERIOD;
#endif //(1==HACK_PERIOD)
		if (SSG.PeriodA == 0) SSG.PeriodA = SSG.UpdateStep;
		SSG.CountA += SSG.PeriodA - old;
		if (SSG.CountA <= 0) SSG.CountA = 1;
		break;
	case SSG_BFINE:
	case SSG_BCOARSE:
		/*PSG->Regs*/YM2610.REGS[SSG_BCOARSE] &= 0x0f;
		old = SSG.PeriodB;
		SSG.PeriodB = (/*PSG->Regs*/YM2610.REGS[SSG_BFINE] + 256 * /*PSG->Regs*/YM2610.REGS[SSG_BCOARSE]) * SSG.UpdateStep;
#if (1==HACK_PERIOD)
SSG.PeriodB ADJ_PERIOD;
#endif //(1==HACK_PERIOD)
		if (SSG.PeriodB == 0) SSG.PeriodB = SSG.UpdateStep;
		SSG.CountB += SSG.PeriodB - old;
		if (SSG.CountB <= 0) SSG.CountB = 1;
		break;
	case SSG_CFINE:
	case SSG_CCOARSE:
		/*PSG->Regs*/YM2610.REGS[SSG_CCOARSE] &= 0x0f;
		old = SSG.PeriodC;
		SSG.PeriodC = (/*PSG->Regs*/YM2610.REGS[SSG_CFINE] + 256 * /*PSG->Regs*/YM2610.REGS[SSG_CCOARSE]) * SSG.UpdateStep;
#if (1==HACK_PERIOD)
SSG.PeriodC ADJ_PERIOD;
#endif //(1==HACK_PERIOD)
		if (SSG.PeriodC == 0) SSG.PeriodC = SSG.UpdateStep;
		SSG.CountC += SSG.PeriodC - old;
		if (SSG.CountC <= 0) SSG.CountC = 1;
		break;
	case SSG_NOISEPER:
		/*PSG->Regs*/YM2610.REGS[SSG_NOISEPER] &= 0x1f;
		old = SSG.PeriodN;
		SSG.PeriodN = /*PSG->Regs*/YM2610.REGS[SSG_NOISEPER] * SSG.UpdateStep;
#if (1==HACK_PERIOD)
SSG.PeriodN ADJ_PERIOD;
#endif //(1==HACK_PERIOD)
		if (SSG.PeriodN == 0) SSG.PeriodN = SSG.UpdateStep;
		SSG.CountN += SSG.PeriodN - old;
		if (SSG.CountN <= 0) SSG.CountN = 1;
		break;
//	case SSG_ENABLE:
//		if ((PSG->lastEnable == -1) ||
//			((PSG->lastEnable & 0x40) != (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x40)))
//		{
//			/* write out 0xff if port set to input */
//			if (PSG->PortAwrite)
//				(*PSG->PortAwrite)(0, (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x40) ? /*PSG->Regs*/YM2610.REGS[SSG_PORTA] : 0xff);
//		}

//		if ((PSG->lastEnable == -1) ||
//			((PSG->lastEnable & 0x80) != (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x80)))
//		{
//			/* write out 0xff if port set to input */
//			if (PSG->PortBwrite)
//				(*PSG->PortBwrite)(0, (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x80) ? /*PSG->Regs*/YM2610.REGS[SSG_PORTB] : 0xff);
//		}
//		PSG->lastEnable = /*PSG->Regs*/YM2610.REGS[SSG_ENABLE];
//		break;
	case SSG_AVOL:
		/*PSG->Regs*/YM2610.REGS[SSG_AVOL] &= 0x1f;
		SSG.EnvelopeA = /*PSG->Regs*/YM2610.REGS[SSG_AVOL] & 0x10;
		SSG.VolA = SSG.EnvelopeA ? SSG.VolE : PSG_VolTable[/*PSG->Regs*/YM2610.REGS[SSG_AVOL] ? /*PSG->Regs*/YM2610.REGS[SSG_AVOL]*2+1 : 0];
		break;
	case SSG_BVOL:
		/*PSG->Regs*/YM2610.REGS[SSG_BVOL] &= 0x1f;
		SSG.EnvelopeB = /*PSG->Regs*/YM2610.REGS[SSG_BVOL] & 0x10;
		SSG.VolB = SSG.EnvelopeB ? SSG.VolE : PSG_VolTable[/*PSG->Regs*/YM2610.REGS[SSG_BVOL] ? /*PSG->Regs*/YM2610.REGS[SSG_BVOL]*2+1 : 0];
		break;
	case SSG_CVOL:
		/*PSG->Regs*/YM2610.REGS[SSG_CVOL] &= 0x1f;
		SSG.EnvelopeC = /*PSG->Regs*/YM2610.REGS[SSG_CVOL] & 0x10;
		SSG.VolC = SSG.EnvelopeC ? SSG.VolE : PSG_VolTable[/*PSG->Regs*/YM2610.REGS[SSG_CVOL] ? /*PSG->Regs*/YM2610.REGS[SSG_CVOL]*2+1 : 0];
		break;
	case SSG_EFINE:
	case SSG_ECOARSE:
		old = SSG.PeriodE;
		SSG.PeriodE = ((/*PSG->Regs*/YM2610.REGS[SSG_EFINE] + 256 * /*PSG->Regs*/YM2610.REGS[SSG_ECOARSE])) * SSG.UpdateStep;
#if (1==HACK_PERIOD)
SSG.PeriodE ADJ_PERIOD;
#endif //(1==HACK_PERIOD)
		if (SSG.PeriodE == 0) SSG.PeriodE = SSG.UpdateStep / 2;
		SSG.CountE += SSG.PeriodE - old;
		if (SSG.CountE <= 0) SSG.CountE = 1;
		break;
	case SSG_ESHAPE:
		/* envelope shapes:
		C AtAlH
		0 0 x x  \___

		0 1 x x  /___

		1 0 0 0  \\\\

		1 0 0 1  \___

		1 0 1 0  \/\/
				  ___
		1 0 1 1  \

		1 1 0 0  ////
				  ___
		1 1 0 1  /

		1 1 1 0  /\/\

		1 1 1 1  /___

		The envelope counter on the AY-3-8910 has 16 steps. On the YM2149 it
		has twice the steps, happening twice as fast. Since the end result is
		just a smoother curve, we always use the YM2149 behaviour.
		*/
		/*PSG->Regs*/YM2610.REGS[SSG_ESHAPE] &= 0x0f;
		SSG.Attack = (/*PSG->Regs*/YM2610.REGS[SSG_ESHAPE] & 0x04) ? 0x1f : 0x00;
		if ((/*PSG->Regs*/YM2610.REGS[SSG_ESHAPE] & 0x08) == 0)
		{
			/* if Continue = 0, map the shape to the equivalent one which has Continue = 1 */
			SSG.Hold = 1;
			SSG.Alternate = SSG.Attack;
		}
		else
		{
			SSG.Hold = /*PSG->Regs*/YM2610.REGS[SSG_ESHAPE] & 0x01;
			SSG.Alternate = /*PSG->Regs*/YM2610.REGS[SSG_ESHAPE] & 0x02;
		}
		SSG.CountE = SSG.PeriodE;
		SSG.count_env = 0x1f;
		SSG.Holding = 0;
		SSG.VolE = PSG_VolTable[SSG.count_env ^ SSG.Attack];
		if (SSG.EnvelopeA) SSG.VolA = SSG.VolE;
		if (SSG.EnvelopeB) SSG.VolB = SSG.VolE;
		if (SSG.EnvelopeC) SSG.VolC = SSG.VolE;
		break;
#if 0
	case SSG_PORTA:
//		if (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x40)
//		{
//			if (PSG->PortAwrite)
//				(*PSG->PortAwrite)(0, /*PSG->Regs*/YM2610.REGS[SSG_PORTA]);
//		//	else
//		//		logerror("PC %04x: warning - write %02x to 8910 #%d Port A\n",activecpu_get_pc(),/*PSG->Regs*/YM2610.REGS[SSG_PORTA],PSG->index);
//		}
	//	else
	//	{
	//		logerror("warning: write to 8910 #%d Port A set as input - ignored\n",PSG->index);
	//	}
		break;
	case SSG_PORTB:
//		if (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x80)
//		{
//			if (PSG->PortBwrite)
//				(*PSG->PortBwrite)(0, /*PSG->Regs*/YM2610.REGS[SSG_PORTB]);
//		//	else
//		//		logerror("PC %04x: warning - write %02x to 8910 #%d Port B\n",activecpu_get_pc(),/*PSG->Regs*/YM2610.REGS[SSG_PORTB],PSG->index);
//		}
	//	else
	//	{
	//		logerror("warning: write to 8910 #%d Port B set as input - ignored\n",PSG->index);
	//	}
		break;
#endif
	}
}

static int SSG_CALC(int outn)
{
		int vola,volb,volc;
		int left;
		/* vola, volb and volc keep track of how long each square wave stays */
		/* in the 1 position during the sample period. */
		vola = volb = volc = 0;

		left = SSG_STEP;
		do
		{
			int nextevent;
			if (SSG.CountN < left) nextevent = SSG.CountN;
			else nextevent = left;

			if (outn & 0x08)
			{
				if (SSG.OutputA) vola += SSG.CountA;
				SSG.CountA -= nextevent;
				/* PeriodA is the half period of the square wave. Here, in each */
				/* loop I add PeriodA twice, so that at the end of the loop the */
				/* square wave is in the same status (0 or 1) it was at the start. */
				/* vola is also incremented by PeriodA, since the wave has been 1 */
				/* exactly half of the time, regardless of the initial position. */
				/* If we exit the loop in the middle, OutputA has to be inverted */
				/* and vola incremented only if the exit status of the square */
				/* wave is 1. */
				while (SSG.CountA <= 0)
				{
					SSG.CountA += SSG.PeriodA;
					if (SSG.CountA > 0)
					{
						SSG.OutputA ^= 1;
						if (SSG.OutputA) vola += SSG.PeriodA;
						break;
					}
					SSG.CountA += SSG.PeriodA;
					vola += SSG.PeriodA;
				}
				if (SSG.OutputA) vola -= SSG.CountA;
			}
			else
			{
				SSG.CountA -= nextevent;
				while (SSG.CountA <= 0)
				{
					SSG.CountA += SSG.PeriodA;
					if (SSG.CountA > 0)
					{
						SSG.OutputA ^= 1;
						break;
					}
					SSG.CountA += SSG.PeriodA;
				}
			}
			if (outn & 0x10)
			{
				if (SSG.OutputB) volb += SSG.CountB;
				SSG.CountB -= nextevent;
				while (SSG.CountB <= 0)
				{
					SSG.CountB += SSG.PeriodB;
					if (SSG.CountB > 0)
					{
						SSG.OutputB ^= 1;
						if (SSG.OutputB) volb += SSG.PeriodB;
						break;
					}
					SSG.CountB += SSG.PeriodB;
					volb += SSG.PeriodB;
				}
				if (SSG.OutputB) volb -= SSG.CountB;
			}
			else
			{
				SSG.CountB -= nextevent;
				while (SSG.CountB <= 0)
				{
					SSG.CountB += SSG.PeriodB;
					if (SSG.CountB > 0)
					{
						SSG.OutputB ^= 1;
						break;
					}
					SSG.CountB += SSG.PeriodB;
				}
			}
			if (outn & 0x20)
			{
				if (SSG.OutputC) volc += SSG.CountC;
				SSG.CountC -= nextevent;
				while (SSG.CountC <= 0)
				{
					SSG.CountC += SSG.PeriodC;
					if (SSG.CountC > 0)
					{
						SSG.OutputC ^= 1;
						if (SSG.OutputC) volc += SSG.PeriodC;
						break;
					}
					SSG.CountC += SSG.PeriodC;
					volc += SSG.PeriodC;
				}
				if (SSG.OutputC) volc -= SSG.CountC;
			}
			else
			{
				SSG.CountC -= nextevent;
				while (SSG.CountC <= 0)
				{
					SSG.CountC += SSG.PeriodC;
					if (SSG.CountC > 0)
					{
						SSG.OutputC ^= 1;
						break;
					}
					SSG.CountC += SSG.PeriodC;
				}
			}
			SSG.CountN -= nextevent;
			if (SSG.CountN <= 0)
			{
			static INT32 PSG_RNG=1;
				/* Is noise output going to change? */
				if ((PSG_RNG + 1) & 2) /* (bit0^bit1)? */
				{
					SSG.OutputN = ~SSG.OutputN;
					outn = (SSG.OutputN | /*PSG->Regs*/YM2610.REGS[SSG_ENABLE]);
				}

				/* The Random Number Generator of the 8910 is a 17-bit shift */
				/* register. The input to the shift register is bit0 XOR bit3 */
				/* (bit0 is the output). This was verified on AY-3-8910 and YM2149 chips. */

				/* The following is a fast way to compute bit17 = bit0^bit3. */
				/* Instead of doing all the logic operations, we only check */
				/* bit0, relying on the fact that after three shifts of the */
				/* register, what now is bit3 will become bit0, and will */
				/* invert, if necessary, bit14, which previously was bit17. */
				if (PSG_RNG & 1) PSG_RNG ^= 0x24000; /* This version is called the "Galois configuration". */
				PSG_RNG >>= 1;
				SSG.CountN += SSG.PeriodN;
			}
			left -= nextevent;
		} while (left > 0);

		/* update envelope */
		if (SSG.Holding == 0)
		{
			SSG.CountE -= SSG_STEP;
			if (SSG.CountE <= 0)
			{
				do
				{
					SSG.count_env--;
					SSG.CountE += SSG.PeriodE;
				} while (SSG.CountE <= 0);

				/* check envelope current position */
				if (SSG.count_env < 0)
				{
					if (SSG.Hold)
					{
						if (SSG.Alternate)
						{
							SSG.Attack ^= 0x1f;
						}
						SSG.Holding = 1;
						SSG.count_env = 0;
					}
					else
					{
						/* if CountEnv has looped an odd number of times (usually 1), */
						/* invert the output. */
						if (SSG.Alternate)
						{
							if (SSG.count_env & 0x20)
							{
								SSG.Attack ^= 0x1f;
							}
						}
						SSG.count_env &= 0x1f;
					}
				}
				SSG.VolE = PSG_VolTable[SSG.count_env ^ SSG.Attack];
				/* reload volume */
				if (SSG.EnvelopeA) SSG.VolA = SSG.VolE;
				if (SSG.EnvelopeB) SSG.VolB = SSG.VolE;
				if (SSG.EnvelopeC) SSG.VolC = SSG.VolE;
			}
		}
//		if (PSG->streams == 3)
//		{
//			*(buf1++) = (vola * (int)SSG.VolA) / SSG_STEP;
//			*(buf2++) = (volb * (int)SSG.VolB) / SSG_STEP;
//			*(buf3++) = (volc * (int)SSG.VolC) / SSG_STEP;
//		}
//		else
		{
			/**(buf1++)*/out_ssg =
			(	vola * (int)SSG.VolA +
				volb * (int)SSG.VolB +
				volc * (int)SSG.VolC
			) / SSG_STEP;
		}

	return outn;
}


static int SSG_calc_count(int length)
{
	/* The 8910 has three outputs, each output is the mix of one of the three */
	/* tone generators and of the (single) noise generator. The two are mixed */
	/* BEFORE going into the DAC. The formula to mix each channel is: */
	/* (ToneOn | ToneDisable) & (NoiseOn | NoiseDisable). */
	/* Note that this means that if both tone and noise are disabled, the output */
	/* is 1, not 0, and can be modulated changing the volume. */

	/* If the channels are disabled, set their output to 1, and increase the */
	/* counter, if necessary, so they will not be inverted during this update. */
	/* Setting the output to 1 is necessary because a disabled channel is locked */
	/* into the ON state (see above); and it has no effect if the volume is 0. */
	/* If the volume is 0, increase the counter, but don't touch the output. */
	if (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x01)
	{
		if (SSG.CountA <= length*SSG_STEP) SSG.CountA += length*SSG_STEP;
		SSG.OutputA = 1;
	}
	else if (/*PSG->Regs*/YM2610.REGS[SSG_AVOL] == 0)
	{
		/* note that I do count += length, NOT count = length + 1. You might think */
		/* it's the same since the volume is 0, but doing the latter could cause */
		/* interferencies when the program is rapidly modulating the volume. */
		if (SSG.CountA <= length*SSG_STEP) SSG.CountA += length*SSG_STEP;
	}
//
	if (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x02)
	{
		if (SSG.CountB <= length*SSG_STEP) SSG.CountB += length*SSG_STEP;
		SSG.OutputB = 1;
	}
	else if (/*PSG->Regs*/YM2610.REGS[SSG_BVOL] == 0)
	{
		if (SSG.CountB <= length*SSG_STEP) SSG.CountB += length*SSG_STEP;
	}
//
	if (/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x04)
	{
		if (SSG.CountC <= length*SSG_STEP) SSG.CountC += length*SSG_STEP;
		SSG.OutputC = 1;
	}
	else if (/*PSG->Regs*/YM2610.REGS[SSG_CVOL] == 0)
	{
		if (SSG.CountC <= length*SSG_STEP) SSG.CountC += length*SSG_STEP;
	}
//
	/* for the noise channel we must not touch OutputN - it's also not necessary */
	/* since we use outn. */
	if ((/*PSG->Regs*/YM2610.REGS[SSG_ENABLE] & 0x38) == 0x38) /* all off */
	{
		if (SSG.CountN <= length*SSG_STEP) SSG.CountN += length*SSG_STEP;
	}
	/*outn =*/return (SSG.OutputN | /*PSG->Regs*/YM2610.REGS[SSG_ENABLE]);

}

#if 0
static void Z_AY8910_Update(void *param,stream_sample_t **inputs, stream_sample_t **buffer,int length)
{
#if 0
	struct ym2610_info/*Z_AY8910*/ *PSG = param;
	stream_sample_t *buf1;
//	stream_sample_t *buf2;
//	stream_sample_t *buf3;
	int outn;

	buf1 = buffer[0];
//	buf2 = NULL;
//	buf3 = NULL;
//	if (PSG->streams == 3)
//	{
//		buf2 = buffer[1];
//		buf3 = buffer[2];
//	}

	/* hack to prevent us from hanging when starting filtered outputs */
	if (!PSG->ready)
	{
					memset(buf1, 0, length * sizeof(*buf1));
//		if (buf2)	memset(buf2, 0, length * sizeof(*buf2));
//		if (buf3)	memset(buf3, 0, length * sizeof(*buf3));
		return;
	}

	/* calc SSG count */
	outn = SSG_calc_count(length);

	/* buffering loop */
	while (length)
	{
		/* clear outputs SSG */
//		out_ssg = 0;

		outn = SSG_CALC(outn);
		*(buf1++)=out_ssg;
		length--;
	}
#endif
}
#endif


#if 0
/* 要らないらしい(?) */
void Z_AY8910_set_volume(u8 chip,int channel,int volume)
{
	struct ym2610_info/*Z_AY8910*/ *PSG = sndti_token(SOUND_AY8910, chip);
	int ch;

	for (ch = 0; ch < PSG->streams; ch++)
		if (channel == ch || PSG->streams == 1 || channel == ALL_8910_CHANNELS)
			stream_set_output_gain(PSG->Channel, ch, volume / 100.0);
}
#endif













/* Forecast to next Forecast (rate = *8) */
/* 1/8 , 3/8 , 5/8 , 7/8 , 9/8 , 11/8 , 13/8 , 15/8 */
const INT32 ym_deltat_decode_tableB1[16] = {
  1,   3,	5,	 7,   9,  11,  13,	15,
  -1,  -3,	-5,  -7,  -9, -11, -13, -15,
};
/* delta to next delta (rate= *64) */
/* 0.9 , 0.9 , 0.9 , 0.9 , 1.2 , 1.6 , 2.0 , 2.4 */
const INT32 ym_deltat_decode_tableB2[16] = {
  57,  57,	57,  57, 77, 102, 128, 153,
  57,  57,	57,  57, 77, 102, 128, 153
};


/* DELTA-T ADPCM write register */
static void OPNB_ADPCMB_write(ADPCMB *adpcmb,int addr,/*int*/UINT8 data/*v*/)
{
//	if(addr>=(0x10+0x10)) return;
//	/*adpcmb->reg*/YM2610.REGS[(0x10|addr)] = v; /* stock data */
//	YM2610.REGS[addr] = v; /* stock data */
	switch( addr )
	{
	case (0x10|0x00):
/*
START:
	Accessing *external* memory is started when START bit (D7) is set to "1", so
	you must set all conditions needed for recording/playback before starting.
	If you access *CPU-managed* memory, recording/playback starts after
	read/write of ADPCM data register $08.

REC:
	0 = ADPCM synthesis (playback)
	1 = ADPCM analysis (record)

MEMDATA:
	0 = processor (*CPU-managed*) memory (means: using register $08)
	1 = external memory (using start/end/limit registers to access memory: RAM or ROM)


SPOFF:
	controls output pin that should disable the speaker while ADPCM analysis

RESET and REPEAT only work with external memory.


some examples:
value:	 START, REC, MEMDAT, REPEAT, SPOFF, x,x,RESET	meaning:
  C8	 1		1	 0		 0		 1		0 0 0		Analysis (recording) from AUDIO to CPU (to reg $08), sample rate in PRESCALER register
  E8	 1		1	 1		 0		 1		0 0 0		Analysis (recording) from AUDIO to EXT.MEMORY,		 sample rate in PRESCALER register
  80	 1		0	 0		 0		 0		0 0 0		Synthesis (playing) from CPU (from reg $08) to AUDIO,sample rate in DELTA-N register
  a0	 1		0	 1		 0		 0		0 0 0		Synthesis (playing) from EXT.MEMORY to AUDIO,		 sample rate in DELTA-N register

  60	 0		1	 1		 0		 0		0 0 0		External memory write via ADPCM data register $08
  20	 0		0	 1		 0		 0		0 0 0		External memory read  via ADPCM data register $08
*/
		/* handle emulation mode */
		//if(adpcmb->emulation_mode == ADPCMB_EMULATION_MODE_YM2610)
		{
			data |= 0x20;		/*	YM2610 always uses external memory and doesn't even have memory flag bit. */
		}

		adpcmb->portstate = data & (0x80|0x40|0x20|0x10|0x01); /* start, rec, memory mode, repeat flag copy,    reset(bit0) */

		if( adpcmb->portstate&0x80 )/* START,REC,MEMDATA,REPEAT, SPOFF,--,--,RESET */
		{
			/* set PCM BUSY bit */
			adpcmb->PCM_BSY = 1;

			/* start ADPCM */
			adpcmb->now_step = 0;
			adpcmb->acc 	 = 0;
			adpcmb->prev_acc = 0;
			adpcmb->adpcml	 = 0;
			adpcmb->adpcmd	 = ADPCMB_DELTA_DEF;
			adpcmb->now_data = 0;

		}

		/* external memory read */
		//if( adpcmb->portstate&0x20 ) /* do we access external memory? */
		{
			adpcmb->now_addr = adpcmb->start << 1;
//			adpcmb->memread = 2;	/* two dummy reads needed before accesing external memory via register $08*/

			/* if yes, then let's check if ADPCM memory is mapped and big enough */
			if(adpcmb->pcmbufB/*memory*/ == 0)
			{
				//logerror("YM Delta-T ADPCM rom not mapped\n");
				adpcmb->portstate = 0x00;
				adpcmb->PCM_BSY = 0;
			}
			else
			{
				if( adpcmb->end >= adpcmb->pcmsizeB/*memory_size*/ )	/* Check End in Range */
				{
					//logerror("YM Delta-T ADPCM end out of range: $%08x\n", adpcmb->end);
					adpcmb->end = adpcmb->pcmsizeB/*memory_size*/ - 1;
				}
				if( adpcmb->start >= adpcmb->pcmsizeB/*memory_size*/ )	/* Check Start in Range */
				{
					//logerror("YM Delta-T ADPCM start out of range: $%08x\n", adpcmb->start);
					adpcmb->portstate = 0x00;
					adpcmb->PCM_BSY = 0;
				}
			}
		}
		//else	/* we access CPU memory (ADPCM data register $08) so we only reset now_addr here */
		//{
		//	adpcmb->now_addr = 0;
		//}

		if( adpcmb->portstate&0x01 )
		{
			adpcmb->portstate = 0x00;

			/* clear PCM BUSY bit (in status register) */
			adpcmb->PCM_BSY = 0;

			/* set BRDY flag */
		//	if(adpcmb->status_set_handler)
		//		if(adpcmb->status_change_BRDY_bit)
		//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//			if(adpcmb->status_change_BRDY_bit)
//			{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_BRDY_bit;}
		}
		break;
	case (0x10|0x01):	/* L,R,-,-,SAMPLE,DA/AD,RAMTYPE,ROM */
		/* handle emulation mode */
	//	//if(adpcmb->emulation_mode == ADPCMB_EMULATION_MODE_YM2610)
	//	{
	//		data |= 0x01;		/*	YM2610 always uses ROM as an external memory and doesn't have ROM/RAM memory flag bit. */
	//	}

		adpcmb->pan = &adpcmb->output_pointer[(data>>6)&0x03];
//		#if 0 /*not have 2610/2610B*/
//		if ((adpcmb->control2 & 3) != (data & 3))
//		{
//			/* 0-DRAM x1, 1-ROM, 2-DRAM x8, 3-ROM (3 is bad setting - not allowed by the manual) */
//			static const UINT8 dram_rightshift[4]={3,0,0,0};
//			/*0-DRAM x1, 1-ROM, 2-DRAM x8, 3-ROM (3 is bad setting - not allowed by the manual) */
//			if (/*adpcmb->DRAMportshift*/0 != dram_rightshift[data&3])
//			{
//				/*adpcmb->DRAMportshift*/0 = dram_rightshift[data&3];
//				/* final shift value depends on chip type and memory type selected:
//						8 for YM2610 (ROM only),
//						5 for ROM for Y8950 and YM2608,
//						5 for x8bit DRAMs for Y8950 and YM2608,
//						2 for x1bit DRAMs for Y8950 and YM2608.
//				*/
//				/* refresh addresses */
//				adpcmb->start  = (/*adpcmb->reg*/YM2610.REGS[(0x10|0x3)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0x2)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
//				adpcmb->end    = (/*adpcmb->reg*/YM2610.REGS[(0x10|0x5)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0x4)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
//				adpcmb->end   += (1 << (/*adpcmb->portshift*/8-/*adpcmb->DRAMportshift*/0) ) - 1;
//				adpcmb->limit  = (/*adpcmb->reg*/YM2610.REGS[(0x10|0xd)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0xc)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
//			}
//		}
//		#endif
	//	adpcmb->control2 = data;
		break;
	case (0x10|0x02):	/* Start Address L */
	case (0x10|0x03):	/* Start Address H */
		#if 0
//		adpcmb->start  = (/*adpcmb->reg*/YM2610.REGS[(0x10|0x3)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0x2)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
//		/*logerror("DELTAT start: 02=%2x 03=%2x addr=%8x\n",/*adpcmb->reg*/YM2610.REGS[(0x10|0x2)], /*adpcmb->reg*/YM2610.REGS[(0x10|0x3)],adpcmb->start );*/
		#else
		adpcmb->start = (YM2610.REGS[(0x10|0x3)]<<16) |
						(YM2610.REGS[(0x10|0x2)]<< 8);
		#endif

		break;
	case (0x10|0x04):	/* Stop Address L */
	case (0x10|0x05):	/* Stop Address H */
		#if 0
		adpcmb->end    = (/*adpcmb->reg*/YM2610.REGS[(0x10|0x5)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0x4)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
		adpcmb->end   += (1 << (/*adpcmb->portshift*/8-/*adpcmb->DRAMportshift*/0) ) - 1;
//		/*logerror("DELTAT end  : 04=%2x 05=%2x addr=%8x\n",/*adpcmb->reg*/YM2610.REGS[(0x10|0x4)], /*adpcmb->reg*/YM2610.REGS[(0x10|0x5)],adpcmb->end	 );*/
		#else
		adpcmb->end   = (YM2610.REGS[(0x10|0x5)]<<16) |
						(YM2610.REGS[(0x10|0x4)]<< 8);
		adpcmb->end  += (0xff);
		#endif
		break;

//#if 0
//	case (0x10|0x06):	/* Prescale L (ADPCM and Record frq) */
//	case (0x10|0x07):	/* Prescale H */
//		break;
//	case (0x10|0x08):	/* ADPCM data */
/*
some examples:
value:	 START, REC, MEMDAT, REPEAT, SPOFF, x,x,RESET	meaning:
  C8	 1		1	 0		 0		 1		0 0 0		Analysis (recording) from AUDIO to CPU (to reg $08), sample rate in PRESCALER register
  E8	 1		1	 1		 0		 1		0 0 0		Analysis (recording) from AUDIO to EXT.MEMORY,		 sample rate in PRESCALER register
  80	 1		0	 0		 0		 0		0 0 0		Synthesis (playing) from CPU (from reg $08) to AUDIO,sample rate in DELTA-N register
  a0	 1		0	 1		 0		 0		0 0 0		Synthesis (playing) from EXT.MEMORY to AUDIO,		 sample rate in DELTA-N register
//
  60	 0		1	 1		 0		 0		0 0 0		External memory write via ADPCM data register $08
  20	 0		0	 1		 0		 0		0 0 0		External memory read via ADPCM data register $08
*/
//		/* external memory write */
//		if ( (adpcmb->portstate & 0xe0)==0x60 )
//		{
//			if (adpcmb->memread)
//			{
//				adpcmb->now_addr = adpcmb->start << 1;
//				adpcmb->memread = 0;
//			}
//			/*logerror("YM Delta-T memory write $%08x, v=$%02x\n", adpcmb->now_addr >> 1, v);*/
//			if ( adpcmb->now_addr != (adpcmb->end<<1) )
//			{
//				adpcmb->pcmbufB/*memory*/[adpcmb->now_addr>>1] = v;
//				adpcmb->now_addr+=2; /* two nibbles at a time */
//				/* reset BRDY bit in status register, which means we are processing the write */
//			//	if(adpcmb->status_reset_handler)
//			//		if(adpcmb->status_change_BRDY_bit)
//			//			(adpcmb->status_reset_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//				if(adpcmb->status_change_BRDY_bit)
//				{	/*F2610->*/YM2610.adpcm_arrivedEndAddress &= (~(adpcmb->status_change_BRDY_bit));}
//	/* setup a timer that will callback us in 10 master clock cycles for Y8950
//	* in the callback set the BRDY flag to 1 , which means we have written the data.
//	* For now, we don't really do this; we simply reset and set the flag in zero time, so that the IRQ will work.
//	*/
//				/* set BRDY bit in status register */
//			//	if(adpcmb->status_set_handler)
//			//		if(adpcmb->status_change_BRDY_bit)
//			//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//				if(adpcmb->status_change_BRDY_bit)
//				{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_BRDY_bit;}
//			}
//			else
//			{
//				/* set EOS bit in status register */
//			//	if(adpcmb->status_set_handler)
//			//		if(adpcmb->status_change_EOS_bit)
//			//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_EOS_bit);
//				if(adpcmb->status_change_EOS_bit)
//				{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_EOS_bit;}
//			}
//			return;
//		}
//		/* ADPCM synthesis from CPU */
//		if ( (adpcmb->portstate & 0xe0)==0x80 )
//		{
//			adpcmb->CPU_data = data;
//			/* Reset BRDY bit in status register, which means we are full of data */
//		//	if(adpcmb->status_reset_handler)
//		//		if(adpcmb->status_change_BRDY_bit)
//		//			(adpcmb->status_reset_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//			if(adpcmb->status_change_BRDY_bit)
//			{	/*F2610->*/YM2610.adpcm_arrivedEndAddress &= (~(adpcmb->status_change_BRDY_bit));}
//			return;
//		}
//		break;
//#endif
	case (0x10|0x09):	/* DELTA-N L (ADPCM Playback Prescaler) */
	case (0x10|0x0a):	/* DELTA-N H */
		adpcmb->delta  = (/*adpcmb->reg*/YM2610.REGS[(0x10|0xa)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0x9)]);
		adpcmb->step	 = (UINT32)( (double)(adpcmb->delta /* *(1<<(ADPCMB_SHIFT-16)) */ ) * (adpcmb->freqbase) );
//		/*logerror("DELTAT deltan:09=%2x 0a=%2x\n",/*adpcmb->reg*/YM2610.REGS[(0x10|0x9)], /*adpcmb->reg*/YM2610.REGS[(0x10|0xa)]);*/
		break;
	case (0x10|0x0b):	/* Output level control (volume, linear) */
		{
			INT32 oldvol = adpcmb->volume;
			adpcmb->volume = (data&0xff) * (adpcmb->output_range/256) / ADPCMB_DECODE_RANGE;
/*								data	  * 	((1<<16)>>8)		>>	15;
*						thus:	data	  * 	(1<<8)				>>	15;
*						thus: output_range must be (1 << (15+8)) at least
*								data	  * 	((1<<23)>>8)		>>	15;
*								data	  * 	(1<<15) 			>>	15;
*/
			/*logerror("DELTAT vol = %2x\n",data&0xff);*/
			if( /* 0! = */ oldvol )
			{
				adpcmb->adpcml = (int)((double)adpcmb->adpcml / (double)oldvol * (double)adpcmb->volume);
			}
		}
		break;
#if 1
	case 0x1c: /*  FLAG CONTROL : Extend Status Clear/Mask */
		{
			UINT8 statusmask = ~data/*v*/;
			/* set arrived flag mask */
			int ch;
			for(ch=0; ch<6; ch++)
			{	/*F2610->*/YM2610.adpcm[ch].flagMask = statusmask&(1<<ch);}
			/*F2610->*//*YM2610.deltaT.*/adpcmb->status_change_EOS_bit = statusmask & 0x80; /* status flag: set bit7 on End Of Sample */
			/* clear arrived flag */
			/*F2610->*/YM2610.adpcm_arrivedEndAddress &= statusmask;
		}
		break;
#else
/*not have 2610*/
//	case (0x10|0x0c):	/* Limit Address L */
//	case (0x10|0x0d):	/* Limit Address H */
//		adpcmb->limit  = (/*adpcmb->reg*/YM2610.REGS[(0x10|0xd)]*0x0100 | /*adpcmb->reg*/YM2610.REGS[(0x10|0xc)]) << (/*adpcmb->portshift*/8 - /*adpcmb->DRAMportshift*/0);
//		/*logerror("DELTAT limit: 0c=%2x 0d=%2x addr=%8x\n",/*adpcmb->reg*/YM2610.REGS[(0x10|0xc)], /*adpcmb->reg*/YM2610.REGS[(0x10|0xd)],adpcmb->limit );*/
//		break;
#endif
	//	default:
	//		//logerror("YM2610: write to unknown deltat register %02x val=%02x\n",addr,v);
	//		break;
	}
}


//static void ADPCMB_postload(ADPCMB *adpcmb,UINT8 *regs)
//{
//	int r;
//
//	/* to keep adpcml */
//	adpcmb->volume = 0;
//	/* update */
//	for(r=(1+0x10);r<(16+0x10);r++)
//	{	OPNB_ADPCMB_write(DELTAT,r,regs[r]);}
//	/*adpcmb->reg*/YM2610.REGS[(0x10|0)] = regs[0];
//
//	/* current rom data */
//	if (adpcmb->memory)
//		adpcmb->now_data = *(adpcmb->memory + (adpcmb->now_addr>>1) );
//}
//#if 0
//static void ADPCMB_savestate(const char *statename,int num,ADPCMB *adpcmb)
//{
//#ifdef _STATE_H
//	state_save_register_UINT8 (statename, num, "DeltaT.portstate", &adpcmb->portstate, 1);
//	state_save_register_UINT32(statename, num, "DeltaT.address"  , &adpcmb->now_addr , 1);
//	state_save_register_UINT32(statename, num, "DeltaT.step"	 , &adpcmb->now_step , 1);
//	state_save_register_INT32 (statename, num, "DeltaT.acc" 	 , &adpcmb->acc 	 , 1);
//	state_save_register_INT32 (statename, num, "DeltaT.prev_acc" , &adpcmb->prev_acc , 1);
//	state_save_register_INT32 (statename, num, "DeltaT.adpcmd"	 , &adpcmb->adpcmd	 , 1);
//	state_save_register_INT32 (statename, num, "DeltaT.adpcml"	 , &adpcmb->adpcml	 , 1);
//#endif
//}
//#endif

#define ADPCMB_Limit(val,max,min)	\
{										\
	if ( val > max ) val = max; 		\
	else if ( val < min ) val = min;	\
}

INLINE void ADPCMB_synthesis_from_external_memory(ADPCMB *adpcmb)
{
	UINT32 step;
	int data;

	adpcmb->now_step += adpcmb->step;
	if ( adpcmb->now_step >= (1<<ADPCMB_SHIFT) )
	{
		step = adpcmb->now_step >> ADPCMB_SHIFT;
		adpcmb->now_step &= (1<<ADPCMB_SHIFT)-1;
		do{

			if ( adpcmb->now_addr == (/*adpcmb->limit*/(~0)<<1) )
				adpcmb->now_addr = 0;

			if ( adpcmb->now_addr == (adpcmb->end<<1) ) {	/* 12-06-2001 JB: corrected comparison. Was > instead of == */
				if( adpcmb->portstate&0x10 ){
					/* repeat start */
					adpcmb->now_addr = adpcmb->start<<1;
					adpcmb->acc 	 = 0;
					adpcmb->adpcmd	 = ADPCMB_DELTA_DEF;
					adpcmb->prev_acc = 0;
				}else{
					/* set EOS bit in status register */
				//	if(adpcmb->status_set_handler)
				//		if(adpcmb->status_change_EOS_bit)
				//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_EOS_bit);
					if(adpcmb->status_change_EOS_bit)
					{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_EOS_bit;}

					/* clear PCM BUSY bit (reflected in status register) */
					adpcmb->PCM_BSY = 0;

					adpcmb->portstate = 0;
					adpcmb->adpcml = 0;
					adpcmb->prev_acc = 0;
					return;
				}
			}

			if( adpcmb->now_addr&1 ) data = adpcmb->now_data & 0x0f;
			else
			{
				adpcmb->now_data = *(adpcmb->pcmbufB/*memory*/ + (adpcmb->now_addr>>1));
				data = adpcmb->now_data >> 4;
			}

			adpcmb->now_addr++;
			/* 12-06-2001 JB: */
			/* YM2610 address register is 24 bits wide.*/
			/* The "+1" is there because we use 1 bit more for nibble calculations.*/
			/* WARNING: */
			/* Side effect: we should take the size of the mapped ROM into account */
			adpcmb->now_addr &= ( (1<<(24+1))-1);

			/* store accumulator value */
			adpcmb->prev_acc = adpcmb->acc;

			/* Forecast to next Forecast */
			adpcmb->acc += (ym_deltat_decode_tableB1[data] * adpcmb->adpcmd / 8);
			ADPCMB_Limit(adpcmb->acc,ADPCMB_DECODE_MAX, ADPCMB_DECODE_MIN);

			/* delta to next delta */
			adpcmb->adpcmd = (adpcmb->adpcmd * ym_deltat_decode_tableB2[data] ) / 64;
			ADPCMB_Limit(adpcmb->adpcmd,ADPCMB_DELTA_MAX, ADPCMB_DELTA_MIN );

			/* ElSemi: Fix interpolator. */
			/*adpcmb->prev_acc = prev_acc + ((adpcmb->acc - prev_acc) / 2 );*/

		}while(--step);

	}

	/* ElSemi: Fix interpolator. */
	adpcmb->adpcml = adpcmb->prev_acc * (int)((1<<ADPCMB_SHIFT)-adpcmb->now_step);
	adpcmb->adpcml += (adpcmb->acc * (int)adpcmb->now_step);
	adpcmb->adpcml = (adpcmb->adpcml>>ADPCMB_SHIFT) * (int)adpcmb->volume;

	/* output for work of output channels (outd[OPNxxxx])*/
	*(adpcmb->pan) += adpcmb->adpcml;
}



INLINE void ADPCMB_synthesis_from_CPU_memory(ADPCMB *adpcmb)
{
	UINT32 step;
	int data;

	adpcmb->now_step += adpcmb->step;
	if ( adpcmb->now_step >= (1<<ADPCMB_SHIFT) )
	{
		step = adpcmb->now_step >> ADPCMB_SHIFT;
		adpcmb->now_step &= (1<<ADPCMB_SHIFT)-1;
		do{
			if( adpcmb->now_addr&1 )
			{
				data = adpcmb->now_data & 0x0f;
				adpcmb->now_data = adpcmb->CPU_data;
				/* after we used CPU_data, we set BRDY bit in status register,
				* which means we are ready to accept another byte of data */
			//	if(adpcmb->status_set_handler)
			//		if(adpcmb->status_change_BRDY_bit)
			//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//				if(adpcmb->status_change_BRDY_bit)
//				{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_BRDY_bit;}
			}
			else
			{
				data = adpcmb->now_data >> 4;
			}
			adpcmb->now_addr++;

			/* store accumulator value */
			adpcmb->prev_acc = adpcmb->acc;

			/* Forecast to next Forecast */
			adpcmb->acc += (ym_deltat_decode_tableB1[data] * adpcmb->adpcmd / 8);
			ADPCMB_Limit(adpcmb->acc,ADPCMB_DECODE_MAX, ADPCMB_DECODE_MIN);

			/* delta to next delta */
			adpcmb->adpcmd = (adpcmb->adpcmd * ym_deltat_decode_tableB2[data] ) / 64;
			ADPCMB_Limit(adpcmb->adpcmd,ADPCMB_DELTA_MAX, ADPCMB_DELTA_MIN );
		}while(--step);
	}

	/* ElSemi: Fix interpolator. */
	adpcmb->adpcml = adpcmb->prev_acc * (int)((1<<ADPCMB_SHIFT)-adpcmb->now_step);
	adpcmb->adpcml += (adpcmb->acc * (int)adpcmb->now_step);
	adpcmb->adpcml = (adpcmb->adpcml>>ADPCMB_SHIFT) * (int)adpcmb->volume;

	/* output for work of output channels (outd[OPNxxxx])*/
	*(adpcmb->pan) += adpcmb->adpcml;
}



/* ADPCM B (Delta-T control type) */
static void OPNB_ADPCMB_calc(ADPCMB *adpcmb)
{
/*
some examples:
value:	 START, REC, MEMDAT, REPEAT, SPOFF, x,x,RESET	meaning:
  80	 1		0	 0		 0		 0		0 0 0		Synthesis (playing) from CPU (from reg $08) to AUDIO,sample rate in DELTA-N register
  a0	 1		0	 1		 0		 0		0 0 0		Synthesis (playing) from EXT.MEMORY to AUDIO,		 sample rate in DELTA-N register
  c8	 1		1	 0		 0		 1		0 0 0		Analysis (recording) from AUDIO to CPU (to reg $08), sample rate in PRESCALER register
  e8	 1		1	 1		 0		 1		0 0 0		Analysis (recording) from AUDIO to EXT.MEMORY,		 sample rate in PRESCALER register

  60	 0		1	 1		 0		 0		0 0 0		External memory write via ADPCM data register $08
  20	 0		0	 1		 0		 0		0 0 0		External memory read  via ADPCM data register $08
*/
	switch((adpcmb->portstate & 0xe0))
	{
	case 0x80:
		/* ADPCM synthesis from CPU-managed memory (from reg $08) */
		ADPCMB_synthesis_from_CPU_memory(adpcmb/*DELTAT*/);	/* change output based on data in ADPCM data reg ($08) */
		return;
	case 0xa0:
		ADPCMB_synthesis_from_external_memory(adpcmb/*DELTAT*/);
		return;
	//todo: ADPCM analysis
//	default:
//	case 0xc0: //todo: ADPCM analysis
//	case 0xe0: //todo: ADPCM analysis
//		return;
	}
	return;
}

//////////////////








/* limitter */
#define myLimit(val, max,min) { \
	if ( val > max )	  val = max; \
	else if ( val < min ) val = min; \
}


/* status set and IRQ handling */
INLINE void FM_STATUS_SET(FM_ST *ST,int flag)
{
	/* set status flag */
	//FM_ST *ST = &YM2610.OPN.ST;
	ST->status |= flag;
	if ( !(ST->irq) )
	{
		if ( (ST->status & ST->irqmask) )
		{
			ST->irq = 1;
			/* callback user interrupt handler (IRQ is OFF to ON) */
			/*if(ST->IRQ_Handler)*/ (ST->IRQ_Handler)(/*ST->param,*/1);
		}
	}
}

/* status reset and IRQ handling */
INLINE void FM_STATUS_RESET(FM_ST *ST,int flag)
{
	/* reset status flag */
	//FM_ST *ST = &YM2610.OPN.ST;
	ST->status &=~flag;
	if ( (ST->irq) )
	{
		if ( !(ST->status & ST->irqmask) )
		{
			ST->irq = 0;
			/* callback user interrupt handler (IRQ is ON to OFF) */
			/*if(ST->IRQ_Handler)*/ (ST->IRQ_Handler)(/*ST->param,*/0);
		}
	}
}

/* IRQ mask set */
INLINE void FM_IRQMASK_SET(FM_ST *ST,int flag)
{
	ST->irqmask = flag;
	/* IRQ handling check */
	//FM_ST *ST = &YM2610.OPN.ST;
	FM_STATUS_SET(ST,0);
	FM_STATUS_RESET(ST,0);
}

/* OPN Mode Register Write */
INLINE void set_timers( FM_ST *ST, /*void *n,*/ int v )
{
	/* b7 = CSM MODE */
	/* b6 = 3 slot mode */
	/* b5 = reset b */
	/* b4 = reset a */
//
	/* b3 = timer enable b */
	/* b2 = timer enable a */
	/* b1 = load b */
	/* b0 = load a */
	ST->mode = v;

	/* reset Timer b flag */
	if( v & 0x20 )	{	FM_STATUS_RESET(ST,0x02);}
	/* reset Timer a flag */
	if( v & 0x10 )	{	FM_STATUS_RESET(ST,0x01);}
//	/* reset Timer a&b flag */
//	if( v & 0x30 )	{	FM_STATUS_RESET(ST,((v&30)>>4)/*0x01*/);}

	/* load b */
	if( v & 0x02 )
	{
		if( ST->TBC == 0 )
		{
			ST->TBC = ( 256-ST->TB)<<4;
			/* External timer handler */
			/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*n,*/1,ST->TBC,ST->TimerBase);
		}
	}
	else
	{	/* stop timer b */
		if( ST->TBC != 0 )
		{
			ST->TBC = 0;
			/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*n,*/1,0,ST->TimerBase);
		}
	}
	/* load a */
	if( v & 0x01 )
	{
		if( ST->TAC == 0 )
		{
			ST->TAC = (1024-ST->TA);
			/* External timer handler */
			/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*n,*/0,ST->TAC,ST->TimerBase);
		}
	}
	else
	{	/* stop timer a */
		if( ST->TAC != 0 )
		{
			ST->TAC = 0;
			/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*n,*/0,0,ST->TimerBase);
		}
	}
}


/* Timer A Overflow */
INLINE void TimerAOver(FM_ST *ST)
{
	/* set status (if enabled) */
	if(ST->mode & 0x04) FM_STATUS_SET(ST,0x01);
	/* clear or reload the counter */
	ST->TAC = (1024-ST->TA);
	/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*ST->param,*/0,ST->TAC,ST->TimerBase);
}
/* Timer B Overflow */
INLINE void TimerBOver(FM_ST *ST)
{
	/* set status (if enabled) */
	if(ST->mode & 0x08) FM_STATUS_SET(ST,0x02);
	/* clear or reload the counter */
	ST->TBC = ( 256-ST->TB)<<4;
	/*if (ST->Timer_Handler)*/ (ST->Timer_Handler)(/*ST->param,*/1,ST->TBC,ST->TimerBase);
}


#if FM_INTERNAL_TIMER
/* ----- internal timer mode , update timer */
ERROR (not used)
/* ---------- calculate timer A ---------- */
	#define INTERNAL_TIMER_A(/*ST,*/CSM_CH) 			\
	if( (/*ST->*/OPN->ST.Timer_Handler==0) )			\
	{													\
		if( /*ST->*/OPN->ST.TAC )						\
		{												\
			if( (/*ST->*/OPN->ST.TAC -= (int)(/*ST->*/OPN->ST.freqbase*4096)) <= 0 )	\
			{														\
				TimerAOver(( &(/*F2610->*/YM2610.OPN.ST) )/*ST*/);	\
				/* CSM mode total level latch and auto key on */	\
				if( /*ST->*/OPN->ST.mode & 0x80 )					\
				{													\
					CSMKeyControll( CSM_CH );						\
				}													\
			}														\
		}															\
	}
/* ---------- calculate timer B ---------- */
	#define INTERNAL_TIMER_B(/*ST,*/step)		\
	if( (/*ST->*/OPN->ST.Timer_Handler==0) )	\
	{											\
		if( /*ST->*/OPN->ST.TBC )				\
		{	if( (/*ST->*/OPN->ST.TBC -= (int)(/*ST->*/OPN->ST.freqbase*4096*step)) <= 0 )	\
			{						\
				TimerBOver( ST );	\
			}						\
		}							\
	}
#else /* FM_INTERNAL_TIMER */
/* external timer mode */
#define INTERNAL_TIMER_A(/*ST,*/CSM_CH)
#define INTERNAL_TIMER_B(/*ST,*/step)
#endif /* FM_INTERNAL_TIMER */



#if FM_BUSY_FLAG_SUPPORT
INLINE UINT8 FM_STATUS_FLAG(FM_ST *ST)
{
	if( ST->BusyExpire )
	{
		if( (ST->BusyExpire - FM_GET_TIME_NOW()) > 0)
			return ST->status | 0x80;	/* with busy */
		/* expire */
		ST->BusyExpire = 0;
	}
	return ST->status;
}
INLINE void FM_BUSY_SET(FM_ST *ST,int busyclock )
{
	ST->BusyExpire = FM_GET_TIME_NOW() + (ST->TimerBase * busyclock);
}
#define FM_BUSY_CLEAR(ST) ((ST)->BusyExpire = 0)
#else
#define FM_STATUS_FLAG(ST) ((ST)->status)
#define FM_BUSY_SET(ST,bclock) {}
#define FM_BUSY_CLEAR(ST) {}
#endif




INLINE void FM_KEYON( FM_CH *CH, int s )
{
	FM_SLOT *SLOT = &CH->SLOT[s];
	if( !SLOT->key )
	{
		SLOT->key = 1;
		SLOT->phase = 0;		/* restart Phase Generator */
		SLOT->state = EG_ATT;	/* phase -> Attack */
	}
}

INLINE void FM_KEYOFF( FM_CH *CH, int s )
{
	FM_SLOT *SLOT = &CH->SLOT[s];
	if( SLOT->key )
	{
		SLOT->key = 0;
		if (SLOT->state>EG_REL)
			SLOT->state = EG_REL;/* phase -> Release */
	}
}

/* set algorithm connection */
static void setup_connection( FM_CH *CH, int ch )
{
	INT32 *carrier = &out_fm[ch];

	INT32 **om1 = &CH->connect1;
	INT32 **om2 = &CH->connect3;
	INT32 **oc1 = &CH->connect2;

	INT32 **memc = &CH->mem_connect;

	switch( CH->ALGO ){
	case 0:
		/* M1---C1---MEM---M2---C2---OUT */
		*om1 = &c1;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 1:
		/* M1------+-MEM---M2---C2---OUT */
		/*		C1-+					 */
		*om1 = &mem;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 2:
		/* M1-----------------+-C2---OUT */
		/*		C1---MEM---M2-+ 		 */
		*om1 = &c2;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &m2;
		break;
	case 3:
		/* M1---C1---MEM------+-C2---OUT */
		/*				   M2-+ 		 */
		*om1 = &c1;
		*oc1 = &mem;
		*om2 = &c2;
		*memc= &c2;
		break;
	case 4:
		/* M1---C1-+-OUT */
		/* M2---C2-+	 */
		/* MEM: not used */
		*om1 = &c1;
		*oc1 = carrier;
		*om2 = &c2;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	case 5:
		/*	  +----C1----+	   */
		/* M1-+-MEM---M2-+-OUT */
		/*	  +----C2----+	   */
		*om1 = 0;	/* special mark */
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &m2;
		break;
	case 6:
		/* M1---C1-+	 */
		/*		M2-+-OUT */
		/*		C2-+	 */
		/* MEM: not used */
		*om1 = &c1;
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	case 7:
		/* M1-+ 	*/
		/* C1-+-OUT */
		/* M2-+ 	*/
		/* C2-+ 	*/
		/* MEM: not used*/
		*om1 = carrier;
		*oc1 = carrier;
		*om2 = carrier;
		*memc= &mem;	/* store it anywhere where it will not be used */
		break;
	}

	CH->connect4 = carrier;
}

/* set detune & multiple */
INLINE void set_det_mul(FM_ST *ST,FM_CH *CH,FM_SLOT *SLOT,int v)
{
	SLOT->mul = (v&0x0f)? (v&0x0f)*2 : 1;
	SLOT->DT  = ST->dt_tab[(v>>4)&7];
	CH->SLOT[SLOT1].Incr=-1;
}

/* set total level */
INLINE void set_tl(FM_CH *CH,FM_SLOT *SLOT , int v)
{
	SLOT->tl = (v&0x7f)<<(ENV_BITS-7); /* 7bit TL */
}

/* set attack rate & key scale	*/
INLINE void set_ar_ksr(FM_CH *CH,FM_SLOT *SLOT,int v)
{
	UINT8 old_KSR = SLOT->KSR;

	SLOT->ar = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;

	SLOT->KSR = 3-(v>>6);
	if (SLOT->KSR != old_KSR)
	{
		CH->SLOT[SLOT1].Incr=-1;
	}
	else
	{
		/* refresh Attack rate */
		if ((SLOT->ar + SLOT->ksr) < 32+62)
		{
			SLOT->eg_sh_ar	= eg_rate_shift [SLOT->ar  + SLOT->ksr ];
			SLOT->eg_sel_ar = eg_rate_select[SLOT->ar  + SLOT->ksr ];
		}
		else
		{
			SLOT->eg_sh_ar	= 0;
			SLOT->eg_sel_ar = 17*RATE_STEPS;
		}
	}
}

/* set decay rate */
INLINE void set_dr(FM_SLOT *SLOT,int v)
{
	SLOT->d1r = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;

	SLOT->eg_sh_d1r = eg_rate_shift [SLOT->d1r + SLOT->ksr];
	SLOT->eg_sel_d1r= eg_rate_select[SLOT->d1r + SLOT->ksr];

}

/* set sustain rate */
INLINE void set_sr(FM_SLOT *SLOT,int v)
{
	SLOT->d2r = (v&0x1f) ? 32 + ((v&0x1f)<<1) : 0;

	SLOT->eg_sh_d2r = eg_rate_shift [SLOT->d2r + SLOT->ksr];
	SLOT->eg_sel_d2r= eg_rate_select[SLOT->d2r + SLOT->ksr];
}

/* set release rate */
INLINE void set_sl_rr(FM_SLOT *SLOT,int v)
{
	SLOT->sl = sl_table[ v>>4 ];

	SLOT->rr  = 34 + ((v&0x0f)<<2);

	SLOT->eg_sh_rr	= eg_rate_shift [SLOT->rr  + SLOT->ksr];
	SLOT->eg_sel_rr = eg_rate_select[SLOT->rr  + SLOT->ksr];
}


INLINE signed int op_calc1(UINT32 phase, unsigned int env, signed int pm)
{
	UINT32 p;

	p = (env<<3) + sin_tab[ ( ((signed int)((phase & ~FREQ_MASK) + pm	   )) >> FREQ_SH ) & SIN_MASK ];

	if (p >= TL_TAB_LEN)
		return 0;
	return tl_tab[p];
}

/* advance LFO to next sample */
INLINE void advance_lfo(FM_OPN *OPN)
{
	UINT8 pos;
	UINT8 prev_pos;

	if (OPN->lfo_inc)	/* LFO enabled ? */
	{
		prev_pos = OPN->lfo_cnt>>LFO_SH & 127;

		OPN->lfo_cnt += OPN->lfo_inc;

		pos = (OPN->lfo_cnt >> LFO_SH) & 127;


		/* update AM when LFO output changes */

		/*if (prev_pos != pos)*/
		/* actually I can't optimize is this way without rewritting chan_calc()
		to use chip->lfo_am instead of global lfo_am */
		{

			/* triangle */
			/* AM: 0 to 126 step +2, 126 to 0 step -2 */
			if (pos<64)
				LFO_AM = (pos&63) * 2;
			else
				LFO_AM = 126 - ((pos&63) * 2);
		}

		/* PM works with 4 times slower clock */
		prev_pos >>= 2;
		pos >>= 2;
		/* update PM when LFO output changes */
		/*if (prev_pos != pos)*/ /* can't use global lfo_pm for this optimization, must be chip->lfo_pm instead*/
		{
			LFO_PM = pos;
		}

	}
	else
	{
		LFO_AM = 0;
		LFO_PM = 0;
	}
}

INLINE void advance_eg_channel(FM_OPN *OPN, FM_SLOT *SLOT)
{
	unsigned int out;
	unsigned int swap_flag = 0;
	unsigned int i;


	i = 4; /* four operators per channel */
	do
	{
		switch(SLOT->state)
		{
		case EG_ATT:		/* attack phase */
			if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_ar)-1) ) )
			{
				SLOT->volume += (~SLOT->volume *
								  (eg_inc[SLOT->eg_sel_ar + ((OPN->eg_cnt>>SLOT->eg_sh_ar)&7)])
								) >>4;

				if (SLOT->volume <= MIN_ATT_INDEX)
				{
					SLOT->volume = MIN_ATT_INDEX;
					SLOT->state = EG_DEC;
				}
			}
		break;

		case EG_DEC:	/* decay phase */
			if (SLOT->ssg&0x08) /* SSG EG type envelope selected */
			{
				if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_d1r)-1) ) )
				{
					SLOT->volume += 4 * eg_inc[SLOT->eg_sel_d1r + ((OPN->eg_cnt>>SLOT->eg_sh_d1r)&7)];

					if ( SLOT->volume >= SLOT->sl )
						SLOT->state = EG_SUS;
				}
			}
			else
			{
				if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_d1r)-1) ) )
				{
					SLOT->volume += eg_inc[SLOT->eg_sel_d1r + ((OPN->eg_cnt>>SLOT->eg_sh_d1r)&7)];

					if ( SLOT->volume >= SLOT->sl )
						SLOT->state = EG_SUS;
				}
			}
		break;

		case EG_SUS:	/* sustain phase */
			if (SLOT->ssg&0x08) /* SSG EG type envelope selected */
			{
				if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_d2r)-1) ) )
				{
					SLOT->volume += 4 * eg_inc[SLOT->eg_sel_d2r + ((OPN->eg_cnt>>SLOT->eg_sh_d2r)&7)];

					if ( SLOT->volume >= MAX_ATT_INDEX )
					{
						SLOT->volume = MAX_ATT_INDEX;

						if (SLOT->ssg&0x01) /* bit 0 = hold */
						{
							if (SLOT->ssgn&1)	/* have we swapped once ??? */
							{
								/* yes, so do nothing, just hold current level */
							}
							else
								swap_flag = (SLOT->ssg&0x02) | 1 ; /* bit 1 = alternate */

						}
						else
						{
							/* same as KEY-ON operation */

							/* restart of the Phase Generator should be here,
								only if AR is not maximum ??? */
							/*SLOT->phase = 0;*/

							/* phase -> Attack */
							SLOT->state = EG_ATT;

							swap_flag = (SLOT->ssg&0x02); /* bit 1 = alternate */
						}
					}
				}
			}
			else
			{
				if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_d2r)-1) ) )
				{
					SLOT->volume += eg_inc[SLOT->eg_sel_d2r + ((OPN->eg_cnt>>SLOT->eg_sh_d2r)&7)];

					if ( SLOT->volume >= MAX_ATT_INDEX )
					{
						SLOT->volume = MAX_ATT_INDEX;
						/* do not change SLOT->state (verified on real chip) */
					}
				}

			}
		break;

		case EG_REL:	/* release phase */
				if ( !(OPN->eg_cnt & ((1<<SLOT->eg_sh_rr)-1) ) )
				{
					SLOT->volume += eg_inc[SLOT->eg_sel_rr + ((OPN->eg_cnt>>SLOT->eg_sh_rr)&7)];

					if ( SLOT->volume >= MAX_ATT_INDEX )
					{
						SLOT->volume = MAX_ATT_INDEX;
						SLOT->state = EG_OFF;
					}
				}
		break;

		}

		out = SLOT->tl + ((UINT32)SLOT->volume);

		if ((SLOT->ssg&0x08) && (SLOT->ssgn&2)) /* negate output (changes come from alternate bit, init comes from attack bit) */
			out ^= ((1<<ENV_BITS)-1); /* 1023 */

		/* we need to store the result here because we are going to change ssgn
			in next instruction */
		SLOT->vol_out = out;

		SLOT->ssgn ^= swap_flag;

		SLOT++;
		i--;
	}while (i);

}



#define volume_calc(OP) ((OP)->vol_out + (AM & (OP)->AMmask))

INLINE void chan_calc(FM_OPN *OPN, FM_CH *CH)
{
	unsigned int eg_out;

	UINT32 AM = LFO_AM >> CH->ams;


	m2 = c1 = c2 = mem = 0;

	*CH->mem_connect = CH->mem_value;	/* restore delayed sample (MEM) value to m2 or c2 */

	eg_out = volume_calc(&CH->SLOT[SLOT1]);
	{
		INT32 out = CH->op1_out[0] + CH->op1_out[1];
		CH->op1_out[0] = CH->op1_out[1];

		if( !CH->connect1 ){
			/* algorithm 5	*/
			mem = c1 = c2 = CH->op1_out[0];
		}else{
			/* other algorithms */
			*CH->connect1 += CH->op1_out[0];
		}

		CH->op1_out[1] = 0;
		if( eg_out < ENV_QUIET )	/* SLOT 1 */
		{
			if (!CH->FB)
				out=0;

			CH->op1_out[1] = op_calc1(CH->SLOT[SLOT1].phase, eg_out, (out<<CH->FB) );
		}
	}

	eg_out = volume_calc(&CH->SLOT[SLOT3]);
	if( eg_out < ENV_QUIET )		/* SLOT 3 */
		*CH->connect3 += op_calc1(CH->SLOT[SLOT3].phase, eg_out, (m2<<15));

	eg_out = volume_calc(&CH->SLOT[SLOT2]);
	if( eg_out < ENV_QUIET )		/* SLOT 2 */
		*CH->connect2 += op_calc1(CH->SLOT[SLOT2].phase, eg_out, (c1<<15));

	eg_out = volume_calc(&CH->SLOT[SLOT4]);
	if( eg_out < ENV_QUIET )		/* SLOT 4 */
		*CH->connect4 += op_calc1(CH->SLOT[SLOT4].phase, eg_out, (c2<<15));


	/* store current MEM */
	CH->mem_value = mem;

	/* update phase counters AFTER output calculations */
	if(CH->pms)
	{
	/* add support for 3 slot mode */
		UINT32 block_fnum = CH->block_fnum;

		UINT32 fnum_lfo   = ((block_fnum & 0x7f0) >> 4) * 32 * 8;
		INT32  lfo_fn_table_index_offset = lfo_pm_table[ fnum_lfo + CH->pms + LFO_PM ];

		if (lfo_fn_table_index_offset)	/* LFO phase modulation active */
		{
			UINT8  blk;
			UINT32 fn;
			int kc,fc;

			block_fnum = block_fnum*2 + lfo_fn_table_index_offset;

			blk = (block_fnum&0x7000) >> 12;
			fn	= block_fnum & 0xfff;

			/* keyscale code */
			kc = (blk<<2) | opn_fktable[fn >> 8];
			/* phase increment counter */
			fc = OPN->fn_table[fn]>>(7-blk);

			CH->SLOT[SLOT1].phase += ((fc+CH->SLOT[SLOT1].DT[kc])*CH->SLOT[SLOT1].mul) >> 1;
			CH->SLOT[SLOT2].phase += ((fc+CH->SLOT[SLOT2].DT[kc])*CH->SLOT[SLOT2].mul) >> 1;
			CH->SLOT[SLOT3].phase += ((fc+CH->SLOT[SLOT3].DT[kc])*CH->SLOT[SLOT3].mul) >> 1;
			CH->SLOT[SLOT4].phase += ((fc+CH->SLOT[SLOT4].DT[kc])*CH->SLOT[SLOT4].mul) >> 1;
		}
		else	/* LFO phase modulation  = zero */
		{
			CH->SLOT[SLOT1].phase += CH->SLOT[SLOT1].Incr;
			CH->SLOT[SLOT2].phase += CH->SLOT[SLOT2].Incr;
			CH->SLOT[SLOT3].phase += CH->SLOT[SLOT3].Incr;
			CH->SLOT[SLOT4].phase += CH->SLOT[SLOT4].Incr;
		}
	}
	else	/* no LFO phase modulation */
	{
		CH->SLOT[SLOT1].phase += CH->SLOT[SLOT1].Incr;
		CH->SLOT[SLOT2].phase += CH->SLOT[SLOT2].Incr;
		CH->SLOT[SLOT3].phase += CH->SLOT[SLOT3].Incr;
		CH->SLOT[SLOT4].phase += CH->SLOT[SLOT4].Incr;
	}
}

/* update phase increment and envelope generator */
INLINE void refresh_fc_eg_slot(FM_SLOT *SLOT , int fc , int kc )
{
	int ksr;

	/* (frequency) phase increment counter */
	SLOT->Incr = ((fc+SLOT->DT[kc])*SLOT->mul) >> 1;

	ksr = kc >> SLOT->KSR;
	if( SLOT->ksr != ksr )
	{
		SLOT->ksr = ksr;

		/* calculate envelope generator rates */
		if ((SLOT->ar + SLOT->ksr) < 32+62)
		{
			SLOT->eg_sh_ar	= eg_rate_shift [SLOT->ar  + SLOT->ksr ];
			SLOT->eg_sel_ar = eg_rate_select[SLOT->ar  + SLOT->ksr ];
		}
		else
		{
			SLOT->eg_sh_ar	= 0;
			SLOT->eg_sel_ar = 17*RATE_STEPS;
		}

		SLOT->eg_sh_d1r = eg_rate_shift [SLOT->d1r + SLOT->ksr];
		SLOT->eg_sel_d1r= eg_rate_select[SLOT->d1r + SLOT->ksr];

		SLOT->eg_sh_d2r = eg_rate_shift [SLOT->d2r + SLOT->ksr];
		SLOT->eg_sel_d2r= eg_rate_select[SLOT->d2r + SLOT->ksr];

		SLOT->eg_sh_rr	= eg_rate_shift [SLOT->rr  + SLOT->ksr];
		SLOT->eg_sel_rr = eg_rate_select[SLOT->rr  + SLOT->ksr];
	}
}

/* update phase increment counters */
INLINE void refresh_fc_eg_chan(FM_CH *CH )
{
	if( CH->SLOT[SLOT1].Incr==-1){
		int fc = CH->fc;
		int kc = CH->kcode;
		refresh_fc_eg_slot(&CH->SLOT[SLOT1] , fc , kc );
		refresh_fc_eg_slot(&CH->SLOT[SLOT2] , fc , kc );
		refresh_fc_eg_slot(&CH->SLOT[SLOT3] , fc , kc );
		refresh_fc_eg_slot(&CH->SLOT[SLOT4] , fc , kc );
	}
}

/* initialize time tables */
static void init_timetables( FM_ST *ST , const UINT8 *dttable )
{
	int i,d;
	double rate;

	#if 0
	//logerror("FM.C: samplerate=%8i chip clock=%8i  freqbase=%f  \n", ST->rate, ST->clock, ST->freqbase );
	#endif

	/* DeTune table */
	for (d = 0;d <= 3;d++){
		for (i = 0;i <= 31;i++){
			rate = ((double)dttable[d*32 + i]) * SIN_LEN  * ST->freqbase_OPN/*OPN?SSG*/  * (1<<FREQ_SH) / ((double)(1<<20));
			ST->dt_tab[d][i]   = (INT32) rate;
			ST->dt_tab[d+4][i] = -ST->dt_tab[d][i];
			#if 0
			//logerror("FM.C: DT [%2i %2i] = %8x  \n", d, i, ST->dt_tab[d][i] );
			#endif
		}
	}
}


static void reset_channels( FM_ST *ST , FM_CH *CH , int num )
{
	int c,s;

	ST->mode   = 0; /* normal mode */
	ST->TA	   = 0;
	ST->TAC    = 0;
	ST->TB	   = 0;
	ST->TBC    = 0;

	for( c = 0 ; c < num ; c++ )
	{
		CH[c].fc = 0;
		for(s = 0 ; s < 4 ; s++ )
		{
			CH[c].SLOT[s].ssg = 0;
			CH[c].SLOT[s].ssgn = 0;
			CH[c].SLOT[s].state= EG_OFF;
			CH[c].SLOT[s].volume = MAX_ATT_INDEX;
			CH[c].SLOT[s].vol_out= MAX_ATT_INDEX;
		}
	}
}

/* initialize generic tables */
static /*int*/void init_tables(void)
{
	signed int i,x;
	signed int n;
	double o,m;

	for (x=0; x<TL_RES_LEN; x++)
	{
		m = (1<<16) / pow(2, (x+1) * (ENV_STEP/4.0) / 8.0);
		m = floor(m);

		/* we never reach (1<<16) here due to the (x+1) */
		/* result fits within 16 bits at maximum */

		n = (int)m; 	/* 16 bits here */
		n >>= 4;		/* 12 bits here */
		if (n&1)		/* round to nearest */
			n = (n>>1)+1;
		else
			n = n>>1;
						/* 11 bits here (rounded) */
		n <<= 2;		/* 13 bits here (as in real chip) */
		tl_tab[ x*2 + 0 ] = n;
		tl_tab[ x*2 + 1 ] = -tl_tab[ x*2 + 0 ];

		for (i=1; i<13; i++)
		{
			tl_tab[ x*2+0 + i*2*TL_RES_LEN ] =	tl_tab[ x*2+0 ]>>i;
			tl_tab[ x*2+1 + i*2*TL_RES_LEN ] = -tl_tab[ x*2+0 + i*2*TL_RES_LEN ];
		}
	#if 0
			//logerror("tl %04i", x);
			//for (i=0; i<13; i++)logerror(", [%02i] %4x", i*2, tl_tab[ x*2 /*+1*/ + i*2*TL_RES_LEN ]);
			//logerror("\n");
		}
	#endif
	}
	/*logerror("FM.C: TL_TAB_LEN = %i elements (%i bytes)\n",TL_TAB_LEN, (int)sizeof(tl_tab));*/


	for (i=0; i<SIN_LEN; i++)
	{
		/* non-standard sinus */
		m = sin( ((i*2)+1) * PI / SIN_LEN ); /* checked against the real chip */

		/* we never reach zero here due to ((i*2)+1) */

		if (m>0.0)	{	o = 8*log( 1.0/m)/log(2);}	/* convert to 'decibels' */
		else		{	o = 8*log(-1.0/m)/log(2);}	/* convert to 'decibels' */

		o = o / (ENV_STEP/4);

		n = (int)(2.0*o);
		#if 0
	//	if (n&1)	{	n = (n>>1)+1;}				/* round to nearest */
	//	else		{	n = (n>>1)  ;}
		#else
		if(n&1)n++; 		/* round to nearest */
		n>>=1;
		#endif

		sin_tab[ i ] = n*2 + (m>=0.0? 0: 1 );
		/*logerror("FM.C: sin [%4i]= %4i (tl_tab value=%5i)\n", i, sin_tab[i],tl_tab[sin_tab[i]]);*/
	}

	/*logerror("FM.C: ENV_QUIET= %08x\n",ENV_QUIET );*/


	/* build LFO PM modulation table */
	for(i = 0; i < 8; i++) /* 8 PM depths */
	{
		UINT8 fnum;
		for (fnum=0; fnum<128; fnum++) /* 7 bits meaningful of F-NUMBER */
		{
			UINT8 value;
			UINT8 step;
			UINT32 offset_depth = i;
			UINT32 offset_fnum_bit;
			UINT32 bit_tmp;

			for (step=0; step<8; step++)
			{
				value = 0;
				for (bit_tmp=0; bit_tmp<7; bit_tmp++) /* 7 bits */
				{
					if (fnum & (1<<bit_tmp)) /* only if bit "bit_tmp" is set */
					{
						offset_fnum_bit = bit_tmp * 8;
						value += lfo_pm_output[offset_fnum_bit + offset_depth][step];
					}
				}
				lfo_pm_table[(fnum*32*8) + (i*32) + step   + 0] =  value;
				lfo_pm_table[(fnum*32*8) + (i*32) +(step^7)+ 8] =  value;
				lfo_pm_table[(fnum*32*8) + (i*32) + step   +16] = -value;
				lfo_pm_table[(fnum*32*8) + (i*32) +(step^7)+24] = -value;
			}
#if 0
			//logerror("LFO depth=%1x FNUM=%04x (<<4=%4x): ", i, fnum, fnum<<4);
			//for (step=0; step<16; step++) /* dump only positive part of waveforms */
			//	logerror("%02x ", lfo_pm_table[(fnum*32*8) + (i*32) + step] );
			//logerror("\n");
#endif
		}
	}



//#ifdef SAVE_SAMPLE
//	sample[0]=fopen("sampsum.pcm","wb");
//#endif
//	return 1;
}





/* CSM Key Controll */
INLINE void CSMKeyControll(FM_CH *CH)
{
	/* this is wrong, atm */

	/* all key on */
	FM_KEYON(CH,SLOT1);
	FM_KEYON(CH,SLOT2);
	FM_KEYON(CH,SLOT3);
	FM_KEYON(CH,SLOT4);
}

//#if 0
//#ifdef _STATE_H
///* FM channel save , internal state only */
//static void FMsave_state_channel(const char *name,int num,FM_CH *CH,int num_ch)
//{
//	int slot , ch;
//	char state_name[20];
//	const char slot_array[4] = { 1 , 3 , 2 , 4 };
//
//	for(ch=0;ch<num_ch;ch++,CH++)
//	{
//		/* channel */
//		sprintf(state_name,"%s.CH%d",name,ch);
//		state_save_register_INT32(state_name, num, "feedback" , CH->op1_out , 2);
//		state_save_register_UINT32(state_name, num, "phasestep"   , &CH->fc , 1);
//		/* slots */
//		for(slot=0;slot<4;slot++)
//		{
//			FM_SLOT *SLOT = &CH->SLOT[slot];
//
//			sprintf(state_name,"%s.CH%d.SLOT%d",name,ch,slot_array[slot]);
//			state_save_register_UINT32(state_name, num, "phasecount" , &SLOT->phase, 1);
//			state_save_register_UINT8 (state_name, num, "state" 	 , &SLOT->state, 1);
//			state_save_register_INT16/*32*/(state_name, num, "volume"	  , &SLOT->volume, 1);
//		}
//	}
//}
//
//static void FMsave_state_st(const char *state_name,int num,FM_ST *ST)
//{
//#if FM_BUSY_FLAG_SUPPORT
//	state_save_register_double(state_name, num, "BusyExpire", &ST->BusyExpire , 1);
//#endif
//	state_save_register_UINT8 (state_name, num, "address"	, &ST->address , 1);
//	state_save_register_UINT8 (state_name, num, "IRQ"		, &ST->irq	   , 1);
//	state_save_register_UINT8 (state_name, num, "IRQ MASK"	, &ST->irqmask , 1);
//	state_save_register_UINT8 (state_name, num, "status"	, &ST->status  , 1);
//	state_save_register_UINT32(state_name, num, "mode"		, &ST->mode    , 1);
//	state_save_register_UINT8 (state_name, num, "prescaler" , &ST->prescaler_sel , 1);
//	state_save_register_UINT8 (state_name, num, "freq latch", &ST->fn_h , 1);
//	state_save_register_int   (state_name, num, "TIMER A"	, &ST->TA	);
//	state_save_register_int   (state_name, num, "TIMER Acnt", &ST->TAC	);
//	state_save_register_UINT8 (state_name, num, "TIMER B"	, &ST->TB	, 1);
//	state_save_register_int   (state_name, num, "TIMER Bcnt", &ST->TBC	);
//}
//#endif /* _STATE_H */
//#endif

#if BUILD_OPN




#endif /* BUILD_OPN */






#if (BUILD_YM2608||BUILD_YM2610||BUILD_YM2610B)



/* Algorithm and tables verified on real YM2608 and YM2610 */




/* speedup purposes only */
static int jedi_table[ 49*16 ];


static void Init_ADPCMATable(void)
{
/* usual ADPCM table (16 * 1.1^N) */
static const int steps[49] =
{
	 16,  17,	19,   21,	23,   25,	28,
	 31,  34,	37,   41,	45,   50,	55,
	 60,  66,	73,   80,	88,   97,  107,
	118, 130,  143,  157,  173,  190,  209,
	230, 253,  279,  307,  337,  371,  408,
	449, 494,  544,  598,  658,  724,  796,
	876, 963, 1060, 1166, 1282, 1411, 1552
};
	int step, nib;

	for (step = 0; step < 49; step++)
	{
		/* loop over all nibbles and compute the difference */
		for (nib = 0; nib < 16; nib++)
		{
			int value = (2*(nib & 0x07) + 1) * steps[step] / 8;
			jedi_table[step*16 + nib] = (nib&0x08) ? -value : value;
		}
	}
}

/* ADPCM A (Non control type) : calculate one channel output */
INLINE void ADPCMA_calc_chan( /*YM2610 *F2610,*/ ADPCM_CH *ch )
{
	UINT32 step;
	UINT8  data;

	ch->now_step += ch->step;
	if ( ch->now_step >= (1<<ADPCM_SHIFT) )
	{
		step = ch->now_step >> ADPCM_SHIFT;
		ch->now_step &= (1<<ADPCM_SHIFT)-1;
		do{
			/* end check */
			/* 11-06-2001 JB: corrected comparison. Was > instead of == */
			/* YM2610 checks lower 20 bits only, the 4 MSB bits are sample bank */
			/* Here we use 1<<21 to compensate for nibble calculations */

			if ( (ch->now_addr & ((1<<21)-1)) == ((ch->end<<1) & ((1<<21)-1)) )
			{
				ch->flag = 0;
				/*F2610->*/YM2610.adpcm_arrivedEndAddress |= ch->flagMask;
				return;
			}
			#if 0
			if ( ch->now_addr > (pcmsizeA<<1) ) {
				LOG(LOG_WAR,("YM2610: Attempting to play past adpcm rom size!\n" ));
				return;
			}
			#endif
			if ( ch->now_addr&1 )
			{	data = ch->now_data & 0x0f;}
			else
			{
				ch->now_data = *(YM2610.pcmbufA+(ch->now_addr>>1));
				data = (ch->now_data >> 4) & 0x0f;
			}

			ch->now_addr++;

			ch->adpcm_acc += jedi_table[ch->adpcm_step + data];

			/* extend 12-bit signed int */
			if (ch->adpcm_acc & 0x800)
			{	ch->adpcm_acc |= ~0xfff;}
			else
			{	ch->adpcm_acc &= 0xfff;}

			{/* different from the usual ADPCM table */
			/*static*/ const int step_inc[8] = { -1*16, -1*16, -1*16, -1*16, 2*16, 5*16, 7*16, 9*16 };
			ch->adpcm_step += step_inc[data & 7];
			}
			myLimit( ch->adpcm_step, 48*16, 0*16 );

		}while(--step);

		/* calc pcm * volume data */
		ch->adpcm_out = ((ch->adpcm_acc * ch->vol_mul) >> ch->vol_shift) & ~3;	/* multiply, shift and mask out 2 LSB bits */
	}

	/* output for work of output channels (out_adpcm[OPNxxxx])*/
	*(ch->pan) += ch->adpcm_out;
}

/* ADPCM type A Write */
static void OPNB_ADPCMA_write(/*YM2610 *F2610,*//*int*/int/*UINT8*/ addr,int v)
{
	ADPCM_CH *adpcm = /*F2610->*/YM2610.adpcm;
	UINT8 c;// = addr&0x07;

//	/*F2610->*/YM2610.REGS[addr /*+ 0x100*/] = v&0xff; /* stock data */
	#define FIX_HIGH (0x100)
	switch (addr)
	{
	case (0x100/*-FIX_HIGH*/): /* DM,--,C5,C4,C3,C2,C1,C0 */
		if( !(v&0x80) )
		{
			/* KEY ON */
			for( c = 0; c < 6; c++ )
			{
				if( (v>>c)&1 )
				{
					/**** start adpcm ****/
					adpcm[c].step	   = (UINT32)((float)(1<<ADPCM_SHIFT)*((float)/*F2610->*/YM2610.OPN.ST.freqbase_OPN/*OPN?SSG*/)/3.0);
					adpcm[c].now_addr  = adpcm[c].start<<1;
					adpcm[c].now_step  = 0;
					adpcm[c].adpcm_acc = 0;
					adpcm[c].adpcm_step= 0;
					adpcm[c].adpcm_out = 0;
					adpcm[c].flag	   = 1;

					if(/*F2610->*//*YM2610.pcmbuf*/YM2610.pcmbufA==NULL){ 				/* Check ROM Mapped */
						//logerror("YM2608-YM2610: ADPCM-A rom not mapped\n");
						adpcm[c].flag = 0;
					} else{
					//	if(adpcm[c].end >= /*F2610->*/YM2610.pcm_size){ /* Check End in Range */
					//		//logerror("YM2610: ADPCM-A end out of range: $%08x\n",adpcm[c].end);
					//		/*adpcm[c].end = /*F2610->*/YM2610.pcm_size-1;*/ /* JB: DO NOT uncomment this, otherwise you will break the comparison in the ADPCM_CALC_CHA() */
					//	}
						if(adpcm[c].start >= /*F2610->*//*YM2610.pcm_size*/YM2610.pcmsizeA)	/* Check Start in Range */
						{
							//logerror("YM2608-YM2610: ADPCM-A start out of range: $%08x\n",adpcm[c].start);
							adpcm[c].flag = 0;
						}
					}
				}
			}
		}
		else
		{
			/* KEY OFF */
			for( c = 0; c < 6; c++ )
				if( (v>>c)&1 )
					adpcm[c].flag = 0;
		}
		break;
	case (0x101/*-FIX_HIGH*/):	/* B0-5 = TL */
		/*F2610->*/YM2610.adpcmTL = (v & 0x3f) ^ 0x3f;
		for( c = 0; c < 6; c++ )
		{
			int volume = /*F2610->*/YM2610.adpcmTL + adpcm[c].IL;

			if ( volume >= 63 ) /* This is correct, 63 = quiet */
			{
				adpcm[c].vol_mul   = 0;
				adpcm[c].vol_shift = 0;
			}
			else
			{
				adpcm[c].vol_mul   = 15 - (volume & 7); 	/* so called 0.75 dB */
				adpcm[c].vol_shift =  1 + (volume >> 3);	/* Yamaha engineers used the approximation: each -6 dB is close to divide by two (shift right) */
			}

			/* calc pcm * volume data */
			adpcm[c].adpcm_out = ((adpcm[c].adpcm_acc * adpcm[c].vol_mul) >> adpcm[c].vol_shift) & ~3;	/* multiply, shift and mask out low 2 bits */
		}
		break;
	default:

		c = addr&0x07;
		if( c >= 0x06 ) return;
		switch( addr&(0x138-FIX_HIGH) ){
		case (0x108-FIX_HIGH):	/* B7=L,B6=R, B4-0=IL */
		{
			int volume;

			adpcm[c].IL = (v & 0x1f) ^ 0x1f;

			volume = /*F2610->*/YM2610.adpcmTL + adpcm[c].IL;

			if ( volume >= 63 ) /* This is correct, 63 = quiet */
			{
				adpcm[c].vol_mul   = 0;
				adpcm[c].vol_shift = 0;
			}
			else
			{
				adpcm[c].vol_mul   = 15 - (volume & 7); 	/* so called 0.75 dB */
				adpcm[c].vol_shift =  1 + (volume >> 3);	/* Yamaha engineers used the approximation: each -6 dB is close to divide by two (shift right) */
			}

			adpcm[c].pan	= &out_adpcm[(v>>6)&0x03];

			/* calc pcm * volume data */
			adpcm[c].adpcm_out = ((adpcm[c].adpcm_acc * adpcm[c].vol_mul) >> adpcm[c].vol_shift) & ~3;	/* multiply, shift and mask out low 2 bits */
		}
			break;
		case (0x110-FIX_HIGH):
		case (0x118-FIX_HIGH):
			adpcm[c].start	= ( (/*F2610->*/YM2610.REGS[0x118 + c]*0x0100 | /*F2610->*/YM2610.REGS[0x110 + c]) << ADPCMA_ADDRESS_SHIFT);
			break;
		case (0x120-FIX_HIGH):
		case (0x128-FIX_HIGH):
			adpcm[c].end	= ( (/*F2610->*/YM2610.REGS[0x128 + c]*0x0100 | /*F2610->*/YM2610.REGS[0x120 + c]) << ADPCMA_ADDRESS_SHIFT);
			adpcm[c].end   += (1<<ADPCMA_ADDRESS_SHIFT) - 1;
			break;
		}
	}
	#undef FIX_HIGH
}
//#if 0
//#ifdef _STATE_H
///* FM channel save , internal state only */
//static void FMsave_state_adpcma(const char *name,int num,ADPCM_CH *adpcm)
//{
//	int ch;
//	char state_name[20];
//
//	for(ch=0;ch<6;ch++,adpcm++)
//	{
//		sprintf(state_name,"%s.CH%d",name,ch);
//
//		state_save_register_UINT8 (state_name, num, "flag"	  , &adpcm->flag	  , 1);
//		state_save_register_UINT8 (state_name, num, "data"	  , &adpcm->now_data  , 1);
//		state_save_register_UINT32(state_name, num, "addr"	  , &adpcm->now_addr  , 1);
//		state_save_register_UINT32(state_name, num, "step"	  , &adpcm->now_step  , 1);
//		state_save_register_INT32 (state_name, num, "a_acc"   , &adpcm->adpcm_acc , 1);
//		state_save_register_INT32 (state_name, num, "a_step"  , &adpcm->adpcm_step, 1);
//		state_save_register_INT32 (state_name, num, "a_out"   , &adpcm->adpcm_out , 1);
//	}
//}
//#endif /* _STATE_H */
//#endif

#endif /* (BUILD_YM2608||BUILD_YM2610||BUILD_YM2610B) */



/* YM2610(OPNB) */

#if BUILD_YM2610/*2610only*/
/* Generate samples for one of the YM2610s */

//static void ym2610_stream_update(void *param, stream_sample_t **inputs, stream_sample_t **buffer/*s*/, int length)
static void YM2610Update_stream(s16 *mixing_buffer, int length)
//{
//	struct ym2610_info *info = param;
//	YM2610UpdateOne(/*info->chip,*/ buffers, length);
//}
//void YM2610UpdateOne(/*void *_dummychip,*/ FMSAMPLE **buffer, int length)
{
	//YM2610 *F2610 = chip;
	FM_OPN *OPN   = &/*F2610->*/YM2610.OPN;
	int i,j;
	int outn;
//	FMSAMPLE  *bufL;
//	FMSAMPLE  *bufR;
///*static*/ FM_CH	*cch[4/*8*/];		/* pointer of FM channels */

	/* buffer setup */
//	bufL = buffer[0];
//	bufR = buffer[1];

	//if( (void *)F2610 != cur_chip )
	{
		//cur_chip = (void *)F2610;
	//	State = &OPN->ST;
		cch[0] = &/*F2610->*/YM2610.CH[1];
		cch[1] = &/*F2610->*/YM2610.CH[2];
		cch[2] = &/*F2610->*/YM2610.CH[4];
		cch[3] = &/*F2610->*/YM2610.CH[5];
	}
//#ifdef YM2610B_WARNING
//#define FM_KEY_IS(SLOT) ((SLOT)->key)
//#define FM_MSG_YM2610B "YM2610-%p.CH%d is playing,Check whether the type of the chip is YM2610B\n"
//	/* Check YM2610B warning message */
//	if( FM_KEY_IS(&/*F2610->*/YM2610.CH[0].SLOT[3]) ) LOG(LOG_WAR,(FM_MSG_YM2610B,/*F2610->*/YM2610.OPN.ST.param,0));
//	if( FM_KEY_IS(&/*F2610->*/YM2610.CH[3].SLOT[3]) ) LOG(LOG_WAR,(FM_MSG_YM2610B,/*F2610->*/YM2610.OPN.ST.param,3));
//#endif

	/* refresh PG and EG */
	refresh_fc_eg_chan( cch[0] );
	if( (/*State->*/OPN->ST.mode & 0xc0) )
	{
		/* 3SLOT MODE */
		if( cch[1]->SLOT[SLOT1].Incr==-1)
		{
			refresh_fc_eg_slot(&cch[1]->SLOT[SLOT1] , OPN->SL3.fc[1] , OPN->SL3.kcode[1] );
			refresh_fc_eg_slot(&cch[1]->SLOT[SLOT2] , OPN->SL3.fc[2] , OPN->SL3.kcode[2] );
			refresh_fc_eg_slot(&cch[1]->SLOT[SLOT3] , OPN->SL3.fc[0] , OPN->SL3.kcode[0] );
			refresh_fc_eg_slot(&cch[1]->SLOT[SLOT4] , cch[1]->fc , cch[1]->kcode );
		}
	}else refresh_fc_eg_chan( cch[1] );
	refresh_fc_eg_chan( cch[2] );
	refresh_fc_eg_chan( cch[3] );

	/* calc SSG count */
	outn = SSG_calc_count(length);

	/* buffering */
	for(i=0; i < length ;)
	{
		advance_lfo(OPN);
		/* clear output acc. */
		out_adpcm[OUTD_LEFT] = out_adpcm[OUTD_RIGHT]= out_adpcm[OUTD_CENTER] = 0;
		out_delta[OUTD_LEFT] = out_delta[OUTD_RIGHT]= out_delta[OUTD_CENTER] = 0;
		/* clear outputs */
		out_fm[1] = 0;
		out_fm[2] = 0;
		out_fm[4] = 0;
		out_fm[5] = 0;
		/* clear outputs SSG */
//		out_ssg = 0;

		/* advance envelope generator */
		OPN->eg_timer += OPN->eg_timer_add;
		while (OPN->eg_timer >= OPN->eg_timer_overflow)
		{
			OPN->eg_timer -= OPN->eg_timer_overflow;
			OPN->eg_cnt++;

			advance_eg_channel(OPN, &cch[0]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[1]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[2]->SLOT[SLOT1]);
			advance_eg_channel(OPN, &cch[3]->SLOT[SLOT1]);
		}

		/* calculate FM */
		chan_calc(OPN, cch[0] );	/*remapped to 1*/
		chan_calc(OPN, cch[1] );	/*remapped to 2*/
		chan_calc(OPN, cch[2] );	/*remapped to 4*/
		chan_calc(OPN, cch[3] );	/*remapped to 5*/

		/* calculate SSG */
		outn = SSG_CALC(outn);

		/* deltaT ADPCM */
//	ADPCMB *adpcmb = &/*F2610->*/YM2610.deltaT;
		if( /*adpcmb->*/YM2610.deltaT.portstate&0x80 )
		{	OPNB_ADPCMB_calc(/*DELTAT*/&/*F2610->*/YM2610.deltaT);}

		/* ADPCMA */
		for( j = 0; j < 6; j++ )
		{
			if( /*F2610->*/YM2610.adpcm[j].flag )
				ADPCMA_calc_chan( /*F2610,*/ &/*F2610->*/YM2610.adpcm[j]);
		}

#if 0
		/* buffering */
		{
			int lt,rt;
			lt	= ((out_adpcm[OUTD_LEFT]  + out_adpcm[OUTD_CENTER])<<1);
			lt += ((out_delta[OUTD_LEFT]  + out_delta[OUTD_CENTER])>>8);
			lt += ((out_ssg));/*adjusted in ssg, PSG_VolTable[] */
			if(OPN->pan[ 2]) {lt += ((out_fm[1]) );}
			if(OPN->pan[ 4]) {lt += ((out_fm[2]) );}
			if(OPN->pan[ 8]) {lt += ((out_fm[4]) );}
			if(OPN->pan[10]) {lt += ((out_fm[5]) );}
			lt>>=1;/* 2610/2610B/2608 final shift */ /* the shift right was verified on real chip */
			#if (FM_SAMPLE_BITS==16)
			#else
			lt >>= 8/*FINAL_SH*/;/*for 8bit sound only(not psp)*/
			#endif
			myLimit( lt, MAXOUT, MINOUT );

			rt	= ((out_adpcm[OUTD_RIGHT] + out_adpcm[OUTD_CENTER])<<1);
			rt += ((out_delta[OUTD_RIGHT] + out_delta[OUTD_CENTER])>>8);
			rt += ((out_ssg));/*adjusted in ssg, PSG_VolTable[] */
			if(OPN->pan[ 3]) {rt += ((out_fm[1]) );}
			if(OPN->pan[ 5]) {rt += ((out_fm[2]) );}
			if(OPN->pan[ 9]) {rt += ((out_fm[4]) );}
			if(OPN->pan[11]) {rt += ((out_fm[5]) );}
			rt>>=1;/* 2610/2610B/2608 final shift */ /* the shift right was verified on real chip */
			#if (FM_SAMPLE_BITS==16)
			#else
			rt >>= 8/*FINAL_SH*/;/*for 8bit sound only(not psp)*/
			#endif
			myLimit( rt, MAXOUT, MINOUT );
			/* buffering */
			#if (2==RENDER_DIV_TYPE)
			//bufL[i] = lt;
			//bufR[i] = rt;
			mixing_buffer[i] = (lt+rt);
			i++;
			#endif //(2==RENDER_DIV_TYPE)
			//bufL[i] = lt;
			//bufR[i] = rt;
			mixing_buffer[i] = (lt+rt);
			i++;
			//	SAVE_ALL_CHANNELS
		}
#else
		/* buffering */
		{
			int lt;
			lt	= ((out_adpcm[OUTD_LEFT]  + out_adpcm[OUTD_CENTER])<<1);
			lt += ((out_adpcm[OUTD_RIGHT] + out_adpcm[OUTD_CENTER])<<1);
			lt += ((out_delta[OUTD_LEFT]  + out_delta[OUTD_CENTER])>>8);
			lt += ((out_delta[OUTD_RIGHT] + out_delta[OUTD_CENTER])>>8);
			lt += ((out_ssg));/*adjusted in ssg, PSG_VolTable[] */
			lt += ((out_ssg));/*adjusted in ssg, PSG_VolTable[] */
			if(OPN->pan[ 2]) {lt += (out_fm[1]);}
			if(OPN->pan[ 3]) {lt += (out_fm[1]);}
			if(OPN->pan[ 4]) {lt += (out_fm[2]);}
			if(OPN->pan[ 5]) {lt += (out_fm[2]);}
			if(OPN->pan[ 8]) {lt += (out_fm[4]);}
			if(OPN->pan[ 9]) {lt += (out_fm[4]);}
			if(OPN->pan[10]) {lt += (out_fm[5]);}
			if(OPN->pan[11]) {lt += (out_fm[5]);}
			#if 0
		//	lt>>=1;/* 2610/2610B/2608 final shift */ /* the shift right was verified on real chip */
		//	lt>>=1;/* 2610/2610B/2608 final shift */ /* the shift right was verified on real chip */
			#else
			lt>>=2;
			#endif
//
			#if (FM_SAMPLE_BITS==16)
			#else
			lt >>= 8/*FINAL_SH*/;/*for 8bit sound only(not psp)*/
			lt >>= 8/*FINAL_SH*/;/*for 8bit sound only(not psp)*/
			#endif
			myLimit( lt, MAXOUT, MINOUT );

			/* buffering */
			#if (2==RENDER_DIV_TYPE)
			//bufL[i] = lt;
			//bufR[i] = rt;
			mixing_buffer[i] = (lt);
			i++;
			#endif //(2==RENDER_DIV_TYPE)
			//bufL[i] = lt;
			//bufR[i] = rt;
			mixing_buffer[i] = (lt);
			i++;
			//	SAVE_ALL_CHANNELS
		}
#endif
		/* timer A control */
		INTERNAL_TIMER_A(/*State,*/ cch[1] )
	}
	INTERNAL_TIMER_B(/*State,*/ length)
}
#endif //BUILD_YM2610/*2610only*/






#if (BUILD_YM2610||BUILD_YM2610B)

//#ifdef _STATE_H
//void YM2610Postload(void *chip)
//{
//	if (chip)
//	{
//		//YM2610 *F2610 = (YM2610 *)chip;
//		int r;
//
//		/* SSG registers */
//		for(r=0;r<16;r++)
//		{
//			/*(*/*F2610->*/YM2610.OPN.ST.SSG->write)*//*psg_write*/SSG_write(/*F2610->*//*YM2610.OPN.ST.param,*/0,r);
//			/*(*/*F2610->*/YM2610.OPN.ST.SSG->write)*//*psg_write*/SSG_write(/*F2610->*//*YM2610.OPN.ST.param,*/1,/*F2610->*/YM2610.REGS[r]);
//		}
//
//		/* OPN registers */
//		/* DT / MULTI , TL , KS / AR , AMON / DR , SR , SL / RR , SSG-EG */
//		for(r=0x30;r<0x9e;r++)
//			if((r&3) != 3)
//			{
//				OPNWriteReg(&/*F2610->*/YM2610.OPN,r,/*F2610->*/YM2610.REGS[r]);
//				OPNWriteReg(&/*F2610->*/YM2610.OPN,r|0x100,/*F2610->*/YM2610.REGS[r|0x100]);
//			}
//		/* FB / CONNECT , L / R / AMS / PMS */
//		for(r=0xb0;r<0xb6;r++)
//			if((r&3) != 3)
//			{
//				OPNWriteReg(&/*F2610->*/YM2610.OPN,r,/*F2610->*/YM2610.REGS[r]);
//				OPNWriteReg(&/*F2610->*/YM2610.OPN,r|0x100,/*F2610->*/YM2610.REGS[r|0x100]);
//			}
//		/* FM channels */
//		/*FM_channel_postload(/*F2610->*/YM2610.CH,6);*/
//
//		/* rhythm(ADPCMA) */
//		OPNB_ADPCMA_write(F2610,1,/*F2610->*/YM2610.REGS[0x101]);
//		for( r=0 ; r<6 ; r++)
//		{
//			OPNB_ADPCMA_write(F2610,r+0x08,/*F2610->*/YM2610.REGS[r+0x108]);
//			OPNB_ADPCMA_write(F2610,r+0x10,/*F2610->*/YM2610.REGS[r+0x110]);
//			OPNB_ADPCMA_write(F2610,r+0x18,/*F2610->*/YM2610.REGS[r+0x118]);
//			OPNB_ADPCMA_write(F2610,r+0x20,/*F2610->*/YM2610.REGS[r+0x120]);
//			OPNB_ADPCMA_write(F2610,r+0x28,/*F2610->*/YM2610.REGS[r+0x128]);
//		}
//		/* Delta-T ADPCM unit */
//		ADPCMB_postload(&/*F2610->*/YM2610.deltaT , &/*F2610->*/YM2610.REGS[0x010] );
//	}
//	cur_chip = NULL;
//}
//
//#if 0
//static void YM2610_save_state(YM2610 *F2610, int index)
//{
//	const char statename[] = "YM2610";
//
//	state_save_register_UINT8 (statename, index, "regs"   , /*F2610->*/YM2610.REGS	 , 512);
//	FMsave_state_st(statename,index,&/*F2610->*/YM2610.OPN.ST);
//	FMsave_state_channel(statename,index,/*F2610->*/YM2610.CH,6);
//	/* 3slots */
//	state_save_register_UINT32(statename, index, "slot3fc" , /*F2610->*/YM2610.OPN.SL3.fc	 , 3);
//	state_save_register_UINT8 (statename, index, "slot3fh" , &/*F2610->*/YM2610.OPN.SL3.fn_h, 1);
//	state_save_register_UINT8 (statename, index, "slot3kc" , /*F2610->*/YM2610.OPN.SL3.kcode, 3);
//	/* address register1 */
//	state_save_register_UINT8 (statename, index, "addr_A1" , &/*F2610->*/YM2610.addr_A1, 1);
//
//	state_save_register_UINT8 (statename, index, "arrivedFlag", &/*F2610->*/YM2610.adpcm_arrivedEndAddress , 1);
//	/* rythm(ADPCMA) */
//	FMsave_state_adpcma(statename,index,/*F2610->*/YM2610.adpcm);
//	/* Delta-T ADPCM unit */
//	ADPCMB_savestate(statename,index,&/*F2610->*/YM2610.deltaT);
//}
//#endif
//#endif /* _STATE_H */

//static void YM2610_deltat_status_set(/*void *chip,*/ UINT8 changebits)
//{
//	//YM2610 *F2610 = chip;
//	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= changebits;
//}
//static void YM2610_deltat_status_reset(/*void *chip,*/ UINT8 changebits)
//{
//	//YM2610 *F2610 = chip;
//	/*F2610->*/YM2610.adpcm_arrivedEndAddress &= (~changebits);
//}








/* write a OPN mode register 0x20-0x2f */
static void OPNWriteMode(/*FM_OPN *OPN,*/ int r, /*int*/UINT8 v)
{
	FM_OPN *OPN   = &/*F2610->*/YM2610.OPN;
	UINT8 c;
	FM_CH *CH;

	switch(r){
	case 0x21:	/* Test */
		break;
	case 0x22:	/* LFO FREQ (YM2608/YM2610/YM2610B/YM2612) */
		if( OPN->type & TYPE_LFOPAN )
		{
			if (v&0x08) /* LFO enabled ? */
			{
				OPN->lfo_inc = OPN->lfo_freq[v&7];
			}
			else
			{
				OPN->lfo_inc = 0;
			}
		}
		break;
	case 0x24:	/* timer A High 8*/
		OPN->ST.TA = (OPN->ST.TA & 0x03)|(((int)v)<<2);
		break;
	case 0x25:	/* timer A Low 2*/
		OPN->ST.TA = (OPN->ST.TA & 0x3fc)|(v&3);
		break;
	case 0x26:	/* timer B */
		OPN->ST.TB = v;
		break;
	case 0x27:	/* mode, timer control */
		set_timers( &(OPN->ST),/*OPN->ST.param,*/v );
		break;
	case 0x28:	/* key on / off */
		c = v & 0x03;
		if( c == 3 ) break;
		if( (v&0x04) && (OPN->type & TYPE_6CH) ) c+=3;
		CH = OPN->P_CH;
		CH = &CH[c];
		if(v&0x10) FM_KEYON(CH,SLOT1); else FM_KEYOFF(CH,SLOT1);
		if(v&0x20) FM_KEYON(CH,SLOT2); else FM_KEYOFF(CH,SLOT2);
		if(v&0x40) FM_KEYON(CH,SLOT3); else FM_KEYOFF(CH,SLOT3);
		if(v&0x80) FM_KEYON(CH,SLOT4); else FM_KEYOFF(CH,SLOT4);
		break;
	}
}

/* write a OPN register (0x30-0xff) */
static void OPNWriteReg(/*FM_OPN *OPN,*/ int r, /*int*/UINT8 v)
{
	FM_OPN *OPN   = &/*F2610->*/YM2610.OPN;
	FM_CH *CH;
	FM_SLOT *SLOT;

	UINT8 c = OPN_CHAN(r);

	if (c == 3) return; /* 0xX3,0xX7,0xXB,0xXF */

	if (r >= 0x100) c+=3;

	CH = OPN->P_CH;
	CH = &CH[c];

	SLOT = &(CH->SLOT[OPN_SLOT(r)]);

	switch( r & 0xf0 ) {
	case 0x30:	/* DET , MUL */
		set_det_mul(&OPN->ST,CH,SLOT,v);
		break;

	case 0x40:	/* TL */
		set_tl(CH,SLOT,v);
		break;

	case 0x50:	/* KS, AR */
		set_ar_ksr(CH,SLOT,v);
		break;

	case 0x60:	/* bit7 = AM ENABLE, DR */
		set_dr(SLOT,v);

		if(OPN->type & TYPE_LFOPAN) /* YM2608/2610/2610B/2612 */
		{
			SLOT->AMmask = (v&0x80) ? ~0 : 0;
		}
		break;

	case 0x70:	/*	   SR */
		set_sr(SLOT,v);
		break;

	case 0x80:	/* SL, RR */
		set_sl_rr(SLOT,v);
		break;

	case 0x90:	/* SSG-EG */

		SLOT->ssg  =  v&0x0f;
		SLOT->ssgn = (v&0x04)>>1; /* bit 1 in ssgn = attack */

		/* SSG-EG envelope shapes :

		E AtAlH
		1 0 0 0  \\\\

		1 0 0 1  \___

		1 0 1 0  \/\/
				  ___
		1 0 1 1  \

		1 1 0 0  ////
				  ___
		1 1 0 1  /

		1 1 1 0  /\/\

		1 1 1 1  /___


		E = SSG-EG enable


		The shapes are generated using Attack, Decay and Sustain phases.

		Each single character in the diagrams above represents this whole
		sequence:

		- when KEY-ON = 1, normal Attack phase is generated (*without* any
		  difference when compared to normal mode),

		- later, when envelope level reaches minimum level (max volume),
		  the EG switches to Decay phase (which works with bigger steps
		  when compared to normal mode - see below),

		- later when envelope level passes the SL level,
		  the EG swithes to Sustain phase (which works with bigger steps
		  when compared to normal mode - see below),

		- finally when envelope level reaches maximum level (min volume),
		  the EG switches to Attack phase again (depends on actual waveform).

		Important is that when switch to Attack phase occurs, the phase counter
		of that operator will be zeroed-out (as in normal KEY-ON) but not always.
		(I havent found the rule for that - perhaps only when the output level is low)

		The difference (when compared to normal Envelope Generator mode) is
		that the resolution in Decay and Sustain phases is 4 times lower;
		this results in only 256 steps instead of normal 1024.
		In other words:
		when SSG-EG is disabled, the step inside of the EG is one,
		when SSG-EG is enabled, the step is four (in Decay and Sustain phases).

		Times between the level changes are the same in both modes.


		Important:
		Decay 1 Level (so called SL) is compared to actual SSG-EG output, so
		it is the same in both SSG and no-SSG modes, with this exception:

		when the SSG-EG is enabled and is generating raising levels
		(when the EG output is inverted) the SL will be found at wrong level !!!
		For example, when SL=02:
			0 -6 = -6dB in non-inverted EG output
			96-6 = -90dB in inverted EG output
		Which means that EG compares its level to SL as usual, and that the
		output is simply inverted afterall.


		The Yamaha's manuals say that AR should be set to 0x1f (max speed).
		That is not necessary, but then EG will be generating Attack phase.

		*/


		break;

	case 0xa0:
		switch( OPN_SLOT(r) ){
		case 0: 	/* 0xa0-0xa2 : FNUM1 */
			{
				UINT32 fn = (((UINT32)( (OPN->ST.fn_h)&7))<<8) + v;
				UINT8 blk = OPN->ST.fn_h>>3;
				/* keyscale code */
				CH->kcode = (blk<<2) | opn_fktable[fn >> 7];
				/* phase increment counter */
				CH->fc = OPN->fn_table[fn*2]>>(7-blk);

				/* store fnum in clear form for LFO PM calculations */
				CH->block_fnum = (blk<<11) | fn;

				CH->SLOT[SLOT1].Incr=-1;
			}
			break;
		case 1: 	/* 0xa4-0xa6 : FNUM2,BLK */
			OPN->ST.fn_h = v&0x3f;
			break;
		case 2: 	/* 0xa8-0xaa : 3CH FNUM1 */
			if(r < 0x100)
			{
				UINT32 fn = (((UINT32)(OPN->SL3.fn_h&7))<<8) + v;
				UINT8 blk = OPN->SL3.fn_h>>3;
				/* keyscale code */
				OPN->SL3.kcode[c]= (blk<<2) | opn_fktable[fn >> 7];
				/* phase increment counter */
				OPN->SL3.fc[c] = OPN->fn_table[fn*2]>>(7-blk);
				OPN->SL3.block_fnum[c] = fn;
				(OPN->P_CH)[2].SLOT[SLOT1].Incr=-1;
			}
			break;
		case 3: 	/* 0xac-0xae : 3CH FNUM2,BLK */
			if(r < 0x100)
				OPN->SL3.fn_h = v&0x3f;
			break;
		}
		break;

	case 0xb0:
		switch( OPN_SLOT(r) ){
		case 0: 	/* 0xb0-0xb2 : FB,ALGO */
			{
				int feedback = (v>>3)&7;
				CH->ALGO = v&7;
				CH->FB	 = feedback ? feedback+6 : 0;
				setup_connection( CH, c );
			}
			break;
		case 1: 	/* 0xb4-0xb6 : L , R , AMS , PMS (YM2612/YM2610B/YM2610/YM2608) */
			if( OPN->type & TYPE_LFOPAN)
			{
				/* b0-2 PMS */
				CH->pms = (v & 7) * 32; /* CH->pms = PM depth * 32 (index in lfo_pm_table) */

				{
				/*There are 4 different LFO AM depths available, they are:
				  0 dB, 1.4 dB, 5.9 dB, 11.8 dB
				  Here is how it is generated (in EG steps):

				  11.8 dB = 0, 2, 4, 6, 8, 10,12,14,16...126,126,124,122,120,118,....4,2,0
				   5.9 dB = 0, 1, 2, 3, 4, 5, 6, 7, 8....63, 63, 62, 61, 60, 59,.....2,1,0
				   1.4 dB = 0, 0, 0, 0, 1, 1, 1, 1, 2,...15, 15, 15, 15, 14, 14,.....0,0,0

				  (1.4 dB is loosing precision as you can see)

				  It's implemented as generator from 0..126 with step 2 then a shift
				  right N times, where N is:
					8 for 0 dB
					3 for 1.4 dB
					1 for 5.9 dB
					0 for 11.8 dB
				*/
				/*static*/ const UINT8 lfo_ams_depth_shift[4] = {8, 3, 1, 0};

				/* b4-5 AMS */
				CH->ams = lfo_ams_depth_shift[(v>>4) & 0x03];
				}
				/* PAN :  b7 = L, b6 = R */
				OPN->pan[ c*2	] = (v & 0x80) ? ~0 : 0;
				OPN->pan[ c*2+1 ] = (v & 0x40) ? ~0 : 0;

			}
			break;
		}
		break;
	}
}


//static const struct ssg_callbacks psgintf =
//{
//	psg_set_clock,
//	psg_write,
//	psg_read,
//	psg_reset
//};
/*--[ Reset ]----------*/


/* prescaler set (and make time tables) */
static void OPNSetPres(FM_OPN *OPN , int pres , int TimerPres, int SSGpres)
{
	/* frequency base */
//	OPN->ST.freqbase_SSG = (OPN->ST.rate_SSG/*OPN?SSG*/) ? ((double)OPN->ST.clock / OPN->ST.rate_SSG/*OPN?SSG*/) / pres : 0;
	OPN->ST.freqbase_OPN = (OPN->ST.rate_OPN/*OPN?SSG*/) ? ((double)OPN->ST.clock / OPN->ST.rate_OPN/*OPN?SSG*/) / pres : 0;

#if 0
	OPN->ST.rate/*OPN?SSG*/ = (double)OPN->ST.clock / pres;
	OPN->ST.freqbase = 1.0;
#endif

	OPN->eg_timer_add  = (1<<EG_SH) *	OPN->ST.freqbase_OPN/*freqbase_SSG*//*OPN?SSG*/;/*???????????????*/
	OPN->eg_timer_overflow = ( 3 ) * (1<<EG_SH);

	/* Timer base time */
	OPN->ST.TimerBase = 1.0/((double)OPN->ST.clock / (double)TimerPres);

#if 0
	/* SSG part  prescaler set */
	if( SSGpres )
	{
	//	/*(*OPN->ST.SSG->set_clock)*/psg_set_clock( OPN->ST.param, OPN->ST.clock * 2 / SSGpres );
	//static void psg_set_clock(void *param, int clock)
	//{
//		struct ym2610_info *info = /*param*/OPN->ST.param;
//		int clock=(OPN->ST.clock * 2 / SSGpres);
	//	Zs_ay8910_set_clock_ym(info->psg, clock);
//		struct ym2610_info/*Z_AY8910*/ *PSG = info->psg;
//		SSG.UpdateStep = ((float)SSG_STEP * /*PSG->SampleRate*/machine->sample_rate *(8/((8000000/72)/44100))/* * 8*/ /*+clock/2*/) / clock;
//
		int clock=(OPN->ST.clock * 2 / SSGpres);
		SSG.UpdateStep = ((float)SSG_STEP * /*PSG->SampleRate*/machine->sample_rate *(8/((8000000/72)/44100))/* * 8*/ /*+clock/2*/) / clock;
//		SSG.UpdateStep = ((float)SSG_STEP * /*PSG->SampleRate*/machine->sample_rate *8  +clock/2 ) / clock;

	//}
	}
#endif
	/* SSG part  prescaler set */
	if (SSGpres) SSG.UpdateStep = ((double)SSG_STEP * OPN->ST.rate_OPN * 8) / (OPN->ST.clock * 2 / SSGpres);/* */


	/* make time tables */
	init_timetables( &OPN->ST, dt_tab );

	/* there are 2048 FNUMs that can be generated using FNUM/BLK registers
		but LFO works with one more bit of a precision so we really need 4096 elements */
	/* calculate fnumber -> increment counter table */
	int i;
	for(i = 0; i < 4096; i++)
	{
		/* freq table for octave 7 */
		/* OPN phase increment counter = 20bit */
		OPN->fn_table[i] = (UINT32)( (double)i * 32 * OPN->ST.freqbase_OPN/*SSG*//*OPN?SSG*/ * (1<<(FREQ_SH-10)) ); /* -10 because chip works with 10.10 fixed point, while we use 16.16 */
#if 0
		//logerror("FM.C: fn_table[%4i] = %08x (dec=%8i)\n", i, OPN->fn_table[i]>>6,OPN->fn_table[i]>>6 );
#endif
	}

	/* LFO freq. table */
	for(i = 0; i < 8; i++)
	{
		/* 8 LFO speed parameters */
		/* each value represents number of samples that one LFO level will last for */
		static const UINT32 lfo_samples_per_step[8] = {108, 77, 71, 67, 62, 44, 8, 5};
		/* Amplitude modulation: 64 output levels (triangle waveform); 1 level lasts for one of "lfo_samples_per_step" samples */
		/* Phase modulation: one entry from lfo_pm_output lasts for one of 4 * "lfo_samples_per_step" samples  */
		OPN->lfo_freq[i] = (1.0 / lfo_samples_per_step[i]) * (1<<LFO_SH) * OPN->ST.freqbase_OPN/*SSG*//*OPN?SSG*/;
#if 0
		//logerror("FM.C: lfo_freq[%i] = %08x (dec=%8i)\n", i, OPN->lfo_freq[i],OPN->lfo_freq[i] );
#endif
	}
}
static void psg_reset(void/*void *param*/)
//{
//	struct ym2610_info *info = param;
//	Zs_ay8910_reset_ym(/*info*//*info->psg*/);
//}
//static void Zs_ay8910_reset_ym(void/*void *chip*/)
{
//	struct ym2610_info/*Z_AY8910*/ *PSG = chip;
	int i;
//	PSG->register_latch = 0;
//	PSG->RNG = 1;
	SSG.OutputA = 0;
	SSG.OutputB = 0;
	SSG.OutputC = 0;
	SSG.OutputN = 0xff;
//	PSG->lastEnable = -1;	/* force a write */
	for (i = 0;i < SSG_PORTA;i++)
	{
	/*PSG->Regs*/YM2610.REGS[i] = 0x00;
		SSG_write(/*PSG,*/i,0x00); /* AYWriteReg() uses the timer system; we cannot */
								/* call it at this time because the timer system */
								/* has not been initialized. */
	}
//	PSG->ready = 1;
}
//static void ay8910_reset(u8 chip)
//{
//	ay8910_reset_ym(sndti_token(SOUND_AY8910, chip));
//}

/* ---------- reset one of chip ---------- */

void YM2610Reset/*Chip*/(/*void*/void/* *dummy_chip*//*void *chip*/)
{
	int i;
	//YM2610 *F2610 = chip;
	FM_OPN *OPN   = &/*F2610->*/YM2610.OPN;
	FM_ST *ST = &YM2610.OPN.ST;
	ADPCMB *adpcmb = &/*F2610->*/YM2610.deltaT;

	/* Reset Prescaler */
	OPNSetPres( OPN, 6*24, 6*24, 4*2); /* OPN 1/6 , SSG 1/4 */
	/* reset SSG section */
	/*(*OPN->ST.SSG->reset)*/psg_reset(/*OPN->ST.param*/);
	/* status clear */
	FM_IRQMASK_SET(ST/*&OPN->ST*/,0x03);
	FM_BUSY_CLEAR(ST/*&OPN->ST*/);
	OPNWriteMode(/*OPN,*/0x27,0x30); /* mode 0 , timer reset */

	OPN->eg_timer = 0;
	OPN->eg_cnt   = 0;

	FM_STATUS_RESET(ST/*&OPN->ST*/, 0xff);

	reset_channels(ST/*&OPN->ST*/, /*F2610->*/YM2610.CH , 6 );
	/* reset OPerator paramater */
	for(i = 0xb6 ; i >= 0xb4 ; i-- )
	{
		OPNWriteReg(/*OPN,*/i	   ,0xc0);
		OPNWriteReg(/*OPN,*/i|0x100,0xc0);
	}
	for(i = 0xb2 ; i >= 0x30 ; i-- )
	{
		OPNWriteReg(/*OPN,*/i	   ,0);
		OPNWriteReg(/*OPN,*/i|0x100,0);
	}
	for(i = 0x26 ; i >= 0x20 ; i-- ) OPNWriteReg(/*OPN,*/i,0);
	/**** ADPCM work initial ****/
	for( i = 0; i < 6 ; i++ ){
		/*F2610->*/YM2610.adpcm[i].step   = (UINT32)((float)(1<<ADPCM_SHIFT)*((float)/*F2610->*/YM2610.OPN.ST.freqbase_OPN/*OPN?SSG*/)/3.0);
		/*F2610->*/YM2610.adpcm[i].now_addr  = 0;
		/*F2610->*/YM2610.adpcm[i].now_step  = 0;
		/*F2610->*/YM2610.adpcm[i].start	  = 0;
		/*F2610->*/YM2610.adpcm[i].end	  = 0;
		/* /F2610->/YM2610.adpcm[i].delta	 = 21866; */
		/*F2610->*/YM2610.adpcm[i].vol_mul	 = 0;
		/*F2610->*/YM2610.adpcm[i].pan	  = &out_adpcm[OUTD_CENTER]; /* default center */
		/*F2610->*/YM2610.adpcm[i].flagMask  = 1<<i;
		/*F2610->*/YM2610.adpcm[i].flag   = 0;
		/*F2610->*/YM2610.adpcm[i].adpcm_acc = 0;
		/*F2610->*/YM2610.adpcm[i].adpcm_step= 0;
		/*F2610->*/YM2610.adpcm[i].adpcm_out = 0;
	}
	/*F2610->*/YM2610.adpcmTL = 0x3f;

	/*F2610->*/YM2610.adpcm_arrivedEndAddress = 0;

	/* DELTA-T unit */
	adpcmb->freqbase = OPN->ST.freqbase_OPN/*OPN?SSG*/;
	adpcmb->output_pointer = out_delta;
//	/*adpcmb->portshift*/8 = 8;		/* allways 8bits shift */
	adpcmb->output_range = 1<<23;
//	ADPCMB_ADPCM_Reset2610(DELTAT/*,OUTD_CENTER,ADPCMB_EMULATION_MODE_YM2610*/);
//static void ADPCMB_ADPCM_Reset2610(ADPCMB *adpcmb/*, int pan, int emulation_mode*/)
//{
	adpcmb->now_addr  = 0;
	adpcmb->now_step  = 0;
	adpcmb->step	  = 0;
	adpcmb->start	  = 0;
	adpcmb->end 	  = 0;
//	adpcmb->limit	  = ~0; /* this way YM2610 and Y8950 (both of which don't have limit address reg) will still work */
	adpcmb->volume	  = 0;
	adpcmb->pan 	  = &adpcmb->output_pointer[OUTD_CENTER/*pan*/];
	adpcmb->acc 	  = 0;
	adpcmb->prev_acc  = 0;
	adpcmb->adpcmd	  = 127;
	adpcmb->adpcml	  = 0;
//	adpcmb->emulation_mode = (UINT8)emulation_mode;
	adpcmb->portstate = /*(emulation_mode == ADPCMB_EMULATION_MODE_YM2610) ?*/ 0x20 /*: 0*/;
//1	adpcmb->control2  = /*(emulation_mode == ADPCMB_EMULATION_MODE_YM2610) ?*/ 0x01 /*: 0*/; /* default setting depends on the emulation mode. MSX demo called "facdemo_4" doesn't setup control2 register at all and still works */
//	/*adpcmb->DRAMportshift*/0 = 0/*dram_rightshift[adpcmb->control2 & 3]*/;

	/* The flag mask register disables the BRDY after the reset, however
	** as soon as the mask is enabled the flag needs to be set. */

	/* set BRDY bit in status register */
//	if(adpcmb->status_set_handler)
//		if(adpcmb->status_change_BRDY_bit)
//			(adpcmb->status_set_handler)(/*adpcmb->status_change_which_chip,*/ adpcmb->status_change_BRDY_bit);
//	if(adpcmb->status_change_BRDY_bit)
//	{	/*F2610->*/YM2610.adpcm_arrivedEndAddress |= adpcmb->status_change_BRDY_bit;}
//}
}






/* YM2610 write */
/* n = number  */
/* a = address */
/* v = value   */
WRITE8_HANDLER( YM2610_w )
//static int YM2610Write00(/*void *chip_dummy,*/ int a, UINT8 data/*v*/)
{
	//YM2610 *F2610 = chip;
	FM_OPN *OPN   = &/*F2610->*/YM2610.OPN;
	int addr;
//	/*v*/data &= 0xff;	/* adjust to 8 bit bus */

	switch( /*a*/offset&3 )
	{
	case 0: /* address port 0 */
		OPN->ST.address = data/*v*/;
		/*F2610->*/YM2610.addr_A1 = 0;
		#if 0
		/* HARDWARE_BUG??? */
		/* Write register to SSG emulator */
		if( /*v*/data < 16 ) /*(*OPN->ST.SSG->write)*//*psg_write*/SSG_write(/*OPN->ST.param,*/0,data/*v*/);
		/*  (マニュアルどうりなら、こういう動作は無い筈だが実際のところ)良く解からない。 */
		#endif
		break;

	case 1: /* data port 0	  */
		if (/*F2610->*/YM2610.addr_A1 != 0)
			break;	/* verified on real YM2608 */

		addr = OPN->ST.address;
//#ifdef _STATE_H
		/*F2610->*/YM2610.REGS[addr] = data/*v*/; /* stock data */
//#endif
		//dataport0_write_2610(addr,data);
//static void dataport0_write_2610(int addr, UINT8 data)
{
		switch(addr & 0xf0)
		{
		case 0x00:	/* SSG section */
			/* Write data to SSG emulator */
		//	/*(*OPN->ST.SSG->write)*//*psg_write*/SSG_write(/*OPN->ST.param,*//*a*/offset,data/*v*/);
			{
			//struct ym2610_info/*Z_AY8910*/ *PSG = OPN->ST.param;
				SSG_write(/*PSG*//*OPN->ST.param,*//*offset*/addr,data/*v*/);
			}
			break;
		case 0x10: /* DeltaT ADPCM */
#if 1
			#if 0
			YM2610UpdateReq(OPN->ST.param);/*ここは、要らないっぽい*/
			#endif
			OPNB_ADPCMB_write(&/*F2610->*/YM2610.deltaT,addr/*-0x10*/,data/*v*/);
#else
			switch(addr)
			{
		//	case 0x10:	/* control 1 */
		//	case 0x11:	/* control 2 */
		//	case 0x12:	/* start address L */
		//	case 0x13:	/* start address H */
		//	case 0x14:	/* stop address L */
		//	case 0x15:	/* stop address H */

		//	case 0x19:	/* delta-n L */
		//	case 0x1a:	/* delta-n H */
		//	case 0x1b:	/* volume */
			#if 0
			case 0x10 ... 0x15:
			case 0x19 ... 0x1b:
			case 0x1c:
			#else
			case 0x10 ... 0x1c:
			#endif
				{
				}
				break;
			}
#endif
			break;
		case 0x20:	/* Mode Register */
			#if 0
			YM2610UpdateReq(OPN->ST.param);/*テスト無効*/
			#endif
			OPNWriteMode(/*OPN,*/addr,data/*v*/);
			break;
		default:	/* OPN section */
			#if 0
			YM2610UpdateReq(OPN->ST.param);/*テスト無効*/
			#endif
			/* write register */
			OPNWriteReg(/*OPN,*/addr,data/*v*/);
		}
}
		break;

	case 2: /* address port 1 */
		OPN->ST.address = data/*v*/;
		/*F2610->*/YM2610.addr_A1 = 1;
		break;

	case 3: /* data port 1	  */
		if (/*F2610->*/YM2610.addr_A1 != 1)
			break;	/* verified on real YM2608 */
		#if 0
		YM2610UpdateReq(OPN->ST.param);/*テスト無効*/
		#endif
		addr = OPN->ST.address | 0x100;
//#ifdef _STATE_H
		/*F2610->*/YM2610.REGS[addr/* | 0x100*/] = data/*v*/;
//#endif
		if( addr < 0x130 )
			/* 100-12f : ADPCM A section */
		{	OPNB_ADPCMA_write(/*F2610,*/addr/*&0xff*/, data/*v*/);}
		else
		{	OPNWriteReg(/*OPN,*/addr, data/*v*/);}
	}
//	return OPN->ST.irq;
}



READ8_HANDLER( YM2610_r )
//static UINT8 YM2610Read00(/*void *dummy_chip,*/int a)
{
	//YM2610 *F2610 = chip;
	int addr = /*F2610->*/YM2610.OPN.ST.address;
//	UINT8 ret = 0;
	switch(offset&3)
	{
	case 0: /* status 0 : YM2203 compatible */
		/*ret =*/return FM_STATUS_FLAG(&/*F2610->*/YM2610.OPN.ST) & 0x83;
		break;
	case 2: /* status 1 : ADPCM status */
		/* ADPCM STATUS (arrived End Address) */
		/* B,--,A5,A4,A3,A2,A1,A0 */
		/* B	 = ADPCM-B(DELTA-T) arrived end address */
		/* A0-A5 = ADPCM-A			arrived end address */
		/*ret =*/return /*F2610->*/YM2610.adpcm_arrivedEndAddress;
		break;
//	case 3:
//		ret = 0;
//		break;
//	case 3:
//	case 1: /* data 0 */
	default:
		if( addr < /*16*/14/*SSG_PORTA*/ )
		{
		//	/*ret =*/return /*(*  F2610->  YM2610.OPN.ST.SSG->read)*/psg_read(/*F2610->*/YM2610.OPN.ST.param);
			return /*PSG->Regs*/YM2610.REGS[addr];
		}
		else if( addr == 0xff ) /*ret =*/return 0x01;
		break;
	}
//	return ret;
	return 0;
}

#if 0
static int zYM2610TimerOver(/*void *chip,*/int c)
{
	//YM2610 *F2610 = chip;

	if( c )
	{
		/* Timer B */
		TimerBOver( &(/*F2610->*/YM2610.OPN.ST) );
	}
	else
	{
		#if 0
		YM2610UpdateReq(/*F2610->*/YM2610.OPN.ST.param);/*テスト無効*/
		#endif
		/* Timer A */
		/* timer update */
		TimerAOver( &(/*F2610->*/YM2610.OPN.ST) );
		/* CSM mode key,TL controll */
		if( /*F2610->*/YM2610.OPN.ST.mode & 0x80 )
		{	/* CSM mode total level latch and auto key on */
			CSMKeyControll( &(/*F2610->*/YM2610.CH[2]) );
		}
	}
	return /*F2610->*/YM2610.OPN.ST.irq;
}
#endif
/* Timer overflow callback from timer.c */
static void timer_callback_0(void *param)		/* Timer A */
{
//	struct ym2610_info *info = param;
//	zYM2610TimerOver(/*info->chip,*/0);
	{
		#if 0
		YM2610UpdateReq(/*F2610->*/YM2610.OPN.ST.param);/*テスト無効*/
		#endif
		/* Timer A */
		/* timer update */
		TimerAOver( &(/*F2610->*/YM2610.OPN.ST) );
		/* CSM mode key,TL controll */
		if( /*F2610->*/YM2610.OPN.ST.mode & 0x80 )
		{	/* CSM mode total level latch and auto key on */
			CSMKeyControll( &(/*F2610->*/YM2610.CH[2]) );
		}
	}
}

static void timer_callback_1(void *param)		/* Timer B */
{
//	struct ym2610_info *info = param;
//	zYM2610TimerOver(/*info->chip,*/1);
	{
		/* Timer B */
		TimerBOver( &(/*F2610->*/YM2610.OPN.ST) );
	}
}
#endif /* (BUILD_YM2610||BUILD_YM2610B) */


////////////////


#if BUILD_YM2610

//static int sound_type = SOUND_YM2610;







/*------------------------- TM2610 -------------------------------*/
#if 0
/* IRQ Handler */
static void IRQHandler(void *param,int irq)
{
	struct ym2610_info *info = param;
	if(info->intf->handler) info->intf->handler(irq);
}
#endif


/* TimerHandler from fm.c */
static void TimerHandler(/*void *param,*/int c,int count,double stepTime)
{
//	struct ym2610_info *info = param;
	if( count == 0 )
	{	/* Reset FM Timer */
		timer_enable(/*info*/YM2610.my_ym2610_info_aaa_timer[c], 0);
	}
	else
	{	/* Start FM Timer */
		double timeSec = (double)count * stepTime;
		if (!timer_enable(/*info*/YM2610.my_ym2610_info_aaa_timer[c], 1))
		{
			timer_adjust_ptr(/*info*/YM2610.my_ym2610_info_aaa_timer[c], timeSec, 0/*info*//*&my_ym2610_info_aaa*/, 0);
		}
	}
}

#if 0
/* update request from fm.c */
void YM2610UpdateRequest(void *param)/*テスト無効*/
{
	struct ym2610_info *info = param;
	stream_update(info->stream,100);
}
#endif



//#ifdef _STATE_H
//static void ym2610_postload(void)
//{
//	u8 num;
//	for (num=0; num < MAX_SOUND; num++)
//	{
//		struct ym2610_info *info = sndti_token(SOUND_YM2610, num);
//		if (info)
//			YM2610Postload(info->chip);
//	}
//}
//#endif


//static void *ym2610_start(/*int*/UINT8 sndindex, int clock, const void *config)
static void YM2610Init(int clock, int rate/*, FM_IRQHANDLER_2610 IRQHandler*/)
{
/* machine->drv->sound[0].configが 0 なのは、
	0(つまり始めに登録) がYM2610だから。
	詳しくは src/drivers/metal_black.c を参照せよ。 */
#define REGIST_ON_DRIVER_THE_2610_SOUND_NUMBER (0)
	const struct YM2610interface *intf = machine->drv->sound[REGIST_ON_DRIVER_THE_2610_SOUND_NUMBER].config;
#undef  REGIST_ON_DRIVER_THE_2610_SOUND_NUMBER

//const void *config=0;
//	static const struct YM2610interface generic_2610 = { 0 };
//	const struct YM2610interface *intf = config ? config : &generic_2610;
//	int rate = /*(clock/72)*/CLOCK_DIV72/*machine->sample_rate*/;
//	void *pcmbufa,*pcmbufb;
//	int  pcmsizea,pcmsizeb;

//	sound_type = SOUND_YM2610;


//	memset(my_ym2610_info_aaa, 0, sizeof(*my_ym2610_info_aaa));
//	memset(&my_ym2610_info_aaa, 0, sizeof(my_ym2610_info_aaa));

//	my_ym2610_info_aaa->intf = intf;


	//YM2610 *F2610;
	//cur_chip = NULL;	/* hiro-shi!! */
	/* allocate extend state space */
	//if( (F2610 = (YM2610 *)malloc(sizeof(YM2610)))==NULL) return NULL;
	/* clear */
	memset(/*F2610*/&YM2610,0,sizeof(YM2610));
	/* allocate total level table (128kb space) */
	init_tables();
	//if( !init_tables() )
	//{
	//	//free( F2610 );
	//	return NULL;	/* error */
	//}


//	my_ym2610_info_aaa->psg = Zs_ay8910_start_ym(SOUND_YM2610, sndindex, clock, 1/*, NULL, NULL, NULL, NULL*/);
//static void *Zs_ay8910_start_ym(
//	int chip_type=SOUND_YM2610;
//	int sndindex=sndindex;
//	int clock=;
//	int streams=;
//	,read8_handler portAread, read8_handler portBread
//	,write8_handler portAwrite, write8_handler portBwrite
//)
{

//	struct ym2610_info/*Z_AY8910*/ *info_psg;
//	info_psg = auto_malloc(sizeof(*info_psg));
//	memset(info_psg, 0, sizeof(*info_psg));
//	my_ym2610_info_aaa/*_psg*/->index = sndindex;
//	my_ym2610_info_aaa/*_psg*/->streams = 1/*streams*/;
{
#if 0
	struct ym2610_info/*Z_AY8910*/ *PSG=info_psg;
#endif
// causes crashes with YM2610 games - overflow?
//	if (options.use_filter) sample_rate = clock/8;

//	PSG->SampleRate = sample_rate;//==machine->sample_rate
#if 0
	PSG->Channel = stream_create(0,1/*streams*/,/*sample_rate*/machine->sample_rate,PSG,Z_AY8910_Update);
#endif
//	Zs_ay8910_set_clock_ym(PSG,clock);
//	SSG.UpdateStep = ((float)SSG_STEP * /*PSG->SampleRate*/machine->sample_rate *(8/((8000000/72)/44100))/* * 8*/ /*+clock/2*/) / clock;
}
//	build_mixer_table(info_psg);
//	#if 0
//	Z_AY8910_statesave(info_psg, sndindex);
//	#endif
//	return info_psg;
//	info->psg = info_psg;
}
	//if (!info->psg) return NULL;

	/* Timer Handler set */
	/*info*/YM2610.my_ym2610_info_aaa_timer[0] =timer_alloc_ptr(timer_callback_0);
	/*info*/YM2610.my_ym2610_info_aaa_timer[1] =timer_alloc_ptr(timer_callback_1);

	/* stream system initialize */
	#if 0
	my_ym2610_info_aaa->stream = stream_create(0,2,/*rate*//*(clock/72)*//*CLOCK_DIV72_OPN*/CLOCK_DIV72_OPN_STREAM/*machine->sample_rate*/,info,ym2610_stream_update);
	#endif
	/* setup adpcm buffers */

//	/**** initialize YM2610 ****/
//	/*my_ym2610_info_aaa->chip =*/ YM2610Init_bbb(info,sndindex,clock
//			//,/*rate*/machine->sample_rate
//			,pcmbufa,pcmsizea,pcmbufb,pcmsizeb
//			,TimerHandler,IRQHandler/*,&psgintf*/);
//static void *YM2610Init_bbb(
//	void *param
//	,int index
//	,int clock
////	,int rate//==machine->sample_rate
//	,void *pcmroma
//	,int pcmsizea
//	,void *pcmromb
//	,int pcmsizeb
//	,FM_TIMERHANDLER TimerHandler
//	,FM_IRQHANDLER IRQHandler
//	/*, const struct ssg_callbacks *ssg*/
//)
//{
int rate_OPN =/*(clock/72)*/CLOCK_DIV72_OPN/*machine->sample_rate*/;
//int rate_SSG =/*(clock/72)*/CLOCK_DIV72_SSG/*machine->sample_rate*/;
#ifdef RENDER_DIV_TYPE
	rate_OPN /=(RENDER_DIV_TYPE);/* cheat div2 render. */
//	rate_SSG /=(RENDER_DIV_TYPE);/* cheat div2 render. */
#endif
	/* FM */
//	/*F2610->*/YM2610.OPN.ST.param = info/*info*//*my_ym2610_info_aaa*//*param*/;
	/*F2610->*/YM2610.OPN.type = TYPE_YM2610;
	/*F2610->*/YM2610.OPN.P_CH = /*F2610->*/YM2610.CH;
	/*F2610->*/YM2610.OPN.ST.clock = clock;
	/*F2610->*/YM2610.OPN.ST.rate_OPN = rate_OPN;
//	/*F2610->*/YM2610.OPN.ST.rate_SSG = rate_SSG;
	/* Extend handler */
	/*F2610->*/YM2610.OPN.ST.Timer_Handler	= TimerHandler;
	/*F2610->*/YM2610.OPN.ST.IRQ_Handler	= intf->handler/*IRQHandler*/;
//	/*F2610->*/YM2610.OPN.ST.SSG			= ssg;

	/* setup adpcm rom address */
	/* ADPCM */
	/*F2610->*/YM2610.pcmbufA  /*= F2610-> YM2610.pcmbuf; 	F2610-> YM2610.pcmbuf	*/	 = (UINT8 *)/*pcmroma;	pcmbufa  = (void *)*/(memory_region(/*my_ym2610_info_aaa->*//*info->*/intf->pcmroma));
	/*F2610->*/YM2610.pcmsizeA /*= F2610-> YM2610.pcm_size;	F2610-> YM2610.pcm_size	*/	 = /*pcmsizea;	pcmsizea =*/		   memory_region_length(/*my_ym2610_info_aaa->*//*info->*/intf->pcmroma);
	/* DELTA-T */
	/*F2610->*/YM2610.deltaT.pcmbufB/*memory*/ 	 	 = (UINT8 *)/*pcmromb;	pcmbufb  = (void *)*/(memory_region(/*my_ym2610_info_aaa->*//*info->*/intf->pcmromb));
	/*F2610->*/YM2610.deltaT.pcmsizeB/*memory_size*/ = /*pcmsizeb;	pcmsizeb =*/		   memory_region_length(/*my_ym2610_info_aaa->*//*info->*/intf->pcmromb);

//	/*F2610->*/YM2610.deltaT.status_set_handler = YM2610_deltat_status_set;
//	/*F2610->*/YM2610.deltaT.status_reset_handler = YM2610_deltat_status_reset;
//	/*F2610->*/YM2610.deltaT.status_change_which_chip = F2610;
	/*F2610->*/YM2610.deltaT.status_change_EOS_bit = 0x80; /* status flag: set bit7 on End Of Sample */



	YM2610Reset/*Chip*/(/*F2610*//*0*//*dummy*/);


	Init_ADPCMATable();
//#if 0
//#ifdef _STATE_H
//	YM2610_save_state(F2610, sndindex/*index*/);
//#endif
//#endif
	//return /*1*/&YM2610/*F2610*/;
//}

//	/*my_ym2610_info_aaa->*//*info->*/info->chip = &YM2610;

//#ifdef _STATE_H
//	if (sndindex == 0)
//		state_save_register_func_postload(ym2610_postload);
//#endif

//	if (info->chip)
//		return info;

	/* error */
//	return NULL;
}


//static void ym2610_stop(void *token)
//{
	//struct ym2610_info *info = token;
	//YM2610Shutdown(info->chip);
	/* shut down emulator */
	//void YM2610Shutdown(void *chip_dummy)
	//{
	//YM2610 *F2610 = chip;

	//FMCloseTable();
	//static void FMCloseTable( void )
	//{
	//#ifdef SAVE_SAMPLE
	//	fclose(sample[0]);
	//#endif
	//	return;
	//}
	//free(F2610);
	//}

	//Z_ay8910_stop_ym(info->psg);
//}

//static void ym2610_reset(void *token)
//{
////	struct ym2610_info *info = token;
//	YM2610Reset/*Chip*/(/*info->chip*/);
//}

#if 0
/************************************************/
/* Status Read for YM2610 - Chip 0				*/
/************************************************/
READ8_HANDLER( YM2610_status_port_0_A_r )
{
//logerror("PC %04x: 2610 S0A=%02X\n",activecpu_get_pc(),YM2610Read(sndti_token(sound_type,0,0));
	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610Read00(/*info->chip,*/0);
}

READ8_HANDLER( YM2610_status_port_0_B_r )
{
//logerror("PC %04x: 2610 S0B=%02X\n",activecpu_get_pc(),YM2610Read(sndti_token(sound_type,0,2));
	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610Read00(/*info->chip,*/2);
}

/************************************************/
/* Port Read for YM2610 - Chip 0				*/
/************************************************/
READ8_HANDLER( YM2610_read_port_0_r ){
	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610Read00(/*info->chip,*/1);
}

/************************************************/
/* Control Write for YM2610 - Chip 0			*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE8_HANDLER( YM2610_control_port_0_A_w )
{
//logerror("PC %04x: 2610 Reg A %02X",activecpu_get_pc(),data);
	struct ym2610_info *info = sndti_token(sound_type,0);
	YM2610Write(info->chip,0,data);
}

WRITE8_HANDLER( YM2610_control_port_0_B_w )
{
//logerror("PC %04x: 2610 Reg B %02X",activecpu_get_pc(),data);
	struct ym2610_info *info = sndti_token(sound_type,0);
	YM2610Write(info->chip,2,data);
}

/************************************************/
/* Data Write for YM2610 - Chip 0				*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE8_HANDLER( YM2610_data_port_0_A_w )
{
//logerror(" =%02X\n",data);
	struct ym2610_info *info = sndti_token(sound_type,0);
	YM2610Write(info->chip,1,data);
}

WRITE8_HANDLER( YM2610_data_port_0_B_w )
{
//logerror(" =%02X\n",data);
	struct ym2610_info *info = sndti_token(sound_type,0);
	YM2610Write(info->chip,3,data);
}
#else

#if 0
READ8_HANDLER( YM2610_r )
{
//	struct ym2610_info *info = sndti_token(sound_type,0);
	#if 0
	switch (offset)
	{
	case 0:	//READ8_HANDLER( YM2610_status_port_0_A_r )
			return YM2610Read(info->chip,0);
	case 2:	//READ8_HANDLER( YM2610_status_port_0_B_r )
			return YM2610Read(info->chip,2);
//	case 1:
//	case 3:	//READ8_HANDLER( YM2610_read_port_0_r )
	default:
			return YM2610Read(info->chip,1);
	}
	#else
			return YM2610Read00(/*info->chip,*/offset);
	#endif
}
#endif

#if 0
WRITE8_HANDLER( YM2610_w )
{
//	struct ym2610_info *info = sndti_token(sound_type,0);
	#if 0
	switch (offset)
	{
	case 0:	//WRITE8_HANDLER( YM2610_control_port_0_A_w )
			YM2610Write(info->chip,0,data);	break;
	case 1:	//WRITE8_HANDLER( YM2610_data_port_0_A_w )
			YM2610Write(info->chip,1,data);	break;
	case 2:	//WRITE8_HANDLER( YM2610_control_port_0_B_w )
			YM2610Write(info->chip,2,data);	break;
	case 3:	//WRITE8_HANDLER( YM2610_data_port_0_B_w )
			YM2610Write(info->chip,3,data);	break;
	}
	#else
			YM2610Write00(/*info->chip,*/offset,data);
	#endif
}
#endif
#endif

///////////////////////


#if 0//(1==USE_YM2610_LSB0_INTERFACE)
/* MAMEでは taitoZ 以外に無い。 */
/*taito_z only*/
/* TAITO Z SYSTEM 専用 */
/************************************************/
/* Status Read for YM2610 - Chip 0				*/
/************************************************/
READ16_HANDLER( YM2610_status_port_0_A_lsb_r )
{
//logerror("PC %04x: 2610 S0A=%02X\n",activecpu_get_pc(),YM2610Read(sndti_token(sound_type,0,0));
//	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610_r(0);//YM2610Read(info->chip,0);
}
READ16_HANDLER( YM2610_status_port_0_B_lsb_r )
{
//logerror("PC %04x: 2610 S0B=%02X\n",activecpu_get_pc(),YM2610Read(sndti_token(sound_type,0,2));
//	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610_r(2);//YM2610Read(info->chip,2);
}
/************************************************/
/* Port Read for YM2610 - Chip 0				*/
/************************************************/
READ16_HANDLER( YM2610_read_port_0_lsb_r ){
//	struct ym2610_info *info = sndti_token(sound_type,0);
	return YM2610_r(1);//YM2610Read(info->chip,1);
}
/************************************************/
/* Control Write for YM2610 - Chip 0			*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE16_HANDLER( YM2610_control_port_0_A_lsb_w )
{
//logerror("PC %04x: 2610 Reg A %02X",activecpu_get_pc(),data);
	if (ACCESSING_LSB)
	{
	//	struct ym2610_info *info = sndti_token(sound_type,0);
		YM2610_w(0,data);//YM2610Write(info->chip,0,data);
	}
}
WRITE16_HANDLER( YM2610_control_port_0_B_lsb_w )
{
//logerror("PC %04x: 2610 Reg B %02X",activecpu_get_pc(),data);
	if (ACCESSING_LSB)
	{
	//	struct ym2610_info *info = sndti_token(sound_type,0);
		YM2610_w(2,data);//YM2610Write(info->chip,2,data);
	}
}
/************************************************/
/* Data Write for YM2610 - Chip 0				*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE16_HANDLER( YM2610_data_port_0_A_lsb_w )
{
//logerror(" =%02X\n",data);
	if (ACCESSING_LSB)
	{
	//	struct ym2610_info *info = sndti_token(sound_type,0);
		YM2610_w(1,data);//YM2610Write(info->chip,1,data);
	}
}
WRITE16_HANDLER( YM2610_data_port_0_B_lsb_w )
{
//logerror(" =%02X\n",data);
	if (ACCESSING_LSB)
	{
	//	struct ym2610_info *info = sndti_token(sound_type,0);
		YM2610_w(3,data);//YM2610Write(info->chip,3,data);
	}
}
#endif //(1==USE_YM2610_LSB0_INTERFACE)




#if 0
/* MAMEには(2610が複数ある物は)存在しない。(ってゆーか「世の中」になさそう) */

/************************************************/
/* Status Read for YM2610 - Chip 1				*/
/************************************************/
READ8_HANDLER( YM2610_status_port_1_A_r ) {
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,0);
}
READ8_HANDLER( YM2610_status_port_1_B_r ) {
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,2);
}
/************************************************/
/* Port Read for YM2610 - Chip 1				*/
/************************************************/
READ8_HANDLER( YM2610_read_port_1_r ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,1);
}
/************************************************/
/* Control Write for YM2610 - Chip 1			*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE8_HANDLER( YM2610_control_port_1_A_w ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	YM2610Write(info->chip,0,data);
}
WRITE8_HANDLER( YM2610_control_port_1_B_w ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	YM2610Write(info->chip,2,data);
}
/************************************************/
/* Data Write for YM2610 - Chip 1				*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE8_HANDLER( YM2610_data_port_1_A_w ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	YM2610Write(info->chip,1,data);
}
WRITE8_HANDLER( YM2610_data_port_1_B_w ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	YM2610Write(info->chip,3,data);
}

/////////

/************************************************/
/* Status Read for YM2610 - Chip 1				*/
/************************************************/
READ16_HANDLER( YM2610_status_port_1_A_lsb_r ) {
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,0);
}
READ16_HANDLER( YM2610_status_port_1_B_lsb_r ) {
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,2);
}
/************************************************/
/* Port Read for YM2610 - Chip 1				*/
/************************************************/
READ16_HANDLER( YM2610_read_port_1_lsb_r ){
	struct ym2610_info *info = sndti_token(sound_type,1);
	return YM2610Read(info->chip,1);
}
/************************************************/
/* Control Write for YM2610 - Chip 1			*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE16_HANDLER( YM2610_control_port_1_A_lsb_w ){
	if (ACCESSING_LSB)
	{
		struct ym2610_info *info = sndti_token(sound_type,1);
		YM2610Write(info->chip,0,data);
	}
}
WRITE16_HANDLER( YM2610_control_port_1_B_lsb_w ){
	if (ACCESSING_LSB)
	{
		struct ym2610_info *info = sndti_token(sound_type,1);
		YM2610Write(info->chip,2,data);
	}
}
/************************************************/
/* Data Write for YM2610 - Chip 1				*/
/* Consists of 2 addresses						*/
/************************************************/
WRITE16_HANDLER( YM2610_data_port_1_A_lsb_w ){
	if (ACCESSING_LSB)
	{
		struct ym2610_info *info = sndti_token(sound_type,1);
		YM2610Write(info->chip,1,data);
	}
}
WRITE16_HANDLER( YM2610_data_port_1_B_lsb_w ){
	if (ACCESSING_LSB)
	{
		struct ym2610_info *info = sndti_token(sound_type,1);
		YM2610Write(info->chip,3,data);
	}
}
#endif // 0


#endif //BUILD_YM2610

/********************************/

#if 0//BUILD_YM2610/*2610only*/
void ym2610_get_info(void *token, UINT32 state, union sndinfo *info)
{
	switch (state)
	{
		/* --- the following bits of info are returned as pointers to data or functions --- */
		case SNDINFO_PTR_START: 						info->start = ym2610_start; 			break;
		case SNDINFO_PTR_STOP:							info->stop  = ym2610_stop;				break;
		case SNDINFO_PTR_RESET: 						info->reset = ym2610_reset; 			break;

		/* --- the following bits of info are returned as NULL-terminated strings --- */
		case SNDINFO_STR_NAME:							info->s = "YM2610"; 					break;
//		case SNDINFO_STR_CORE_FAMILY:					info->s = "Yamaha FM";					break;
//		case SNDINFO_STR_CORE_VERSION:					info->s = "1.0";						break;
//		case SNDINFO_STR_CORE_CREDITS:					info->s = "Copyright (c) 2004, The MAME Team"; break;
	}
}
#endif //BUILD_YM2610/*2610only*/











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

//WRITE8_HANDLER( SegaPCM_w ){}
//READ8_HANDLER( SegaPCM_r ){}

/*------------------------------------------------------
	Reset YM2151 emulation
 -----------------------------------------------------*/



//============================================================
//
//	sndintrf.c - Sound interfece
//
//============================================================

//#include "psp_main.h"
//#include "driver.h"
//#include "sound/ns86snd.h"


static void ns86_sound_update(int p)
{
	int i;
	s16 *buffer = (s16 *)p;
	s16 mixing_buffer[PSP_SOUND_SAMPLES];

	if(SND_44100==(sr))
	{
		YM2610Update_stream(mixing_buffer, PSP_SOUND_SAMPLES);
		for (i = 0; i < PSP_SOUND_SAMPLES; i++)
		{
			*buffer++ = mixing_buffer[i];
		}
	}
	else
	{
		YM2610Update_stream(mixing_buffer, sr_length);
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
			default:
			case SND_22050: 	*buffer++ = v;/*not_break;*//*d=368*/
		/*	case SND_44100:*/	*buffer++ = v;/*not_break;*//*d=735*//*735[samples/flame]==44100[samples/sec]/60[flame] */
			}
		}
	}
}


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

	psp_sound->stack	= 0x10000;
	psp_sound->stereo	= 0;
	psp_sound->callback = ns86_sound_update;

//


//	YM2151Init(3579545, samplerate, NULL);
	YM2610Init(8000000, samplerate/*, NULL*/);// FZもoutrunも4[MHz]
//#if 0
//	namco_cus30_init(24000, samplerate, 8);
//	n63701x_init(6000000, samplerate);
//#endif
//#if (0==NO_SEGAPCM)
//	segapcm_start(15625, samplerate);
//#endif //(0==NO_SEGAPCM)
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
	YM2610Reset();
//	n63701x_reset();
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
#if (1==LINK_MAME_SOUND)
int sndnum_clock(int sndnum)
{
#if 0
	switch(sndnum)
	{
	case 0:	return 8000000;	/* 8.000[MHz] */  // FZもoutrunも4[MHz]
//	case 1:	return 15625;	/* 15.625[kHz] */
//	case 2:	return 99999;
//	case 3:	return 12345;
//	case 4:	return 77777;
	}
	return 0;
#else
	return 8000000;	/* 8.000[MHz] */  // FZもoutrunも4[MHz]
#endif
}
const char *sndnum_get_info_string(int sndnum, UINT32 state)
{
#if 0
	switch(sndnum)
	{
	case 0:	return "YM2610";	// FZもoutrunも2151.
//	case 1:	return "SEGA PCM";	// outrunのみ.
//	case 2:	return "TEST 99999";
//	case 3:	return "TEST 12345";
//	case 4:	return "TEST 77777";
	}
	return ""/*"dummy_STR"*/;
#else
	return "YM2610";	// FZもoutrunも2151.
#endif
}
//////よくワカンナイ。
#if 0
void sndti_set_output_gain(int type, snd_index_t index, int output, float gain)	/* 音量コントロール */
{
//	int sndnum = sound_matrix[type][index] - 1;
//	if (sndnum < 0)
//	{
//		//logerror("sndti_set_output_gain called for invalid sound type %d, index %d\n", type, index);
//		return;
//	}
//	if (output >= sound[sndnum].outputs)
//	{
//		//logerror("sndti_set_output_gain called for invalid sound output %d (type %d, index %d)\n", output, type, index);
//		return;
//	}
//	stream_set_output_gain(sound[sndnum].output[output].stream, sound[sndnum].output[output].output, gain);
}
#endif //0
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
#endif //0

#endif // (1==LINK_MAME_SOUND)




#endif // _METAL_BLACK_SOUND_H_
