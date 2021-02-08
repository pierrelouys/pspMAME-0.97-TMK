

#include "driver.h"
//#include "psp_main.h"
//#include "psp_menu.h"
//#include "pg.h"
//#include "colbl.c"

/* カラーメニューでのＸボタン（０：無効、１：有効） */
#define USE_COLOR_MENU_X_BUTTON 1


#define NOW_DATE   " "__DATE__" "
#define DEVELOPPER "developed by repuken2                            from original src by 開轣闃J発幼稚園"
//∽∝∵∫∬開発幼稚園

#ifdef namcoNA
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥⌒NA(Z12)"
	#define MENU_STR2 "専用ステレオサウンド44100!"
#endif
#ifdef halleys
	#define MENU_STR1 "TAITO86 / HALLEY'S COMET(Z02)"
	#define MENU_STR2 "ハレーズコメット"
#endif
#ifdef taitoAX
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥ AX(Z08)"
	#define MENU_STR2 "少し進化？"
#endif
#ifdef m72
	#define MENU_STR1 " irem m72(Z04)"
	#define MENU_STR2 "まだ重いです。"
#endif
#ifdef namcos86
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥⌒86(mame版)"
	#define MENU_STR2 "落ちないでくれええええぇ。"
#endif
#ifdef bubblesys
	#define MENU_STR1 "konami85 bubble system(Z04)"
	#define MENU_STR2 "沙羅Å蛇は、まだ重い"
#endif
#ifdef thunder_cross
	#define MENU_STR1 "konami88 THUNDER CROSS(Z01)"
	#define MENU_STR2 "音がオカシイ(;^;)"
#endif
#ifdef zerowing
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥⌒90(Z01)"
	#define MENU_STR2 "古き良き東亜に涙せよ        "
#endif
#ifdef raizing
	#define MENU_STR1 "Toaplan92/Raizing93(Z02)"
	#define MENU_STR2 "達人王/(魔法大作戦)"
#endif
#ifdef nmk
	#define MENU_STR1 "UPL91/NMK93 (Z02)"
	#define MENU_STR2 "やっぱおいしい処が動かない..."
#endif
#ifdef seta
	#define MENU_STR1 "SETA90/Atena91(Z04)"
	#define MENU_STR2 "地道にVERSION UP。"
#endif
#ifdef starforce
	#define MENU_STR1 "TEHKAN(tecmo)84 Star Force(Z02)"
	#define MENU_STR2 "□業務,△高橋"
#endif
#ifdef terracre
	#define MENU_STR1 "Terra Cresta  Nichibutsu85(Z01)"
	#define MENU_STR2 "WINGER復活?"
#endif
#ifdef mappy
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥⌒83 MAPPY(Z05)"
	#define MENU_STR2 "phozonとか動かないし"
#endif
#ifdef pacland
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥⌒PAC-LAND(Z03)"
	#define MENU_STR2 "JUMPは左.前後は○と□"
#endif
#ifdef air_buster
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥90(Z02)"
	#define MENU_STR2 "AIR BUSTER / DJ-BOY"
#endif

#ifdef darwin4078
	#define MENU_STR1 "DECO86 DARWIN4078(Z02)"
	#define MENU_STR2 /*"goe 氏に感謝! "*/"B-Wings mame112以降へ"
#endif

#ifdef ninjya_kun2
	#define MENU_STR1 "￢⇒⇔∀ vs ＣＡＰＣＯＭ 忍者くん(A03)"
	#define MENU_STR2 "  園児約２名 +"
#endif
#ifdef gun_smoke
	#define MENU_STR1 "ガンスモーク(A00)[□][□+×][×][×+○][○]５方向"
	#define MENU_STR2 ""
#endif // gun_smoke
#ifdef crazy_climber
	#define MENU_STR1 "クレージークライマー(A00)"
	#define MENU_STR2 "アナログキーは無効です。"
#endif // crazy_climber
#ifdef makai_mura
	#define MENU_STR1 "CAPCOM85 魔界村(A01)"
	#define MENU_STR2 "喰い尽くせ!! (?)"
#endif

#ifdef chuuka_taisen
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥ 88 chuuka_taisen(Z00)"
	#define MENU_STR2 "こっそり中華大戦"
#endif
#ifdef srd08
	#define MENU_STR1 "DECO86 Super Real Darwin(Z00)"
	#define MENU_STR2 "動かん"
#endif
#ifdef fantasy_zone
	#define MENU_STR1 "￢⇒⇔∀86 FantasyZone(A02)"
	#define MENU_STR2 "FZ以外 あとはいら<略>"
#endif
//#ifdef outrun
//	#define MENU_STR1 "￢⇒⇔∀86 OutRun(Z00)"
//	#define MENU_STR2 "マークIII版と思えば 遊べ<謎>"
//#endif
#ifdef turbo_outrun
	#define MENU_STR1 "￢⇒⇔∀86 turbo/OutRun(A02)"
	#define MENU_STR2 "アク△ブレ○ギア×タボ□"
#endif
#ifdef cotton
	#define MENU_STR1 "wb34_test0"
	#define MENU_STR2 "起動方法は特殊だから、説明書読め"
#endif

#ifdef mario_bros
	#define MENU_STR1 "Nintendo1983 Mario Brothers."
	#define MENU_STR2 "企画 かよ & 製作"
#endif

#ifdef pacman
	#define MENU_STR1 "￢⇒⇔∀1982 Pengo."
	#define MENU_STR2 "ペンゴ以外はオマケですぅｗｗｗ   "
#endif // pacman

#ifdef taitoB
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥ Ｂ(A00)"
	#define MENU_STR2 "僕たちは、待っていたんだ...   "
#endif // taitoB

#ifdef darius
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥ ＤＡＲＩＵＳ(A00)"
	#define MENU_STR2 "３画面だお...   "
#endif // darius

#ifdef ninjya_warriors
	#define MENU_STR1 "￢⇒⇔∀∃∠⊥ ニンジャウォーリアーズ ＤＡＲＩＵＳII"
	#define MENU_STR2 " "
#endif // ninjya_warriors


#ifndef MENU_STR1
	#define MENU_STR1 "∽∝∵∫∬ MAME DEATHRASH!!!(0.97s)" //開
#endif
#ifndef MENU_STR2
	#define MENU_STR2 ""//"もっと速くないと遊べん！"
#endif
#define APP_TITLE MENU_STR1 NOW_DATE MENU_STR2 DEVELOPPER

// ----------------------------------------
static SceCtrlData/*ctrl_data_t*/ ctl;
// ----------------------------------------
//static	unsigned long is_PAD;
/* triggered based the pad input */
//[is_#] now input key.
//[rs_#] old key.(before key)
//[ps_#] is pressed.(or pulled)[trigged key the edge.](for trigger check)
//[cs_#] count holding key at time.(fps base counted)(for use auto repeat)

static unsigned int is_PAD; // read pad raw data.

//atic u8 is_E,rs_E,ps_E;//,cs_E;	// E: Select//disabled
//atic u8 is_S,rs_S,ps_S;//,cs_S;	// S: Start
static u8 is_U,rs_U,ps_U,cs_U;	// U: Up
static u8 is_J,rs_J,ps_J,cs_J;	// J: Right (see your keyboard)
static u8 is_D,rs_D,ps_D,cs_D;	// D: Down
static u8 is_F,rs_F,ps_F,cs_F;	// F: Left	(see your keyboard)
static u8 is_L,rs_L,ps_L;//,cs_L;	// L: L-trigger
static u8 is_R,rs_R,ps_R;//,cs_R;	// R: R-trigger
static u8 is_A,rs_A,ps_A;//,cs_A;	// A: triangle
static u8 is_O,rs_O,ps_O;//,cs_O;	// O: Circle
static u8 is_X,rs_X,ps_X;//,cs_X;	// X: Cross
static u8 is_Q,rs_Q,ps_Q;//,cs_Q;	// Q: Square
// ----------------------------------------
static void pad_read(void)
{
static u8 ccc; //counter
u8 ttt; // work
	ccc++; //count up
	ccc&=1; // masked on/off
	//key = Read_Key();
//	sceCtrlReadBufferPositive(&ctl,1);
	sceCtrlPeekBufferPositive(&ctl,1);
UINT32 buttons;
	buttons=ctl.Buttons;

// 0123456789abcdef
// Uuuu--------dddD

	/* pached the analog stick to digitily sense. */
	ttt=ctl.Ly;//analog[CTRL_ANALOG_Y];
		 if(ttt < 0x10) {		  buttons |= PSP_CTRL_UP;		/* UP HIGH */ }
	else if(ttt < 0x40) { if(ccc) buttons |= PSP_CTRL_UP;		/* UP LOW  */ }
	else if(ttt < 0xc0) { ; /* NONE */ }
	else if(ttt < 0xf0) { if(ccc) buttons |= PSP_CTRL_DOWN; 	/* DOWN LOW  */ }
	else				{		  buttons |= PSP_CTRL_DOWN; 	/* DOWN HIGH */ }

	ttt=ctl.Lx;//analog[CTRL_ANALOG_X];
		 if(ttt < 0x10) {		  buttons |= PSP_CTRL_LEFT; 	/* LEFT HIGH */ }
	else if(ttt < 0x40) { if(ccc) buttons |= PSP_CTRL_LEFT; 	/* LEFT LOW  */ }
	else if(ttt < 0xc0) { ; /* NONE */ }
	else if(ttt < 0xf0) { if(ccc) buttons |= PSP_CTRL_RIGHT;	/* RIGHT LOW  */ }
	else				{		  buttons |= PSP_CTRL_RIGHT;	/* RIGHT HIGH */ }

	/* before buttons sense */
//	rs_E=is_E;
//	rs_S=is_S;
	rs_L=is_L;
	rs_R=is_R;

	rs_U=is_U;
	rs_J=is_J;
	rs_D=is_D;
	rs_F=is_F;

	rs_A=is_A;
	rs_O=is_O;
	rs_X=is_X;
	rs_Q=is_Q;

	/* now buttons sense */
	is_PAD=buttons;
//	is_E=(is_PAD & PSP_CTRL_SELECT);
//	is_S=(is_PAD & PSP_CTRL_START);
	is_L=(is_PAD & PSP_CTRL_LTRIGGER)?1:0;
	is_R=(is_PAD & PSP_CTRL_RTRIGGER)?1:0;

	is_U=(is_PAD & PSP_CTRL_UP);
	is_J=(is_PAD & PSP_CTRL_RIGHT);
	is_D=(is_PAD & PSP_CTRL_DOWN);
	is_F=(is_PAD & PSP_CTRL_LEFT);

	is_A=(is_PAD & PSP_CTRL_TRIANGLE)?1:0;
	is_O=(is_PAD & PSP_CTRL_CIRCLE)?1:0;
	is_X=(is_PAD & PSP_CTRL_CROSS)?1:0;
	is_Q=(is_PAD & PSP_CTRL_SQUARE)?1:0;

	/* now pushed or pulled buttons(triggered check) */
//	ps_E=(is_E != rs_E);
//	ps_S=(is_S != rs_S);
	ps_L=(is_L != rs_L);
	ps_R=(is_R != rs_R);

	ps_U=(is_U != rs_U);
	ps_J=(is_J != rs_J);
	ps_D=(is_D != rs_D);
	ps_F=(is_F != rs_F);

	ps_A=(is_A != rs_A);
	ps_O=(is_O != rs_O);
	ps_X=(is_X != rs_X);
	ps_Q=(is_Q != rs_Q);

	/* key pressed time counter at a frame */ /* 要するにオートリピート */
	if(is_U) cs_U++;	if(ps_U) cs_U=0;
	if(is_D) cs_D++;	if(ps_D) cs_D=0;
	if(is_F) cs_F++;	if(ps_F) cs_F=0;
	if(is_J) cs_J++;	if(ps_J) cs_J=0;
//	if(is_E) cs_E++;	if(ps_E) cs_E=0;//disabled
}
//unsigned long Read_Key(void);
//u32 new_pad;


/////////////////Filer
#define PATHLIST_H 20
static int dlist_num;
static int dlist_start;
int dlist_curpos;


extern SETTING setting;

//#define REPEAT_TIME 0x40000
//static unsigned long control_bef_paddata	= 0;
//static unsigned long control_bef_tick = 0;

//unsigned long Read_Key(void) {
//	/*ctrl_data_t*/SceCtrlData paddata;
//
//	//sceCtrlReadBufferPositive(&paddata,1);
//	sceCtrlPeekBufferPositive(&paddata,1);
//	if (buttons == control_bef_paddata) {
//		if ((paddata.TimeStamp - control_bef_tick) > REPEAT_TIME) {
//			return control_bef_paddata;
//		}
//		return 0;
//	}
//	control_bef_paddata  = buttons;
//	control_bef_tick = paddata.TimeStamp;
//	return control_bef_paddata;
//}

void Get_DriverList(void)
{
	dlist_num = 0;

	while (drivers[dlist_num])
		dlist_num++;

	dlist_start  = 0;
	dlist_curpos = 0;
}


/////
static void BitBlt(int x, int y, int w, int h, /*int mag,*/ const u16 *d)
{
	u16 *vptr, *src;
	int xx, yy;//, mx, my;
	const u16 *dst;

//	vptr = pgGetVramAddr(x,y);
	vptr = psp_frame_addr(draw_frame, x, y);

	for (yy = 0; yy < h; yy++)
	{
	//	for (my = 0; my < mag; my++)
		{
			src = vptr;
			dst = d;
			for (xx = 0; xx < w; xx++)
			{
			//	for (mx = 0; mx < mag; mx++)
				{
					*src++ = *dst;
				}
				dst++;
			}
			vptr += BUF_WIDTH;
		}
		d += w;
	}
}


/////
int bBitmap;
u16 bgBitmap[480*272];
static void psp_frame(const char *msg0, const char *msg1)
{
	if (bBitmap)/* 壁紙 */
	{	BitBlt(0, 0, 480, 272, /*1,*/ bgBitmap);}
	else/* 壁紙なし */
	{	psp_fill_frame(draw_frame, CNVCOL15TO32(setting.color[0]));}
//	{	pgFillvram(setting.color[0/*2*/]);}
	psp_print(0, 0, setting.color[3], APP_TITLE);
	// メッセージなど
	if (msg0) psp_print(17, 14, setting.color[2], msg0);
	// 枠
#if 1
	/*pgDrawFrame*/psp_box_frame(draw_frame, 17-16, 25, 463+16, 248+10, setting.color[1]);
	/*pgDrawFrame*/psp_box_frame(draw_frame, 18-16, 26, 462+16, 247+10, setting.color[1]);
#else
	{
		const rectangle tbounds={	17-16,	463+16, 25, 248+10};
		psp_box_rect( draw_frame,	setting.color[1], &tbounds);
		tbounds.min_x++;	tbounds.max_x--;
		tbounds.min_y++;	tbounds.max_y--;
		psp_box_rect( draw_frame,	setting.color[1], &tbounds);
	}
#endif
	// 操作説明
	if(msg1) psp_print(17, 252+10, setting.color[2], msg1);
}


void Draw_All(void)
{                                                         //○：実行 Ｌ：設定メニュー △：エミュレータの終了
	psp_frame(/*NULL*/drivers[dlist_curpos]->name/*mes*/, "Controls>> circle : run game ,LTrigger: config ,triangle: exit");

    long freeMem;
    long largestFreeBlock;
    int sizeFreeMemMb;
    int largestBlockMb;
    char memInfo[80];

    freeMem = sceKernelTotalFreeMemSize();//total free mem available
    largestFreeBlock = sceKernelMaxFreeMemSize();//largest free mem block available

    sizeFreeMemMb = (int)(freeMem / 1024)/1024;
    largestBlockMb = (int)(largestFreeBlock / 1024)/1024;

    sprintf(memInfo,"<<freemem='%d' mb, largest-freememblock='%d' mb>>",sizeFreeMemMb,largestBlockMb);


	psp_print(110, 27, setting.color[3], memInfo);

	int i;
	// ゲームリスト
	i = dlist_start;
	while (i < (dlist_start+PATHLIST_H))
	{
		if (i<dlist_num) {
		    int col;
			col = setting.color[(i==dlist_curpos)?3:2];
#ifdef namcoNA
			/*namcoNA*/
			psp_print(((4-3)*8)-2, (((i-dlist_start)+4)*10)+((i>10)?5:0), col, drivers[i]->description);
#else
	#ifdef namcos86
			psp_print(((4-3)*8)-2, ((i-dlist_start)+3)*14, col, drivers[i]->description);
	#else
//			psp_print(((4-3)*8)-2, ((i-dlist_start)+2)*10, col, drivers[i]->name);
			psp_print(((4-3)*8)-2, ((i-dlist_start)+4)*10, col, drivers[i]->description);
	#endif
#endif
		}
		i++;
	}
	v_sync();
//	pgScreenFlip();
	psp_flip_screen(/*1*/);
}


int Confirm_Control(void)
{
//	unsigned long key;
//	int pressed;

//	pressed = 0;
	while (psp_loop /*&& !pressed*/)
	{
		//pressed = 1;
		v_sync();
		pad_read();//key = Read_Key();

		if (ps_O && is_O)
		{
			return 1;
		}
		if (ps_X && is_X)
		{
			return 0;
		}
		//pressed = 0;
	}
	return 0;
}


int Control(void)
{
	int pressed = 0;

	while (psp_loop && !pressed)
	{
		pressed = 1;
		v_sync();
		pad_read();//key = Read_Key();
	//	if (is_PAD != 0) break;

	//#define REPEAT_TIME_W 6
	//	if ( ((ps_U ||(REPEAT_TIME_W<cs_U)) && is_U) ){
		if ((is_U)&&((6<cs_U)&&(0==(cs_U&7))||(ps_U)))
		{
			if (dlist_curpos > 0) { dlist_curpos--; 	}
			if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
		}
		else if ((is_D)&&((6<cs_D)&&(0==(cs_D&7))||(ps_D)))
		{
			if (dlist_curpos < (dlist_num-1)) { 	dlist_curpos++; 	}
			if (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
		}
		else if (ps_F && is_F)
		{
			dlist_curpos -= PATHLIST_H;
			if (dlist_curpos <	0)			{ dlist_curpos = 0; 		  }
			if (dlist_curpos < dlist_start) { dlist_start = dlist_curpos; }
		}
		else if (ps_J && is_J)
		{
			dlist_curpos += PATHLIST_H;
			if (dlist_curpos >= dlist_num) { dlist_curpos = dlist_num-1; }
			while (dlist_curpos >= (dlist_start+PATHLIST_H)) { dlist_start++; }
		}
		else if (ps_A && is_A)
		{
			//psp_frame("ALO! また遊んでね" DEVELOPPER, "×：やっぱキャンセル  ○：終了" );
			psp_frame(""," circle : confirm   cross : cancel" );
			psp_print(40,120, setting.color[3], " WTF!!!??? QUITTING ALREADY??!!! press Circle to confirm");
			v_sync();
		//	pgScreenFlip();
			psp_flip_screen(/*1*/);
			if (Confirm_Control()) psp_loop = 0;
		}
		else if (ps_O && is_O)
		{
			return 2;
		}
		else if (ps_L && is_L)
		{
			return 3;
		}
		else
		{
			pressed = 0;
		}
	}
	return 0;
}



/* REPEAT_TIME_W: オートリピートがかかるまでの時間 */
#define REPEAT_TIME_W 6

#define MENU2_Y_OFFS  (5-1-1)
#define MENU2_X_SHIFT (4-2)
#define MENU2_X_OFFS  (MENU2_X_SHIFT*8)
#define MENU2B_X_OFFS ((MENU2_X_SHIFT+10)*8)
#define MENU2C_X_OFFS ((MENU2_X_SHIFT-1)*8)


void Draw_Confirm(void)
{
	psp_frame(drivers[dlist_curpos]->name, "CIRCLE : CONFIRM , CROSS : CANCEL  ** Argentina campion 2010!!!**");
	psp_print(MENU2C_X_OFFS, ( 9*10), setting.color[3], drivers[dlist_curpos]->description);
	psp_print(MENU2B_X_OFFS, (12*10), setting.color[3], "Press circle to run game");//"を実行します。"
	v_sync();
//	pgScreenFlip();
	psp_flip_screen(/*1*/);
}


//////////////////



//char *getCurDir(void);
SETTING setting;

void save_config(void)
{
	char CfgPath[MAX_PATH];
	char *p;

	strcpy(CfgPath, getCurDir());
	p = strrchr(CfgPath, '/')+1;
	strcpy(p, "pspmame.cfg");

	int fd;
	fd = sceIoOpen(CfgPath, PSP_O_CREAT|PSP_O_WRONLY|PSP_O_TRUNC, 0777);
	if(fd>=0){
		sceIoWrite(fd, &setting, sizeof(setting));
		sceIoClose(fd);
	}
}

/*	*/
#define SET_ON	(1)
#define SET_OFF (0)

#define SET_FSKIP_07   (8)
#define SET_FSKIP_06   (7)
#define SET_FSKIP_05   (6)
#define SET_FSKIP_04   (5)
#define SET_FSKIP_03   (4)
#define SET_FSKIP_02   (3)
#define SET_FSKIP_01   (2)
#define SET_FSKIP_00   (1)
#define SET_FSKIP_AUTO (0)

#define PSP222MHz (0)
#define PSP266MHz (1)
#define PSP300MHz (2)
#define PSP333MHz (3)


/* set default to fskip07 */
#ifndef SET_DEFAULT_FSKIPS
	#define SET_DEFAULT_FSKIPS SET_FSKIP_07
#endif // SET_DEFAULT_FSKIPS
/* set default to off the video syncronization for draw. */
#define SET_DEFAULT_VSYNC_ON_OFF SET_ON

/* set default speed max limitage. */
#define SET_DEFAULT_LIMIT_ON_OFF SET_ON

/* set default to 22050 */
#ifndef SET_DEFAULT_SOUND_FREQ
	#define SET_DEFAULT_SOUND_FREQ SND_22050
#endif // SET_DEFAULT_SOUND_FREQ

/* set default PSP cpu clock 333M[Hz] */
#ifndef SET_DEFAULT_PSP_CLOCK
	#define SET_DEFAULT_PSP_CLOCK PSP333MHz
#endif // SET_DEFAULT_PSP_CLOCK


//#define SET_DEFAULT_SCREEN SCR_X1
#define SET_DEFAULT_SCREEN SCR_STRETCH

void load_config(void)
{
//	int i;
	char CfgPath[MAX_PATH];
	char *p;

	sprintf(CfgPath, "%sPSPMAME.CFG", getCurDir());
	remove(CfgPath);

	memset(&setting, 0, sizeof(setting));

	strcpy(CfgPath, getCurDir());
	p = strrchr(CfgPath, '/')+1;
	strcpy(p, "pspmame.cfg");

	int fd;
	fd = sceIoOpen(CfgPath,PSP_O_RDONLY, 0777);
	if (fd >= 0)
	{
		sceIoRead(fd, &setting, sizeof(setting));
		sceIoClose(fd);
		/* check if ignore value, forced in range. */
//		#if (0!=LINK_ROT)
//		if(/*setting.screensize<0 ||*/ setting.screensize > (SCR_MAX-1)) setting.screensize = SET_DEFAULT_SCREEN;
//		#endif //(0!=LINK_ROT)
		if(setting.frameskip > /*9*/60) 						setting.frameskip=/*0*/60;
		//if(setting.sound_rate<0 || setting.sound_rate>2)		setting.sound_rate = /*0*/2;
		if (setting.sound_rate > (SND_MAX-1))					setting.sound_rate = (SND_MAX-1);
	//	setting.sound_rate &=7;
		setting.cpu_clock  &=3;
		if(setting.bgbright<0 || setting.bgbright>100)			setting.bgbright=100;
		if(!strcmp(setting.vercnf, CONFIG_VER))
			return;
	}

	strcpy(setting.vercnf, CONFIG_VER);
/*	#if (0!=LINK_ROT)
	setting.screensize		= SET_DEFAULT_SCREEN;
	#endif //(0!=LINK_ROT)*/
	setting.frameskip		= SET_DEFAULT_FSKIPS;
	setting.vsync_ON_OFF	= SET_DEFAULT_VSYNC_ON_OFF;
	setting.limit_ON_OFF	= SET_DEFAULT_LIMIT_ON_OFF;
	setting.show_fps_ON_OFF = SET_OFF;
//	setting.sound_ON_OFF	= SET_ON;
	setting.sound_rate		= SET_DEFAULT_SOUND_FREQ;/*default sound rate 44100 */

	setting.color[0] = DEF_COLOR0;
	setting.color[1] = DEF_COLOR1;
	setting.color[2] = DEF_COLOR2;
	setting.color[3] = DEF_COLOR3;
	setting.bgbright = 100;/* default back bitmap brightness. */

	setting.cpu_clock = SET_DEFAULT_PSP_CLOCK;/* 3==psp speed 333[MHz] */
}

/*static*/ void load_menu_bg(void)
{
	char BgPath[MAX_PATH];
	char *p;

	strcpy(BgPath, getCurDir());
	p = strrchr(BgPath, '/')+1;
	strcpy(p, "MENU.BMP");

	int fd;
	fd = sceIoOpen(BgPath, PSP_O_RDONLY, 0777);
	if(fd>=0){
		#define my_BMP_HEADER_SIZE (0x36)
		#define my_BMP_FILE_SIZE (480*272*3+my_BMP_HEADER_SIZE)
		static u8 menu_bg_buf[my_BMP_FILE_SIZE];
		sceIoRead(fd, menu_bg_buf, my_BMP_FILE_SIZE);
		sceIoClose(fd);
		#undef my_BMP_FILE_SIZE

		u8 *menu_bg;
		u16 *vptr;
		menu_bg = menu_bg_buf + my_BMP_HEADER_SIZE;
		#undef my_BMP_HEADER_SIZE
		vptr=bgBitmap;
		u16 y;
		for(y=0; y<272; y++){
			u16 x;
			for(x=0; x<480; x++){
				*vptr= (((	/*b*/(*(menu_bg +((271-y)*480 +x)*3   )) &0xf8)<<7) |
						((	/*g*/(*(menu_bg +((271-y)*480 +x)*3 +1)) &0xf8)<<2) |
						(	/*r*/(*(menu_bg +((271-y)*480 +x)*3 +2))	   >>3));
				vptr+=/*2*/1;
			}
		}
		bBitmap = 1;
	}else{
		bBitmap = 0;
	}
}



// 半透明処理
static u16 rgbTransp(u16 fgRGB, u16 bgRGB, int alpha)
{
//	unsigned int R, G, B;

	alpha *= 256;
	alpha /= 100;

return(MAKECOL15(
/*R =*/ (((GETR15(fgRGB) * (alpha)) + (GETR15(bgRGB) * (256 - alpha))) >> 9)/*;*/
/*G =*/,(((GETG15(fgRGB) * (alpha)) + (GETG15(bgRGB) * (256 - alpha))) >> 9)/*;*/
/*B =*/,(((GETB15(fgRGB) * (alpha)) + (GETB15(bgRGB) * (256 - alpha))) >> 9)/*;*/
));

//	R = coltbl[fgR][bgR][alpha/10];
//	G = coltbl[fgG][bgG][alpha/10];
//	B = coltbl[fgB][bgB][alpha/10];

//	return MAKECOL15(R, G, B);
}

/*static*/ void bgbright_change(void)
{
	u16 *vptr,rgb;
	int i;

//	load_menu_bg();
	vptr=bgBitmap;
	for(i=0; i<272*480; i++)
	{
		rgb = *vptr;
		*vptr = rgbTransp(rgb, 0x0000, setting.bgbright);
		vptr++;
	}
}


void psp_colorconfig(void)
{
	enum
	{
		COLOR0_R,		COLOR0_G,		COLOR0_B,
		COLOR1_R,		COLOR1_G,		COLOR1_B,
		COLOR2_R,		COLOR2_G,		COLOR2_B,
		COLOR3_R,		COLOR3_G,		COLOR3_B,
		BG_BRIGHT,		INIT,			EXIT,
		MENU2_MAX
	};
	char tmp[64];
	int color[4][3];
	int sel=0, y, i;
	u8 sel_count=0;
	u8 reload_bitmap = 0;

	memset(color, 0, sizeof(int)*4*3);
	for (i=0; i<4; i++)
	{
		color[i][0] = GETR15(setting.color[i]);
		color[i][1] = GETG15(setting.color[i]);
		color[i][2] = GETB15(setting.color[i]);
	}

	while (psp_loop)
	{
		v_sync();
		pad_read();//new_pad =Read_Key();
		if ( ((ps_J ||(REPEAT_TIME_W<cs_J)) && is_J) )
		{
			if (sel == BG_BRIGHT)
			{
				setting.bgbright++;
				if (setting.bgbright > 100) setting.bgbright = 0;
				reload_bitmap = bBitmap;
			}
			else if (sel >= COLOR0_R && sel <= COLOR3_B)
			{
				if (color[sel/3][sel%3] < 255/*31*/)
				{	color[sel/3][sel%3]++;}
			}
		}
		else if ( ((ps_F ||(REPEAT_TIME_W<cs_F)) && is_F) )
		{
			if (sel == BG_BRIGHT)
			{
				setting.bgbright--;
				if (setting.bgbright < 0) setting.bgbright = 100;
				reload_bitmap = bBitmap;
			}
			else if (sel >= COLOR0_R && sel <= COLOR3_B)
			{
				if (color[sel/3][sel%3] > 0)
				{	color[sel/3][sel%3]--;}
			}
		}
		else if ( ((ps_U ||(REPEAT_TIME_W<cs_U)) && is_U) )
		{
			sel--;
			if (sel < 0) sel = (MENU2_MAX-1);
			//sel_count = 0;
		}
		else if ( ((ps_D ||(REPEAT_TIME_W<cs_D)) && is_D) )
		{
			sel++;
			if (sel > (MENU2_MAX-1)) sel = 0;
			//sel_count = 0;
		}
		#if (1==USE_COLOR_MENU_X_BUTTON)
		else if( ps_X && is_X )
		{
			break;/*戻る*/
		}
		#endif //(1==USE_COLOR_MENU_X_BUTTON)
		else if( ps_O && is_O ){
			if (sel == EXIT){ break;}
			else if (sel == INIT)
			{
				color[0][0] = GETR15(DEF_COLOR0);
				color[0][1] = GETG15(DEF_COLOR0);
				color[0][2] = GETB15(DEF_COLOR0);
				color[1][0] = GETR15(DEF_COLOR1);
				color[1][1] = GETG15(DEF_COLOR1);
				color[1][2] = GETB15(DEF_COLOR1);
				color[2][0] = GETR15(DEF_COLOR2);
				color[2][1] = GETG15(DEF_COLOR2);
				color[2][2] = GETB15(DEF_COLOR2);
				color[3][0] = GETR15(DEF_COLOR3);
				color[3][1] = GETG15(DEF_COLOR3);
				color[3][2] = GETB15(DEF_COLOR3);
				setting.bgbright = 100;
				reload_bitmap = bBitmap;
			}
		}

		if (reload_bitmap)
		{
			load_menu_bg();
			bgbright_change();
			reload_bitmap = 0;
		}

		for (i = 0; i < 4; i++)
		{	setting.color[i] = MAKECOL15(color[i][0], color[i][1], color[i][2]);}

		sel_count++;
		sel_count &=15;

		if (sel >= COLOR0_R && sel <= BG_BRIGHT)
		{	psp_frame(NULL, "←→：値の変更"
			#if (1==USE_COLOR_MENU_X_BUTTON)
				"  ×：メインメニューに戻る"
			#endif //(1==USE_COLOR_MENU_X_BUTTON)
			);
		} else if (sel == INIT)
		{	psp_frame(NULL, "○：設定を初期化する"
			#if (1==USE_COLOR_MENU_X_BUTTON)
				"  ×：メインメニューに戻る"
			#endif //(1==USE_COLOR_MENU_X_BUTTON)
			);
		} else
		{	psp_frame(NULL, "○"
			#if (1==USE_COLOR_MENU_X_BUTTON)
				"×"
			#endif //(1==USE_COLOR_MENU_X_BUTTON)
				"：メインメニューに戻る"
			);
		}

#define MENU2_Y_SPACER 11
#define DDD (0.90)
		int color_rrr;
		int color_ggg;
		int color_bbb;
		int rr1,gg1,bb1;
		int rr2,gg2,bb2;
		rr1=GETR15(setting.color[3]);	rr2=rr1*DDD;
		gg1=GETG15(setting.color[3]);	gg2=gg1*DDD;
		bb1=GETB15(setting.color[3]);	bb2=bb1*DDD;
		color_rrr = MAKECOL15(rr1, gg2, bb2);
		color_ggg = MAKECOL15(rr2, gg1, bb2);
		color_bbb = MAKECOL15(rr2, gg2, bb1);
#undef DDD
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS	 )*(MENU2_Y_SPACER)), color_rrr,"COLOR0 R:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 1)*(MENU2_Y_SPACER)), color_ggg,"COLOR0 G:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 2)*(MENU2_Y_SPACER)), color_bbb,"COLOR0 B:");
		//3
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 4)*(MENU2_Y_SPACER)), color_rrr,"COLOR1 R:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 5)*(MENU2_Y_SPACER)), color_ggg,"COLOR1 G:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 6)*(MENU2_Y_SPACER)), color_bbb,"COLOR1 B:");
		//7
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 8)*(MENU2_Y_SPACER)), color_rrr,"COLOR2 R:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+ 9)*(MENU2_Y_SPACER)), color_ggg,"COLOR2 G:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+10)*(MENU2_Y_SPACER)), color_bbb,"COLOR2 B:");
		//11
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+12)*(MENU2_Y_SPACER)), color_rrr,"COLOR3 R:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+13)*(MENU2_Y_SPACER)), color_ggg,"COLOR3 G:");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+14)*(MENU2_Y_SPACER)), color_bbb,"COLOR3 B:");
		//15
		sprintf(tmp, "BG BRIGHT:%3d%%", setting.bgbright);
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+16)*(MENU2_Y_SPACER)), setting.color[3],tmp);
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+17)*(MENU2_Y_SPACER)), setting.color[3],"Initialize");
		psp_print(MENU2_X_OFFS, ((MENU2_Y_OFFS+18)*(MENU2_Y_SPACER)), setting.color[3],"Return to Main Menu");

		y=MENU2_Y_OFFS;
		for(i=0; i<12; i++, y++)
		{
			if(i!=0 && i%3==0) y++;
			//_itoa(color[i/3][i%3], tmp);
			sprintf(tmp, "%d", color[i/3][i%3]);
			psp_print(MENU2B_X_OFFS,y*(MENU2_Y_SPACER),setting.color[3],tmp);
		}

		if (sel_count < 12)
		{
			y=MENU2_Y_OFFS + sel;
			if(sel>=COLOR1_R) y++;
			if(sel>=COLOR2_R) y++;
			if(sel>=COLOR3_R) y++;
			if(sel>=BG_BRIGHT) y++;
			psp_print(MENU2C_X_OFFS,y*(MENU2_Y_SPACER),setting.color[3],">");
		}
		v_sync();
	//	pgScreenFlip();
		psp_flip_screen(/*1*/);
	}
}



#define STR_DEFAULT " ""[default]"

const char *cpu_clocks[] = {
	"222MHz"
	#if (PSP222MHz==SET_DEFAULT_PSP_CLOCK)
	STR_DEFAULT
	#endif
	,"266MHz"
	#if (PSP266MHz==SET_DEFAULT_PSP_CLOCK)
	STR_DEFAULT
	#endif
	,"300MHz"
	#if (PSP300MHz==SET_DEFAULT_PSP_CLOCK)
	STR_DEFAULT
	#endif
	,"333MHz"
	#if (PSP333MHz==SET_DEFAULT_PSP_CLOCK)
	STR_DEFAULT
	#endif
};


#define MENU1_Y_OFFS (5-1-1)

extern void mame_set_to_options_samplerate(void);/* src/psp/config.c */
void psp_menu(void)
{
	enum
	{
//		ITEM_SOUND=0,
		ITEM_SOUND_RATE,
//---space---
/*#if (0!=LINK_ROT)
		ITEM_SCREEN_SIZE,
#endif //(0!=LINK_ROT)*/
        ITEM_SCREEN_SIZE,
		ITEM_FRAME_SKIP,
		ITEM_VSYNC,
		ITEM_LIMIT_SPEED,
		ITEM_SHOW_FPS,
//---space---
		ITEM_CPU_CLOCK,
		ITEM_COLOR_CONFIG,
//---space---
		ITEM_CONTINUE,
//---end---
		ITEM_MENU1_MAX
	};
	char tmp[64];
	static u8 sel=0;
	u8 y;
	u8 sel_count=0;
//	int ret;
	//int bSave, fd, romsize, ramsize;
	//char *p;

	while (psp_loop)
	{
		v_sync();
		pad_read();//new_pad =Read_Key();
		if( ps_F && is_F )//atras
		{
			switch (sel)
			{
		//	case ITEM_SOUND:		setting.sound_ON_OFF	= SET_OFF; break;
			case ITEM_SOUND_RATE:	if (setting.sound_rate < (SND_MAX-1)) setting.sound_rate++; break;
			case ITEM_FRAME_SKIP:	if (setting.frameskip  > 0) setting.frameskip--;	break;
			case ITEM_SCREEN_SIZE:	if (setting.screensize > 0) setting.screensize--; break;

			case ITEM_VSYNC:		setting.vsync_ON_OFF	= SET_OFF; break;
			case ITEM_LIMIT_SPEED:	setting.limit_ON_OFF	= SET_OFF; break;
			case ITEM_SHOW_FPS: 	setting.show_fps_ON_OFF = SET_OFF; break;
			case ITEM_CPU_CLOCK:	if (setting.cpu_clock  > 0) setting.cpu_clock--;	break;
			}
		}
		else if( ps_J && is_J )//adelante
		{
			switch (sel)
			{
		//	case ITEM_SOUND:		setting.sound_ON_OFF	= SET_ON; break;
			case ITEM_SOUND_RATE:	if (setting.sound_rate >  0) setting.sound_rate--; break;
			case ITEM_FRAME_SKIP:	if (setting.frameskip  < (60+1)) setting.frameskip++;  break;
			case ITEM_SCREEN_SIZE:	if (setting.screensize < (SCR_MAX-1)) setting.screensize++; break;
			case ITEM_VSYNC:		setting.vsync_ON_OFF	= SET_ON; break;
			case ITEM_LIMIT_SPEED:	setting.limit_ON_OFF	= SET_ON; break;
			case ITEM_SHOW_FPS: 	setting.show_fps_ON_OFF = SET_ON; break;
			case ITEM_CPU_CLOCK:	if (setting.cpu_clock  <  3) setting.cpu_clock++;  break;
			}
		}
		else if ( ((ps_U ||(REPEAT_TIME_W<cs_U)) && is_U) )
		{
			sel--;
			if (sel > ITEM_MENU1_MAX) sel=(ITEM_MENU1_MAX-1);
			//sel_count = 0;
		}
		else if ( ((ps_D ||(REPEAT_TIME_W<cs_D)) && is_D) )
		{
			sel++;
			if (sel > (ITEM_MENU1_MAX-1)) sel = 0;
			//sel_count = 0;
		}
		else if( ps_O && is_O )
		{
			switch(sel)
			{
			case ITEM_SCREEN_SIZE:
                 break;

			case ITEM_COLOR_CONFIG:
				psp_colorconfig();
				break;
			case ITEM_CONTINUE: return;
			}
		}
		else if( ( ps_X && is_X ) || ( ps_L && is_L ) )
		{
			break;
		}

		{// sprintf(tmp, "selected item #%d",sel );
#if 0
/* 「説明」入れると「不安定」になるので「廃止」。 */
		const char *setumei[]={
			"OFFで、音なしです。",
			#if (0!=LINK_ROT)
			"画面の縦横です。([H]が横＝PSPは普通、[V]が縦＝PSPを回転)。",
			#endif //(0!=LINK_ROT)
			"フレームスキップです。00は「０フレームスキップ」、つまりフルフレーム描画。",
			"VSYNC入れると重い...orz",
			"デバッグ用FPS表\示。(表\示ルーチンが遅いので) 3%～5%程度遅くなるようです。",
			"そら333MHzに決まってます。",
			"メニューの「字の色」の設定です。",
			"ファイラー(ゲーム選択画面)に戻ります。",
			};
			strcpy(tmp, &setumei[sel][0] );
#endif
			if (sel < ITEM_COLOR_CONFIG)
				psp_frame(NULL/*tmp*/, "←→：値の変更  ×Ｌ：ゲーム選択画面に戻る");
			else if (sel == ITEM_COLOR_CONFIG)
				psp_frame(NULL/*tmp*/, "○：カラー設定メニューに移動  ×Ｌ：ゲーム選択画面に戻る");
			else
				psp_frame(NULL/*tmp*/, "○×Ｌ：ゲーム選択画面に戻る");
		}
#define MENU1_Y_SPACER 11
	//	if(setting.sound_ON_OFF)
	//		{	psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND)*(MENU1_Y_SPACER)), setting.color[3],"SOUND:        ON " STR_DEFAULT);}
	//	else{	psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND)*(MENU1_Y_SPACER)), setting.color[3],"SOUND:        OFF");}

	//	switch(setting.sound_rate)
	//	{
	//	case 0: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND 44100[Hz]" STR_DEFAULT );	break;
	//	case 1: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND 22050[Hz]"); break;
	//	case 2: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND 14700[Hz]"); break;
	//	case 3: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND 11025[Hz]"); break;
	//	case 4: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND  8820[Hz]"); break;
	//	case 5: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND  7350[Hz]"); break;
	//	case 6: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND  6300[Hz]"); break;
	//	case 7: psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],"SOUND  5512[Hz]"); break;
	//	}
		mame_set_to_options_samplerate();
		if(SND_OFF==setting.sound_rate)
			{	sprintf(tmp, "SOUND:  OFF    ");}
		else {sprintf(tmp,"SOUND:   %5d[Hz]", options.samplerate);}
	//	else {sprintf(tmp,"SOUND:   %5d[Hz]",(44100/(setting.sound_rate/*+1*/)));}
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SOUND_RATE)*(MENU1_Y_SPACER)), setting.color[3],tmp);
		//2
//#if (0!=LINK_ROT)
{
static const char *scr_names[] = {
	"[666] Auto stretch image",
	"[666] force stretch image 240-320",
//	"[H] stretch image",
	"[666] force stretch image 240-320 NO_ADJUST",
	"[666] force stretch image 256",
	"[666] force stretch image 288",
	"[666] force x1-normal",
//	"[H] stretch & aspect keep",
	"[V] rotate (who knows!?)",
//	"[V] rotate & stretch",
//	"[V] rotate & stretch & aspect keep",
};
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SCREEN_SIZE+(1))*(MENU1_Y_SPACER)), setting.color[3], "SCREEN MODE:  %s", scr_names[setting.screensize]);
}
//#endif// (0!=LINK_ROT)

	//	strcpy(tmp,"FRAME SKIP:  ");
	//	if (0 ==setting.frameskip)
	//	{	strcat(&tmp[strlen(tmp)-1], "Auto");}
	//	else
	//	{	tmp[strlen(tmp)-1] = setting.frameskip+'0'-1-1;}
		if (0 ==setting.frameskip)
		{	sprintf(tmp, "FRAME SKIP:   Auto"
		#if (SET_DEFAULT_FSKIPS==SET_FSKIP_AUTO)
			STR_DEFAULT
		#endif
			);
		}
		else if (SET_DEFAULT_FSKIPS ==setting.frameskip)
		{	sprintf(tmp, "FRAME SKIP:   %02d" STR_DEFAULT, (setting.frameskip - 1));}
		else
		{	sprintf(tmp, "FRAME SKIP:   %02d", (setting.frameskip - 1));}
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_FRAME_SKIP+(1))*(MENU1_Y_SPACER)), setting.color[3],tmp);


//		sprintf(tmp, "VSYNC: %s", (setting.vsync)?"ON":"OFF" STR_DEFAULT);
//		psp_print(x,y,setting.color[3],tmp);
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_VSYNC+(1))*(MENU1_Y_SPACER)), 			setting.color[3], setting.vsync_ON_OFF
			 ? "VSYNC:        " "ON"
			#if (SET_ON==SET_DEFAULT_VSYNC_ON_OFF)
			STR_DEFAULT
			#endif
			 : "VSYNC:        " "OFF"
			#if (SET_OFF==SET_DEFAULT_VSYNC_ON_OFF)
			STR_DEFAULT
			#endif
		);

		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_LIMIT_SPEED +(1)	)*(MENU1_Y_SPACER)),	setting.color[3], setting.limit_ON_OFF		? "LIMIT SPEED:  " "ON" STR_DEFAULT : "LIMIT SPEED:  " "OFF"			);
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_SHOW_FPS	+(1)	)*(MENU1_Y_SPACER)),	setting.color[3], setting.show_fps_ON_OFF	? "SHOW FPS:     " "ON" 			: "SHOW FPS:     " "OFF" /*STR_DEFAULT*/);
		//7
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_CPU_CLOCK	+(1+1)	)*(MENU1_Y_SPACER)),	setting.color[3],"CPU CLOCK:    " "%s",cpu_clocks[setting.cpu_clock]);
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_COLOR_CONFIG+(1+1)	)*(MENU1_Y_SPACER)),	setting.color[3],"COLOR CONFIG");
		//10
		psp_print(MENU2_X_OFFS, ((MENU1_Y_OFFS+ITEM_CONTINUE	+(1+1+1))*(MENU1_Y_SPACER)),	setting.color[3],"Go back to roms list");

		sel_count++;
		sel_count &=15;
		if(sel_count < 12)
		{
			y=MENU1_Y_OFFS + sel;
			if(sel > ITEM_SOUND_RATE)	{y++;}
			if(sel > ITEM_SHOW_FPS) 	{y++;}
			if(sel > ITEM_COLOR_CONFIG) {y++;}

			psp_print(MENU2C_X_OFFS,y*(MENU1_Y_SPACER),setting.color[3], ">");
		}
		v_sync();
	//	pgScreenFlip();
		psp_flip_screen(/*1*/);
	}

//	psp_clear_screen();
//	v_syncn(10);
}
