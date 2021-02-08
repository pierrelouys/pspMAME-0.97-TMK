//============================================================
//
//	psp_ticker.c - PSP timing code
//
//============================================================

#include "driver.h"


//============================================================
//	psp_tick_resolution
//============================================================

//UINT64 psp_tick_resolution(void)
//{
//	return sceRtcGetTickResolution();
//}



//============================================================
//	psp_ticker
//============================================================

u64 psp_ticker(void)
{
	u64 current_ticks;

	sceRtcGetCurrentTick(&current_ticks);
	return current_ticks;
}
