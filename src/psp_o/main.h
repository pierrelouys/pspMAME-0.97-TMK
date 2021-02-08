#ifndef MAIN_H
#define MAIN_H

#include "syscall.h"
#include "filer.h"
#include "menu.h"
#include "pg.h"
#include "gbcore/gb.h"
#include "sound.h"
#include "string.h"

//#define OVER_CLOCK
//#define DEBUG

typedef struct
{
	char vercnf[16];
	int screensize;
	int gb_type;
	int gb_color;
	int frameskip;
	int vsync;
	int sound;
	int sound_buffer;
	unsigned long color[4];
	int bgbright;
	int key_config[32]; //—]•ª‚ÉŠm•Û
	int analog2dpad;
	int thumb;
	int cpu_clock;
} SETTING;

enum{
	DEF_COLOR0=0x9063,
	DEF_COLOR1=RGB(85,85,95),
	DEF_COLOR2=RGB(105,105,115),
	DEF_COLOR3=0xffff,
};

extern char RinPath[], RomPath[], SavePath[], RomName[];
extern SETTING setting;

#endif
