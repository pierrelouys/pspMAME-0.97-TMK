//============================================================
//
//	psp_main.c - PSP main
//
//============================================================

#include <time.h>
#include "driver.h"
#include <string.h>
#include "syscall.h"
#include "menu.h"
#include <psppower.h>
#include <pspctrl.h>
#include <stdio.h>

#include <pspdebug.h>
//============================================================
//	PSP module info section
//============================================================

#ifdef KERNEL_MODE
PSP_MODULE_INFO("pspMAME86", 0x1000, 0, 4);
PSP_MAIN_THREAD_ATTR(0);
#else
PSP_MODULE_INFO("pspMAME86", 0x0000, 0, 4);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);

PSP_HEAP_SIZE_KB(-1024);
//PSP_HEAP_SIZE_MAX();
#endif

#define PATHLIST_H 20
#define MAXPATH   0x108
//#include "psp_main.h"

#define printfX pspDebugScreenPrintf

//#include "pg.h"
//#include "syscall.h"
//#include "psp_menu.h"







//============================================================
//	GLOBAL VARIABLES
//============================================================

int	dlist_num;
int	dlist_start;
int	dlist_curpos;
extern SETTING setting;

int psp_exit =0;

volatile int psp_loop;
volatile int psp_sleep;



//============================================================
//	PROTOTYPES
//============================================================

int run_mame(int argc, char **argv);



//============================================================
//	HOMEキー関連
//============================================================

//============================================================
//	ExitCallback
//============================================================

static SceKernelCallbackFunction/*int*/ ExitCallback(int arg1, int arg2, void *arg)
{
	psp_exit =1;

	pgaTerm();

	scePowerSetClockFrequency(222,222,111);
	save_config();

	psp_loop = 0;
	psp_sleep = 0;
	return 0;
}



//============================================================
//	PowerCallback
//============================================================

static SceKernelCallbackFunction/*int*/ PowerCallback(int unknown, int pwrflags, void *arg)
{
	if (pwrflags & PSP_POWER_CB_POWER_SWITCH)
	{
		psp_sleep = 1;
		save_config();
	}
	else if (pwrflags & PSP_POWER_CB_RESUME_COMPLETE)
	{
		psp_sleep = 0;
	}

	int cbid;
	cbid = sceKernelCreateCallback("Power Callback", ( int (*)(int, int, void*) )PowerCallback, NULL);
	scePowerRegisterCallback(0, cbid);
	return 0;
}



//============================================================
//	CreateCallbackThread
//============================================================
/*--------------------------------------------------------
	コールバックスレッド作成
--------------------------------------------------------*/
int exit_callback(int arg1, int arg2, void *common) {
		sceKernelExitGame();
		return 0;
	}


static int CallbackThread(SceSize/*int*/ args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);

	cbid = sceKernelCreateCallback("Power Callback", ( int (*)(int, int, void*) )PowerCallback, NULL);
	scePowerRegisterCallback(0, cbid);

	sceKernelSleepThreadCB();
	return 0;
}



/*--------------------------------------------------------
	コールバックスレッド設定
--------------------------------------------------------*/

/* Sets up the callback thread and returns its thread id */
static /*int*/void SetupCallbacks(void)
{
	SceUID/*int*/ thread_id = 0;

	//0 = kernel ???
	thread_id = sceKernelCreateThread("Update Thread", CallbackThread, /*0x12*/0x11, 0xFA0, 0, 0/*NULL*/);
	//PSP_THREAD_ATTR_USER
	//thread_id = sceKernelCreateThread("Update Thread", CallbackThread, 0x12, 0xFA0, PSP_THREAD_ATTR_USER, NULL);

	if (thread_id >= 0)
	{
		sceKernelStartThread(thread_id, 0, 0);
	}

//	return thread_id;
}

//============================================================
//	GUI
//============================================================
#define REPEAT_TIME 0x40000
static unsigned long control_bef_ctl  = 0;
static unsigned long control_bef_tick = 0;

unsigned long Read_Key(void) {
	ctrl_data_t ctl;

	sceCtrlReadBufferPositive(&ctl,1);
	if (ctl.buttons == control_bef_ctl) {
		if ((ctl.frame - control_bef_tick) > REPEAT_TIME) {
			return control_bef_ctl;
		}
		return 0;
	}
	control_bef_ctl  = ctl.buttons;
	control_bef_tick = ctl.frame;
	return control_bef_ctl;
}

void Get_DriverList(void) {
	int i;

	dlist_num = 0;

	for (i = 0;drivers[i];i++)
		dlist_num +=1;
}

void Draw_All(void)
{
	int	i, col;
//	char wk[20];
//	int clk =CLOCKS_PER_SEC;
    char memInfo[512];
//	pgFillvram(0);
    long freeMem;
    long largestFreeBlock;
    int sizeFreeMemMb;
    int largestBlockMb;
//	mh_print(0, 0, APP_TITLE, setting.color[1]);
//	sprintf(wk, "%d", clk);
//	mh_print(0,10, wk, 0xffff);

	psp_frame(0, 0);

    col = setting.color[2];

    freeMem = sceKernelTotalFreeMemSize();
    largestFreeBlock = sceKernelMaxFreeMemSize();

    sizeFreeMemMb = (int)(freeMem / 1024)/1024;
    largestBlockMb = (int)(largestFreeBlock / 1024)/1024;

    sprintf(memInfo,"total free memory='%d' b, '%d' mb , largest free memory block='%d' b, '%d' mb",freeMem,sizeFreeMemMb,largestFreeBlock,largestBlockMb);
    mh_print(1, 10, memInfo, col);



	// ゲームリスト
	i = dlist_start;
	while (i<(dlist_start+PATHLIST_H)) {
		if (i<dlist_num) {
			col = setting.color[2];
			if (i==dlist_curpos) {
				col = setting.color[3];
			}
//			mh_print(32, ((i-dlist_start)+2)*10, (unsigned char *)drivers[i]->name, col);
			mh_print(32, ((i-dlist_start)+4)*10, (unsigned char *)drivers[i]->description, col);
		}
		i++;
	}
	pgScreenFlipV();
}

void Draw_Confirm(void) {
//	pgFillvram(0);
	psp_frame(0, 0);

	mh_print(40, 80, (unsigned char *)drivers[dlist_curpos]->name, setting.color[3]);
	mh_print(50, 90, (unsigned char *)drivers[dlist_curpos]->description, setting.color[3]);
	mh_print(40,120, (unsigned char *)"を実行します。", setting.color[3]);

	mh_print(40,160, (unsigned char *)"実行：○  キャンセル：×", setting.color[3]);
	pgScreenFlipV();
}

int Confirm_Control(void) {
	unsigned long key;

	while(1) {
		while(1) {
			key = Read_Key();
			if (key != 0) break;
			pgWaitV();
		}

		if (key & CTRL_CIRCLE) {
			return 1;
		}
		if (key & CTRL_CROSS) {
			return 0;
		}
	}
}

int Control(void) {
	unsigned long key;
//	int i;

	// wait key
	while(1) {
		key = Read_Key();
		if (key != 0) break;
		pgWaitV();
	}

	if (key & CTRL_UP) {
		if (dlist_curpos > 0) {
			dlist_curpos--;
			if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
		}
	}
	if (key & CTRL_DOWN) {
		if (dlist_curpos < (dlist_num-1)) {
			dlist_curpos++;
			if (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
		}
	}
	if (key & CTRL_LEFT) {
		dlist_curpos -= PATHLIST_H;
		if (dlist_curpos <  0)          { dlist_curpos = 0;           }
		if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
	}
	if (key & CTRL_RIGHT) {
		dlist_curpos += PATHLIST_H;
		if (dlist_curpos >= dlist_num) { dlist_curpos = dlist_num-1; }
		while (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
	}

	if (key & CTRL_CIRCLE) {
		return 2;
	}
	if (key & CTRL_LTRIGGER) {
		return 3;
	}

	return 0;
}



//============================================================
//	メイン
//============================================================
/*
#ifdef KERNEL_MODE
static int user_main(SceSize args, void *argp)
#else*/
int main(int argc, char *argv[])
//#endif //KERNEL_MODE
{
//	int res = 0;
	int game_index;
	int res = 0;

	char *_argv[5];
	int _argc = 0;

	char sPath[40];
	char pPtr[512];
    char *RpPtr;

    char fHell[512];

    int i;//killme

	psp_loop = 1;
	psp_sleep = 0;


	SetupCallbacks();
    pgcInit();

	pgScreenFrame(2,0);
    sceCtrlSetSamplingCycle(0);//sceCtrlInit(0);
    sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG/*1*/);//sceCtrlSetAnalogMode(1);

    //_argv[_argc++] = getCurDir();
    setCurDir(argv[0]);/* ユーザーモード時のみ使用 */
    _argv[_argc++] = getCurDir();


    //needed for psp/fileio.c osd_fopen , psp needs full qualified path to open() files
    setCurrentAppDirectory(getCurDir());


	Get_DriverList(); //<--- dlist_num == numero de drivers , esta en driver.h gameDriver *drivers[];
	dlist_start  = 0;
	dlist_curpos = 0;

    sprintf(fHell,"%s%s",getCurDir(),"pspLogDelOrto.txt");
	openLog(fHell);
    logWrite("arranco la cuestion!","","");
    logWrite("estos son los argv[0]:'",argv[0],"'");
    logWrite("estos son los argv[1]:'",argv[1],"'");


	logWriteX("probando la tid...","","",sceKernelGetThreadId());
	//psp_printf_init();
	//pgcInit();


	//_argv[_argc++] = "./";//argv;
	//_argv[_argc++] ="-log";


	load_config();


	//load_menu_bg();


	//bgbright_change();

	while(1) {
		Draw_All();
		switch(Control()) {
		case 2:
			Draw_Confirm();
			if (Confirm_Control()!=0) {
				pgFillvram(0);
				pgScreenFlipV();
				pgFillvram(0);
				pgScreenFlipV();

				_argv[_argc] = (char *)drivers[dlist_curpos]->name;

				if(setting.cpu_clock==0)
					scePowerSetClockFrequency(222,222,111);
				else if(setting.cpu_clock==1)
					scePowerSetClockFrequency(266,266,133);
				else if(setting.cpu_clock==2)
					scePowerSetClockFrequency(333,333,166);

				psp_exit =0;

/*
                for(i=0;i < _argc;i++){
                   printf("mame argument (%d): '%s'\n",i,_argv[i]);

                }*/
//de src/fileio.c :  , ver common.c : rom_load o load_rom no se!
/*mame_file *mame_fopen_rom(const char *gamename, const char *filename, const char* exphash)
{
	return generic_fopen(FILETYPE_ROM, gamename, filename, exphash, FILEFLAG_OPENREAD | FILEFLAG_HASH);
}*/
                //TEST:
                /*_argc = 0;
                strcpy(_argv[0],"ms0:/PSP/GAME/mameGBeret/");*/

               logWriteY("ejecutando run_mame..._argc + 1","","",sceKernelGetThreadId(),_argc + 1,0);

               for(i=0;i < _argc+1;i++)
                  logWriteY("argumentos run_mame : argv=='",_argv[i],"'",sceKernelGetThreadId(),i,0);

               res = run_mame(_argc + 1, _argv);
				//res = run_game (game_index);

				scePowerSetClockFrequency(222,222,111);
				malloc_psp_term();
//				malloc_term();
//				_sbrk_term();
			}
			break;
		case 3:
			psp_menu();
			break;
		}
	}


//---------> no es original:

	save_config();
	psp_video_exit();
#ifndef KERNEL_MODE
	sceKernelExitGame();/* ユーザーモード時のみ使用 */
#endif
	return 0;
}

#ifdef KERNEL_MODE
soy una conchuda
static SceUID/*int*/ main_thread;

void main_thread_set_priority(int priority)
{
	sceKernelChangeThreadPriority(main_thread, priority);
}

int main(int argc, char *argv[])
{
	setCurDir(argv[0]);/* カーネルモードの場合はここで。 */

	main_thread = sceKernelCreateThread(
		"User Mode Thread",
		user_main,
		0x11,
		256 * 1024,
		PSP_THREAD_ATTR_USER,
		NULL);

	sceKernelStartThread(main_thread, 0, 0);
	sceKernelWaitThreadEnd(main_thread, NULL);

	sceKernelExitGame();/* カーネルモードの場合はここで。 */

	return 0;
}
#endif


void exit(int i)
{
	pgaTerm();

	pgScreenFlip();
	Confirm_Control();

//	sceKernelExitGame();
	_exit(i);

}

void raise(int i)
{
	exit(i);
}

int printf2_( const char *format, const char *format2)
{
int rtn;

	rtn =printf2nw_( format, format2);

	Confirm_Control();
	return(rtn);
}

int printf2nw_( const char *format, const char *format2)
{
char buff[1000];

	strcpy(buff, format);
	strcat(buff, ":");
	strcat(buff, format2);

	return(printfnw_(buff));
}

int printf_( const char *format)
{
int rtn;

	rtn =printfnw_(format);

	Confirm_Control();
	return(rtn);
}

int printfnw_(const char *format)
{
int rtn;

	rtn =printflow(format, strlen(format), 0);
	fflush_();
	return rtn;
}

static int printbuflen =0;
int printflow(const char *format, int len, int flush)
{
static int sx =0, sy =0;
int i,j,k;
char buf[100];

	if (!printbuflen) {
		pgFillvram(0);
		sx =sy =0;
	}

	for(i=sy, k=0; i <27 && k <strlen(format) && k <len; i++)
	{
		for(j=0;(sx+j) <96 && format[k] !='\n' && k <strlen(format) && k <len; j++, k++)
			buf[j] =format[k];
		buf[j] =0x00;
		mh_print(sx *5, i*10, buf, RGB(255,255,255));
		sx +=j;
		if (95 <=sx) {
			sx =0;
			sy ++;
		}
		if ('\n' ==format[k]) {
			sx =0;
			sy ++;
			k ++;
		}

		printbuflen +=j;
	}

	if (flush) {
//		fflush_();
		printbuflen =0;
		pgScreenFlipV();
		Confirm_Control();
	}
	return(k);
}

static int fl_mode =1;
int fflushmode_(int fast)
{
	fl_mode =fast;
}

int printflen_(const char *format, int len)
{
	return(printflow(format, len, fl_mode));
}

int fflush_(void)
{
	if (printbuflen) {
		pgScreenFlipV();
		if (0 ==fl_mode)
			Confirm_Control();
	}
	printbuflen =0;

	return(0);
}
