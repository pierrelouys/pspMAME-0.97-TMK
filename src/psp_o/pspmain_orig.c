#include <time.h>
#include "driver.h"

//#include "pg.h"
#include "syscall.h"
#include "menu.h"

#define PATHLIST_H 20
#define MAXPATH   0x108
int	dlist_num;
int	dlist_start;
int	dlist_curpos;
extern SETTING setting;

int psp_exit =0;

int _main (int argc, char **argv);

//============================================================
//	HOMEキー関連
//============================================================
int exit_callback(void)
{
	psp_exit =1;

//	malloc_psp_term();
//	malloc_term();
	pgaTerm();

	scePowerSetClockFrequency(222,222,111);
	save_config();

	sceKernelExitGame();

	return 0;
}

void power_callback(int unknown, int pwrflags)
{
	if(pwrflags & POWER_CB_POWER){
		scePowerSetClockFrequency(222,222,111);

		save_config();
	}

	int cbid = sceKernelCreateCallback("Power Callback", power_callback);
	scePowerRegisterCallback(0, cbid);
}

int CallbackThread(void *arg)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback);
	sceKernelRegisterExitCallback(cbid);
	cbid = sceKernelCreateCallback("Power Callback", power_callback);
	scePowerRegisterCallback(0, cbid);

	sceKernelPollCallbacks();

	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", (pg_threadfunc_t)CallbackThread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
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

//	pgFillvram(0);

//	mh_print(0, 0, APP_TITLE, setting.color[1]);
//	sprintf(wk, "%d", clk);
//	mh_print(0,10, wk, 0xffff);

	psp_frame(0, 0);

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
int xmain(int argc, char *argv)
{
	int game_index;
	int res = 0;
	char *_argv[5];
	int _argc =0;

	// Initialize
//	pgaInit();
	SetupCallbacks();

	pgcInit();
	pgScreenFrame(2,0);

	sceCtrlSetAnalogMode(1);
	sceCtrlInit(0);

	//
	Get_DriverList();
	dlist_start  = 0;
	dlist_curpos = 0;

	/*char sPath[40], *pPtr;
	strcpy(sPath, argv);
	pPtr =strrchr(sPath, '/');
	*pPtr = 0;
	chdir(sPath);*/

	_argv[_argc++] =argv;
	//_argv[_argc++] ="-log";

	load_config();
	load_menu_bg();
	bgbright_change();

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

				_argv[_argc] =drivers[dlist_curpos]->name;

				if(setting.cpu_clock==0)
					scePowerSetClockFrequency(222,222,111);
				else if(setting.cpu_clock==1)
					scePowerSetClockFrequency(266,266,133);
				else if(setting.cpu_clock==2)
					scePowerSetClockFrequency(333,333,166);

				psp_exit =0;


				res = _main(_argc+1, _argv);
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

	return 0;
}

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

//int puts( const char *string )
//{
//	mh_print(0, 0, (unsigned char *)string, RGB(255,255,255),0,0);
//
//	return(1);
//}
//
//int *__errno() {
//	return 0;
//}
