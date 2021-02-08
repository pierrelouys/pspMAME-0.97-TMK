#include "driver.h"
#include <signal.h>
#include <time.h>
#include <ctype.h>
#include "ticker.h"

int  psp_init_sound(void);
void psp_init_input(void);
void psp_shutdown_sound(void);
void psp_shutdown_input(void);
extern void cli_frontend_exit( void );
extern int cli_frontend_init( int argc, char **argv );


/* put here anything you need to do when the program is started. Return 0 if */
/* initialization was successful, nonzero otherwise. */
int osd_init(void)
{
	if (psp_init_sound())
	{
		return 1;
	}
	psp_init_input();

	return 0;
}


/* put here cleanup routines to be executed when the program is terminated. */
void osd_exit(void)
{
	psp_shutdown_sound();
	psp_shutdown_input();
}



//============================================================
//	osd_alloc_executable
//============================================================

void *osd_alloc_executable(size_t size)
{
	return malloc( size );
}



//============================================================
//	osd_free_executable
//============================================================

void osd_free_executable(void *ptr)
{
	free( ptr );
}



//============================================================
//	osd_is_bad_read_ptr
//============================================================

int osd_is_bad_read_ptr(const void *ptr, size_t size)
{
	return 0;
}

int _main (int argc, char **argv)
{
	int game_index;
	int res = 0;

	init_ticker();	/* after Allegro init because we use cpu_cpuid */

	game_index = cli_frontend_init( argc, argv );

	if( game_index != -1 )
	{
		/* go for it */
		res = run_game (game_index);
	}

	cli_frontend_exit();

//	exit (res);
	return (res);
}

int printf(const char *fmt, ...)
{
va_list va;
char buf[2048];

	va_start(va, fmt);
	vsnprintf(buf, sizeof(buf), fmt, va);
	va_end(va);

	return printf_(buf);
}

int fprintf(FILE *fp, const char *fmt, ...)
{
va_list va;
char buf[2048];
int rtn;

	va_start(va, fmt);
	if (stdout ==fp || stderr ==fp)
		rtn =vsnprintf(buf, sizeof(buf), fmt, va);
	else
		rtn =vfprintf(fp, fmt, va);
	va_end(va);

	return(rtn);
}
