//============================================================
//
//	ticker.c - MSDOS timing code
//
//============================================================

#include "driver.h"
#include <time.h>
#include "misc.h"


//============================================================
//	PROTOTYPES
//============================================================

static cycles_t uclock_cycle_counter(void);
static cycles_t nop_cycle_counter(void);



//============================================================
//	GLOBAL VARIABLES
//============================================================

// global cycle_counter function and divider
cycles_t		(*cycle_counter)(void) = nop_cycle_counter;
cycles_t		(*ticks_counter)(void) = nop_cycle_counter;
cycles_t		cycles_per_sec;


//============================================================
//	init_ticker
//============================================================

void init_ticker(void)
{
	cycle_counter = uclock_cycle_counter;
	ticks_counter = nop_cycle_counter;
	logerror("using uclock for timing ... ");

	cycles_per_sec = UCLOCKS_PER_SEC;

	// log the results
	logerror("cycles/second = %u\n", (int)cycles_per_sec);
}

//============================================================
//	uclock_cycle_counter
//============================================================

static cycles_t uclock_cycle_counter( void )
{
	/* this assumes that uclock_t is 64-bit (which it is) */
	return uclock();
}



//============================================================
//	nop_cycle_counter
//============================================================

static cycles_t nop_cycle_counter(void)
{
	return 0;
}



//============================================================
//	osd_cycles
//============================================================

cycles_t osd_cycles(void)
{
	return (*cycle_counter)();
}



//============================================================
//	osd_cycles_per_second
//============================================================

cycles_t osd_cycles_per_second( void )
{
	return cycles_per_sec;
}



//============================================================
//	osd_profiling_ticks
//============================================================

cycles_t osd_profiling_ticks(void)
{
	return (*ticks_counter)();
}
