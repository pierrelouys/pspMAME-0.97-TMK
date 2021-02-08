#include <stdio.h>
#include <string.h>
#include <pspiofilemgr.h>
#include "menu.h"
#include "syscall.h"
#include "colbl.c"
#include <malloc.h>

unsigned long Read_Key(void);

u32 new_pad;

const char *scr_names[] = {
	"x1",
	"stretch image",
	"stretch & aspect keep",
	"rotate",
	"rotate & stretch",
	"rotate & stretch & aspect keep",
};

int bBitmap;
unsigned short bgBitmap[480*272];

char *getCurDir(void);
SETTING setting;

void save_config(void)
{
	char CfgPath[MAX_PATH];
	char *p;

	strcpy(CfgPath, getCurDir());
	p = strrchr(CfgPath, '/')+1;
	strcpy(p, "PSPMAME.CFG");

	int fd;
	fd = sceIoOpen(CfgPath,O_CREAT|O_WRONLY|O_TRUNC, 0777);
	if(fd>=0){
		sceIoWrite(fd, &setting, sizeof(setting));
		sceIoClose(fd);
	}
}

void load_config(void)
{
	int i;
	char CfgPath[MAX_PATH];
	char *p;

	memset(&setting, 0, sizeof(setting));
    strcpy(CfgPath, getCurDir());
	p = strrchr(CfgPath, '/')+1;/**/

	//strcpy(CfgPath, "./");
	//strcpy(p, "PSPMAME.CFG");//<---- OFFENSOR!


	int fd;

	/*fd = sceIoOpen(CfgPath,O_RDONLY, 0777);
	printfnw_("despues de open...\n");

	if(fd>=0){
		sceIoRead(fd, &setting, sizeof(setting));
		sceIoClose(fd);

		if(setting.screensize<0 || setting.screensize>=SCR_END)
			setting.screensize = SCR_X1;
		if(setting.frameskip > 9)
			setting.frameskip=0;
		if(setting.sound_rate<0 || setting.sound_rate>2)
			setting.sound_rate = 0;
		if(setting.bgbright<0 || setting.bgbright>100)
			setting.bgbright=100;
		if(setting.cpu_clock<0 || setting.cpu_clock>2)
			setting.cpu_clock = 0;
		if(!strcmp(setting.vercnf, CONFIG_VER))
			return;
	}*/

	strcpy(setting.vercnf, CONFIG_VER);

	setting.screensize = SCR_X1;
	setting.frameskip = 0;
	setting.vsync = 0;
	setting.sound = 1;
	setting.sound_rate = 0;

	setting.color[0] = DEF_COLOR0;
	setting.color[1] = DEF_COLOR1;
	setting.color[2] = DEF_COLOR2;
	setting.color[3] = DEF_COLOR3;
	setting.bgbright=100;

	setting.cpu_clock = 0;
}

//more---




void load_menu_bg()
{
	unsigned char *menu_bg;
	unsigned char *vptr;
	static unsigned char menu_bg_buf[480*272*3+0x36];
	char BgPath[MAX_PATH];
	char *p;
 	unsigned short x,y,yy,r,g,b,data;

	/*strcpy(BgPath, getCurDir());
	p = strrchr(BgPath, '/')+1;*/
	strcpy(p, "MENU.BMP");

	int fd;
	fd = sceIoOpen(BgPath,O_RDONLY,0777);
	if(fd>=0){
		sceIoRead(fd, menu_bg_buf, 480*272*3+0x36);
		sceIoClose(fd);

		menu_bg = menu_bg_buf + 0x36;
		vptr=(unsigned char*)bgBitmap;
		for(y=0; y<272; y++){
			for(x=0; x<480; x++){
				yy = 271 - y;
				r = *(menu_bg + (yy*480 + x)*3 + 2);
				g = *(menu_bg + (yy*480 + x)*3 + 1);
				b = *(menu_bg + (yy*480 + x)*3);
				data = (((b & 0xf8) << 7) | ((g & 0xf8) << 2) | (r >> 3));
				*(unsigned short *)vptr=data;
				vptr+=2;
			}
		}
		bBitmap = 1;
	}else{
		bBitmap = 0;
	}
}

// 半透明処理
unsigned short rgbTransp(unsigned short fgRGB, unsigned short bgRGB, int alpha) {

    unsigned short fgR, fgG, fgB;
    unsigned short bgR, bgG, bgB;
	unsigned short R, G, B;
 	unsigned short rgb;

    fgB = (fgRGB >> 10) & 0x1F;
    fgG = (fgRGB >> 5) & 0x1F;
    fgR = fgRGB & 0x1F;

    bgB = (bgRGB >> 10) & 0x1F;
    bgG = (bgRGB >> 5) & 0x1F;
    bgR = bgRGB & 0x1F;

	R = coltbl[fgR][bgR][alpha/10];
	G = coltbl[fgG][bgG][alpha/10];
	B = coltbl[fgB][bgB][alpha/10];

	rgb = (((B & 0x1F)<<10)+((G & 0x1F)<<5)+((R & 0x1F)<<0)+0x8000);
    return rgb;
}

void bgbright_change()
{
	unsigned short *vptr,rgb;
 	int i;

//	load_menu_bg();
	vptr=bgBitmap;
	for(i=0; i<272*480; i++){
			rgb = *vptr;
			*vptr = rgbTransp(rgb, 0x0000, setting.bgbright);
			vptr++;
	}
}

void psp_frame(const char *msg0, const char *msg1)
{
	if(bBitmap)
		pgBitBlt(0,0,480,272,1,bgBitmap);
	else
		pgFillvram(setting.color[0]);
	mh_print(0, 0, APP_TITLE, setting.color[1]);
	// メッセージなど
	if(msg0) mh_print(17, 14, msg0, setting.color[2]);
	pgDrawFrame(17,25,463,248,setting.color[1]);
	pgDrawFrame(18,26,462,247,setting.color[1]);
	// 操作説明
	if(msg1) mh_print(17, 252, msg1, setting.color[2]);
}

void psp_colorconfig(void)
{
	enum
	{
		COLOR0_R=0,
		COLOR0_G,
		COLOR0_B,
		COLOR1_R,
		COLOR1_G,
		COLOR1_B,
		COLOR2_R,
		COLOR2_G,
		COLOR2_B,
		COLOR3_R,
		COLOR3_G,
		COLOR3_B,
		BG_BRIGHT,
		EXIT,
		INIT,
	};
	char tmp[4], msg[256];
	int color[4][3];
	int sel=0, x, y, i, sel_count;

	memset(color, 0, sizeof(int)*4*3);
	for(i=0; i<4; i++){
		color[i][2] = setting.color[i]>>10 & 0x1F;
		color[i][1] = setting.color[i]>>5 & 0x1F;
		color[i][0] = setting.color[i] & 0x1F;
	}

	for(;;){
		new_pad =Read_Key();
		if(new_pad & CTRL_CIRCLE){
			if(sel==EXIT){
				break;
			}else if(sel==INIT){
				color[0][2] = DEF_COLOR0>>10 & 0x1F;
				color[0][1] = DEF_COLOR0>>5 & 0x1F;
				color[0][0] = DEF_COLOR0 & 0x1F;
				color[1][2] = DEF_COLOR1>>10 & 0x1F;
				color[1][1] = DEF_COLOR1>>5 & 0x1F;
				color[1][0] = DEF_COLOR1 & 0x1F;
				color[2][2] = DEF_COLOR2>>10 & 0x1F;
				color[2][1] = DEF_COLOR2>>5 & 0x1F;
				color[2][0] = DEF_COLOR2 & 0x1F;
				color[3][2] = DEF_COLOR3>>10 & 0x1F;
				color[3][1] = DEF_COLOR3>>5 & 0x1F;
				color[3][0] = DEF_COLOR3 & 0x1F;
				setting.bgbright = 100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else if(sel == BG_BRIGHT) {
				//輝度変更
				setting.bgbright += 10;
				if(setting.bgbright > 100) setting.bgbright=0;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else{
				if(color[sel/3][sel%3]<31)
					color[sel/3][sel%3]++;
			}
		}else if(new_pad & CTRL_CROSS){
			if(sel == BG_BRIGHT) {
				//輝度変更
				setting.bgbright -= 10;
				if(setting.bgbright < 0) setting.bgbright=100;
				if(bBitmap){
					load_menu_bg();
					bgbright_change();
				}
			}else if(sel>=COLOR0_R && sel<=COLOR3_B){
				if(color[sel/3][sel%3]>0)
					color[sel/3][sel%3]--;
			}
		}else if(new_pad & CTRL_UP){
			if(sel!=0)	sel--;
			else		sel=INIT;
			sel_count=0;
		}else if(new_pad & CTRL_DOWN){
			if(sel!=INIT)	sel++;
			else			sel=0;
			sel_count=0;
		}else if(new_pad & CTRL_RIGHT){
			if(sel<COLOR1_R) 		sel=COLOR1_R;
			else if(sel<COLOR2_R)	sel=COLOR2_R;
			else if(sel<COLOR3_R)	sel=COLOR3_R;
			else if(sel<BG_BRIGHT)	sel=BG_BRIGHT;
			else if(sel<EXIT)		sel=EXIT;
			sel_count=0;
		}else if(new_pad & CTRL_LEFT){
			if(sel>BG_BRIGHT)		sel=BG_BRIGHT;
			else if(sel>COLOR3_B)	sel=COLOR3_R;
			else if(sel>COLOR2_B)	sel=COLOR2_R;
			else if(sel>COLOR1_B)	sel=COLOR1_R;
			else					sel=COLOR0_R;
			sel_count=0;
		}

		for(i=0; i<4; i++)
			setting.color[i]=color[i][2]<<10|color[i][1]<<5|color[i][0]|0x8000;

		if (++sel_count>=30) sel_count=0;

		x = 2;
		y = 5;

		if(sel>=COLOR0_R && sel<=BG_BRIGHT)
			strcpy(msg, "○：Add　×：Sub");
		else
			strcpy(msg, "○：OK");

		psp_frame(0, msg);

		pgPrint(x,y++,setting.color[3],"  COLOR0 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR0 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR0 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR1 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR1 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR1 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR2 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR2 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR2 B:");
		y++;
		pgPrint(x,y++,setting.color[3],"  COLOR3 R:");
		pgPrint(x,y++,setting.color[3],"  COLOR3 G:");
		pgPrint(x,y++,setting.color[3],"  COLOR3 B:");
		y++;
		if(setting.bgbright / 100 == 1)
			pgPrint(x,y++,setting.color[3],"  BG BRIGHT:100%");
		else
			pgPrint(x,y++,setting.color[3],"  BG BRIGHT:  0%");
		if(setting.bgbright % 100 != 0)			// 10%～90%
			pgPutChar((x+13)*8,(y-1)*8,setting.color[3],0,'0'+setting.bgbright/10,1,0,1);
		y++;
		pgPrint(x,y++,setting.color[3],"  Return to Main Menu");
		pgPrint(x,y++,setting.color[3],"  Initialize");

		x=14; y=5;
		for(i=0; i<12; i++){
			if(i!=0 && i%3==0) y++;
			//_itoa(color[i/3][i%3], tmp);
			sprintf(tmp, "%d", color[i/3][i%3]);
			pgPrint(x,y++,setting.color[3],tmp);
		}

		if (sel_count < 15){
			x = 2;
			y = sel + 5;
			if(sel>=COLOR1_R) y++;
			if(sel>=COLOR2_R) y++;
			if(sel>=COLOR3_R) y++;
			if(sel>=BG_BRIGHT) y++;
			if(sel>=EXIT) y++;
			pgPutChar((x+1)*8,y*8,setting.color[3],0,127,1,0,1);
		}

		pgScreenFlipV();
	}
}

int psp_screensize(int n)
{
	int x,y,i,sel=n;

	for(;;){
		new_pad =Read_Key();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>=SCR_END) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=SCR_END-1;
		}else if(new_pad & CTRL_RIGHT){
			sel+=SCR_END/2;
			if(sel>=SCR_END) sel=SCR_END-1;
		}else if(new_pad & CTRL_LEFT){
			sel-=SCR_END/2;
			if(sel<0) sel=0;
		}

		psp_frame("Select Screen Mode", "○：OK  ×：Cancel");

		x=4, y=5;
		pgPrint(x++,y++,setting.color[3],"SCREEN MODE:");
		for(i=0; i<SCR_END; i++){
			if(i==sel)
				pgPrint(x,y++,setting.color[2],scr_names[i]);
			else
				pgPrint(x,y++,setting.color[3],scr_names[i]);
		}

		pgScreenFlipV();
	}
}

int psp_frameskip(int sel)
{
	char tmp[8];
	int x,y,i;

	strcpy(tmp,"0");

	for(;;){
		new_pad =Read_Key();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>10) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=10;
		}else if(new_pad & CTRL_RIGHT){
			sel+=5;
			if(sel>10) sel=10;
		}else if(new_pad & CTRL_LEFT){
			sel-=5;
			if(sel<0) sel=0;
		}

		psp_frame("Select Frame Skip", "○：OK  ×：Cancel");

		x=4, y=5;
		pgPrint(x++,y++,setting.color[3],"FRAME SKIP:");
		if(0==sel)
			pgPrint(x,y++,setting.color[2],"Auto");
		else
			pgPrint(x,y++,setting.color[3],"Auto");
		for(i=1; i<=10; i++){
			tmp[0] = i + '0' -1;
			if(i==sel)
				pgPrint(x,y++,setting.color[2],tmp);
			else
				pgPrint(x,y++,setting.color[3],tmp);
		}

		pgScreenFlipV();
	}
}

const char *cpu_clocks[] = {
	"222MHz (default)",
	"266MHz",
	"333MHz",
};

int psp_cpuclock(int sel)
{
	unsigned long color;
	char *msg;
	int x,y,i;

	for(;;){
		new_pad =Read_Key();
		if(new_pad & CTRL_CIRCLE)
			return sel;
		else if(new_pad & CTRL_CROSS)
			return -1;
		else if(new_pad & CTRL_DOWN){
			sel++;
			if(sel>2) sel=0;
		}else if(new_pad & CTRL_UP){
			sel--;
			if(sel<0) sel=2;
		}

		psp_frame("Select CPU Clock (DANGER! DANGER! DANGER! DANGER! DANGER!)", "○：OK  ×：Cancel");

		x=4, y=5;
		pgPrint(x++,y++,setting.color[3],"CPU CLOCK:");
		for(i=0; i<3; i++){
			if(i==sel)	color = setting.color[2];
			else		color = setting.color[3];
			pgPrint(x,y++,color,cpu_clocks[i]);
		}

		pgScreenFlipV();
	}
}


void psp_menu(void)
{
	enum
	{
		SOUND,
		SOUND_RATE,
		SCREEN_SIZE,
		FRAME_SKIP,
		VSYNC,
		CPU_CLOCK,
		COLOR_CONFIG,
		CONTINUE,
	};
	char msg[256], tmp[256];
	static int sel=0;
	int x, y, sel_count=0, bLoop=1;
	int ret;
	int bSave, fd, romsize, ramsize;
	char *p;

	msg[0]=0;

	for(;;){
		new_pad =Read_Key();
		if(new_pad & CTRL_CIRCLE){
			switch(sel)
			{
			case SOUND:
				setting.sound = !setting.sound;
				break;

			case SOUND_RATE:
				setting.sound_rate++;
				if(setting.sound_rate>2) setting.sound_rate=0;
				break;

			case SCREEN_SIZE:
				ret = psp_screensize(setting.screensize);
				if(ret>=0)
					setting.screensize = ret;
				msg[0]=0;
				sel_count=0;
				break;

			case FRAME_SKIP:
				ret = psp_frameskip(setting.frameskip);
				if(ret>=0)
					setting.frameskip = ret;
				msg[0]=0;
				sel_count=0;
				break;


			case VSYNC:
				setting.vsync = !setting.vsync;
				break;

			case CPU_CLOCK:
				ret = psp_cpuclock(setting.cpu_clock);
				if(ret>=0){
					setting.cpu_clock = ret;
//					if(setting.cpu_clock==0)
//						scePowerSetClockFrequency(222,222,111);
//					else if(setting.cpu_clock==1)
//						scePowerSetClockFrequency(266,266,133);
//					else if(setting.cpu_clock==2)
//						scePowerSetClockFrequency(333,333,166);
				}
				break;

			case COLOR_CONFIG:
				psp_colorconfig();
				msg[0]=0;
				sel_count=0;
				break;

			case CONTINUE:
				bLoop = 0;
				break;
			}
		}else if(new_pad & (CTRL_CROSS | CTRL_LTRIGGER)){
			break;

		}else if(new_pad & CTRL_UP){
			if(sel!=0)
				sel--;
			else
				sel=CONTINUE;
			sel_count=0;

		}else if(new_pad & CTRL_DOWN){
			if(sel!=CONTINUE)	sel++;
			else				sel=0;
			sel_count=0;

//		}else if(setting.key_config[6] && (new_pad&setting.key_config[6])==setting.key_config[6]){
//			break;
		}

		if(!bLoop) break;

		if (++sel_count>=30) sel_count=0;

		psp_frame(msg, "○：OK  ×：Continue  MenuBTN：Continue");

		x = 2;
		y = 5;

		if(setting.sound)
			pgPrint(x,y++,setting.color[3],"  SOUND: ON");
		else
			pgPrint(x,y++,setting.color[3],"  SOUND: OFF");

		if(setting.sound_rate == 0)
			pgPrint(x,y++,setting.color[3],"  SOUND RATE: 44khz");
		else if(setting.sound_rate == 1)
			pgPrint(x,y++,setting.color[3],"  SOUND RATE: 22khz");
		else
			pgPrint(x,y++,setting.color[3],"  SOUND RATE: 11khz");

		strcpy(tmp,"  SCREEN MODE: ");
		strcat(tmp,scr_names[setting.screensize]);
		pgPrint(x,y++,setting.color[3],tmp);

		strcpy(tmp,"  FRAME SKIP:  ");
		if (0 ==setting.frameskip)
			strcat(&tmp[strlen(tmp)-1], "Auto");
		else
			tmp[strlen(tmp)-1] = setting.frameskip+'0' -1;
		pgPrint(x,y++,setting.color[3],tmp);

		if(setting.vsync)
			pgPrint(x,y++,setting.color[3],"  VSYNC: ON");
		else
			pgPrint(x,y++,setting.color[3],"  VSYNC: OFF");

		strcpy(tmp,"  CPU CLOCK: ");
		strcat(tmp,cpu_clocks[setting.cpu_clock]);
		pgPrint(x,y++,setting.color[3],tmp);

		pgPrint(x,y++,setting.color[3],"  COLOR CONFIG");
		y++;
		pgPrint(x,y++,setting.color[3],"  Continue");

		if(sel_count < 15){
			y = sel + 5;
			if(sel >= CONTINUE)	y++;
			pgPutChar((x+1)*8,y*8,setting.color[3],0,127,1,0,1);
		}

		pgScreenFlipV();
	}

//	pgFillvram(0);
//	pgScreenFlipV();
//	pgFillvram(0);
//	pgScreenFlipV();
	pgWaitVn(10);
}
