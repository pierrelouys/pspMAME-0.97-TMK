//============================================================
//
//	psp.c
//
//============================================================

//#include <signal.h>
//#include <time.h>
//#include <ctype.h>
//#include "psp_main.h"
#include "driver.h"
//#include "ticker.h"

/* MAME終了時にメモリーがきちんと開放されたかチェックする(1==ON, 0==OFF) */
  #define DEBUG_MAME_MEMORY_CHECK 1
//#define DEBUG_MAME_MEMORY_CHECK 0

//============================================================
//	PROTOTYPES
//============================================================

//int  psp_init_sound(void);
//void psp_init_input(void);
//void psp_sound_exit(void);
//void psp_shutdown_input(void);

// in psp/input.c
void init_keyboard(void);
void shutdown_keyboard(void);

extern void cli_frontend_exit(void);
extern int cli_frontend_init(int argc, char **argv);

#if (1==DEBUG_MAME_MEMORY_CHECK)

static UINT32 start_size, end_size;

//============================================================
//	check_free_memory
//============================================================

static UINT32 check_free_memory(void)
{
	UINT8 *mem;
	int size_mb = 0;
	int size_kb = 0;
	int size_b  = 0;

	while (1)
	{
		if ((mem = malloc(size_mb)) == NULL)
			break;

		free(mem);
		size_mb += 1024*1024;
	}
	while (1)
	{
		if ((mem = malloc(size_mb + size_kb)) == NULL)
			break;

		free(mem);
		size_kb += 1024;
	}
	while (1)
	{
		if ((mem = malloc(size_mb + size_kb + size_b)) == NULL)
			break;

		free(mem);
		size_b += 100;
	}
	while (1)
	{
		if ((mem = malloc(size_mb + size_kb + size_b)) == NULL)
			break;

		free(mem);
		size_b++;
	}
	return size_mb + size_kb + size_b;
}
#endif //(1==MEMORY_CHECK)

//============================================================
//	osd_init
//============================================================

int osd_init(void)
{
	psp_sound_init();//psp_init_sound();
	init_keyboard();//psp_init_input();
	return 0;
}



//============================================================
//	osd_exit
//============================================================

void osd_exit(void)
{
	//shutdown_keyboard();//	psp_shutdown_input();
	//psp_sound_exit();
}



//============================================================
//	osd_alloc_executable
//============================================================

void *osd_alloc_executable(size_t size)
{
	return malloc(size);
}



//============================================================
//	osd_free_executable
//============================================================

void osd_free_executable(void *ptr)
{
	free(ptr);
}



//============================================================
//	osd_is_bad_read_ptr
//============================================================

int osd_is_bad_read_ptr(const void *ptr, size_t size)
{
	return 0;
}


#if 0
//============================================================
//	osd_printf
//============================================================

void CLIB_DECL osd_printf(const char *text, ... )
{
	char buf[128];

	va_list va;
	va_start(va, text);
	vsprintf(buf, text, va);
	va_end(va);

	psp_printf_bbb(buf);
}
#endif

#if 1
//============================================================
//	osd_die
//============================================================

void CLIB_DECL osd_die(const char *text, ... )
{
	char buf[128];

	va_list va;
	va_start(va, text);
	vsprintf(buf, text, va);
	va_end(va);

	psp_printf_bbb(buf);
	psp_print_color(PSP_COLOR_RED);
	psp_printf_bbb("press any key to terminate.");
	Confirm_Control();

	sceKernelExitGame();
	//exit(1);
	return;
}
#endif


//============================================================
//	run_mame
//============================================================

int run_mame(int argc, char **argv)
{
	int game_index;
	int res = 0;

	//init_ticker();	/* after Allegro init because we use cpu_cpuid */
#if (1==DEBUG_MAME_MEMORY_CHECK)
	start_size = check_free_memory();
#endif //(1==MEMORY_CHECK)

	psp_print_locate(0, 0);
	game_index = cli_frontend_init(argc, argv);

	if (game_index != -1)
	{
		/* go for it */
		res = run_game(game_index);
	}
	cli_frontend_exit();/*???*/

#if (1==DEBUG_MAME_MEMORY_CHECK)
	end_size = check_free_memory();

	if (start_size != end_size)
	{
		psp_print_color(PSP_COLOR_RED);
		psp_printf_bbb(
			"メモリーリークですぅ\n"
			" 開始 : %d(0x%x) bytes.\n"
			" 終了 : %d(0x%x) bytes.\n"
			" 差分 : %d(0x%x) bytes.\n"
			, start_size
			, start_size
			, end_size
			, end_size
			, (end_size-start_size)
			, (end_size-start_size)
		);
		psp_print_color(PSP_COLOR_WHITE);
	}
#endif //(1==MEMORY_CHECK)

//	exit(res);
	return(res);
}


#if 0
//============================================================
//	printf
//============================================================

int printf(const char *text, ... )
{
	char buf[2048];
	va_list arg;

	va_start(arg, text);
	vsprintf(buf, text, arg);
	va_end(arg);

	/*return*/ psp_printf_bbb(buf);
}
#endif
