
#ifndef _PSP_MENU_H_
#define _PSP_MENU_H_

//#include "pg.h"

typedef struct
{
	char vercnf[16];
	u8/*u8*//*int*/ frameskip;
	u8/*u8*//*int*/ screensize;
	u8/*u8*//*int*/ vsync_ON_OFF;
	u8/*u8*//*int*/ show_fps_ON_OFF;//	int showfps;

	u8/*u8*//*int*/ sound_skip;
	u8/*u8*//*int*/ limit_ON_OFF;//sound_ON_OFF_dummy;
	u8/*u8*//*int*/ sound_rate;
	u8/*u8*//*int*/ cpu_clock;

	unsigned long color[4];
	int bgbright;
//	int analog2dpad;
} SETTING;


#define DEF_COLOR0			MAKECOL15( 20, 20, 21)
#define DEF_COLOR1			MAKECOL15( 85, 85, 95)
#define DEF_COLOR2			MAKECOL15(105,105,115)
#define DEF_COLOR3			MAKECOL15(245,245,255)

#define PSP_COLOR_RED		MAKECOL15(255, 20, 40)
#define PSP_COLOR_GREEN 	MAKECOL15( 55,220, 40)
#define PSP_COLOR_BLUE		MAKECOL15( 35, 55,255)
#define PSP_COLOR_YELLOW	MAKECOL15(255,245,100)
#define PSP_COLOR_WHITE 	MAKECOL15(255,255,255)

enum{
	SND_OFF=0,
	SND_44100=1,/* 44100=44100/1 *//*d=735*//*735[samples/flame]==44100[samples/sec]/60[flame] */
	SND_22050,	/* 22050=44100/2 *//*d=368*/
	SND_14700,	/* 14700=44100/3 *//*d=245*/
	SND_11025,	/* 11025=44100/4 *//*d=184*/
	SND_08820,	/*	8820=44100/5 *//*d=147*/
	SND_07350,	/*	7350=44100/6 *//*d=123*/
	SND_06300,	/*	6300=44100/7 *//*d=105*/
	SND_05512,	/*	5512=44100/8 *//*d= 92*/
	SND_MAX,
};

enum{
	SCR_AUTO_STRETCH=0,              //0
	SCR_STRETCH_240_320,              //1
	SCR_STRETCH_240_320_NO_ADJUST,      //2
//	SCR_STRETCH_ASPECT,
	SCR_STRETCH_256,                    //3
	SCR_STRETCH_288,                    //4
	SCR_X1,                             //5
	SCR_ROT,                            //6
//	SCR_ROT_STRETCH,
//	SCR_ROT_STRETCH_ASPECT,
	SCR_MAX,//SCR_END,
};
//enum{
//	SCR_MODE01=0,
//	SCR_MODE02,
//	SCR_MODE03,
//	SCR_MODE04,
//	SCR_MODE05,
//	SCR_MODE06,
//	SCR_MAX,
//};

#define CONFIG_VER	"PSPMAME_Z00"

extern int dlist_curpos;

void Get_DriverList(void);
void Draw_All(void);
int Confirm_Control(void);
int Control(void);
void Draw_Confirm(void);

void save_config(void);
void load_config(void);
void load_menu_bg(void);
//static void psp_frame(const char *msg0, const char *msg1);
void bgbright_change(void);
void psp_menu(void);



//enum{
//	STATE_SLOT_MAX=4,
//};
//extern int QuickSlot, bTurbo;

//int save_state(int slot);
//int load_state(int slot);
//long load_rom(const char *pszFile);
//void rin_frame(const char *msg0, const char *msg1);
//void rin_menu(void);

#endif //_PSP_MENU_H_
