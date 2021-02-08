//============================================================
//
//	psp_main.h - PSP main
//
//============================================================

#ifndef _PSP_MAIN_H_
#define _PSP_MAIN_H_

#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
//#include <sys/unistd.h>

/* use PSP_ALIGN_DATA in c6809 */

#ifndef ALIGN_N_DATA
	//#define ALIGN_N_DATA __attribute__((aligned(64)))
	#define ALIGN_N_DATA(nnn) __attribute__((aligned(nnn)))
#endif // ALIGN_N_DATA

#define PSP_ALIGN_DATA ALIGN_N_DATA(64)

//#include "osd_cpu.h"
//#include "osdepend.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#include "psp_ticker.h"
#include "psp_file.h"
#include "psp_font.h"
#include "psp_sound.h"
#include "psp_video.h"
#include "psp_menu.h"

extern volatile int psp_loop;
extern volatile int psp_sleep;
extern volatile int psp_rajar;

#define HANKAKU10_OFFS 0x00
extern const unsigned char hankaku_font5x10[];
extern const unsigned short zenkaku_font10[];


/* 1 USE_PSP_SLEEP むむむ、本当は１なんだケド。。。 */
#define USE_PSP_SLEEP 0




#endif /* _PSP_MAIN_H_ */
