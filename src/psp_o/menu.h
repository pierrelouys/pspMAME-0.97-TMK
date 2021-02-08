#ifndef MENU_H
#define MENU_H

#include "pg.h"

typedef struct
{
	char vercnf[16];
	int screensize;
	int frameskip;
	int vsync;
	int sound;
	int sound_rate;
	unsigned long color[4];
	int bgbright;
	int analog2dpad;
	int cpu_clock;
} SETTING;

enum{
	DEF_COLOR0=0x9063,
	DEF_COLOR1=RGB(85,85,95),
	DEF_COLOR2=RGB(105,105,115),
	DEF_COLOR3=0xffff,
};

enum{
	SCR_X1,
	SCR_STRETCH,
	SCR_STRETCH_ACPECT,
	SCR_ROT,
	SCR_ROT_STRETCH,
	SCR_ROT_STRETCH_ACPECT,
	SCR_END,
};

#define CONFIG_VER	"PSPMAMEr0.2"
#define APP_TITLE	"PSPMAME r0.4 (M.A.M.E. v0.97 for PSP)"

//enum{
//	STATE_SLOT_MAX=4,
//};
//extern int QuickSlot, bTurbo;

//int save_state(int slot);
//int load_state(int slot);
//long load_rom(const char *pszFile);
void rin_frame(const char *msg0, const char *msg1);
void rin_menu(void);

#endif
