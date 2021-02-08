#define DIRECT15_USE_LOOKUP ( 1 )


#include "driver.h"

#include <stdio.h>
#include <string.h>
//#include <memory.h>

//#include "mame.h"//#include "mamedbg.h"
//#include "video/vector.h"

#include <math.h>
#include "blit.h"
//#include "syscall.h"

#define makecol(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)

typedef struct RGB
{
   unsigned char r, g, b;
   unsigned char filler;
} RGB;

// from sound.c
//extern void sound_update_refresh_rate(float newrate);

/* tweak values for centering tweaked modes */
//int center_x;
//int center_y;

//float screen_aspect = (480.0 / 272.0);

//#define USED_DUMMY 0
//#ifdef starforce
//#undef  USED_DUMMY
//#define USED_DUMMY 1
//#endif //starforce

//#if USED_DUMMY
//int keep_aspect = 1;
//#endif //USED_DUMMY

/* in msdos/sound.c */
//int msdos_update_audio( int throttle );

/* in msdos/input.c */
void poll_joysticks(void);

int video_flipx;
int video_flipy;
int video_swapxy;

int blitMode;

//all used for stretching purposes
int masterVertStretch;
int masterHorzStretch;
int masterHorizStretchRemain;

int masterScreenOffset;

static void bitblit_dummy( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh/*, int dx, int dy*/ );
void ( *bitblit )( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh/*, int dx, int dy*/ ) = bitblit_dummy;

//extern const char *g_s_resolution;
//int gfx_depth;
//static int video_depth;
static int video_attributes;
static int video_width;
static int video_height;


int frameskip;
int autoframeskip;
#define FRAMESKIP_LEVELS 12
#define FRAMESKIP_LIMIT (FRAMESKIP_LEVELS-1)

static const int skiptable[FRAMESKIP_LEVELS][FRAMESKIP_LEVELS] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 0,0,0,0,0,1,0,0,0,0,0,1 },
	{ 0,0,0,1,0,0,0,1,0,0,0,1 },
	{ 0,0,1,0,0,1,0,0,1,0,0,1 },
	{ 0,1,0,0,1,0,1,0,0,1,0,1 },
	{ 0,1,0,1,0,1,0,1,0,1,0,1 },
	{ 0,1,0,1,1,0,1,0,1,1,0,1 },
	{ 0,1,1,0,1,1,0,1,1,0,1,1 },
	{ 0,1,1,1,0,1,1,1,0,1,1,1 },
	{ 0,1,1,1,1,1,0,1,1,1,1,1 },
	{ 0,1,1,1,1,1,1,1,1,1,1,1 }
};

//static const int waittable[FRAMESKIP_LEVELS][FRAMESKIP_LEVELS] =
//{
//	{ 1,1,1,1,1,1,1,1,1,1,1,1 },
//	{ 2,1,1,1,1,1,1,1,1,1,1,0 },
//	{ 2,1,1,1,1,0,2,1,1,1,1,0 },
//	{ 2,1,1,0,2,1,1,0,2,1,1,0 },
//	{ 2,1,0,2,1,0,2,1,0,2,1,0 },
//	{ 2,0,2,1,0,2,0,2,1,0,2,0 },
//	{ 2,0,2,0,2,0,2,0,2,0,2,0 },
//	{ 2,0,2,0,0,3,0,2,0,0,3,0 },
//	{ 3,0,0,3,0,0,3,0,0,3,0,0 },
//	{ 4,0,0,0,4,0,0,0,4,0,0,0 },
//	{ 6,0,0,0,0,0,6,0,0,0,0,0 },
//	{12,0,0,0,0,0,0,0,0,0,0,0 }
//};

/* type of monitor output- */
/* Standard PC, NTSC, PAL or Arcade */
//int monitor_type;

//int use_keyboard_leds;
//int always_synced;

//#if USED_DUMMY
//int triple_buffer;
//#endif //USED_DUMMY

int skiplines;
int skipcolumns;
int gfx_skiplines;
int gfx_skipcolumns;
int vscanlines;
int hscanlines;
//int stretch;
//char *resolution;
//char *mode_desc;
//int gfx_mode;
/*static int*/#define gfx_width  512
/*static int*/#define gfx_height 272
static int vis_min_x,vis_max_x,vis_min_y,vis_max_y;

static int viswidth;
static int visheight;
static int skiplinesmax;
static int skipcolumnsmax;
static int skiplinesmin;
static int skipcolumnsmin;
//static int show_debugger,debugger_focus_changed;

int gfx_xoffset;
int gfx_yoffset;
int gfx_display_lines;
int gfx_display_columns;
static unsigned long cmultiply;
//static int 1/*xmultiply*/,1/*ymultiply*/;
static int xmultiply,ymultiply;

static int frameskip_counter;
static int frameskipadjust;
//static int frames_displayed;
//static cycles_t start_time,end_time;	/* to calculate fps average on exit */

static /*int*/u8 warming_up;
static /*int*/u8 gone_to_gfx_mode;
static /*int*/UINT8 bitmap_depth;
//static /*int*/UINT8 video_sync;
/*static*/ /*int*/UINT8 wait_vsync;
UINT8 limit_max_speed;
#define USE_throttle 0
#if (1==USE_throttle)
int throttle;       /* toggled by F10 */
#endif
float vsync_frame_rate;
static float video_fps;

//repuken2 re-added
static float bitmap_aspect_ratio;
#define MAX_X_MULTIPLY 4
#define MAX_Y_MULTIPLY 4


#define FRAMES_TO_SKIP 20				/* skip the first few frames from the FPS calculation */
										/* to avoid counting the copyright and info screens */


#define RM_TO_LINEAR(addr)    ((((addr) & 0xFFFF0000) >> 12) + ((addr) & 0xFFFF))

/*
 * This function tries to find the best display mode.
 */
static int select_display_mode(int width,int height,int depth,int colors,int attributes)
{

	//int display_pages;
	//int display_page_offset;
	//int found;

	//display_pages	= 1;
	//display_page_offset = 0;
	//found			= 0;

//	gfx_width		= 512;
//	gfx_height		= 272;
	//gfx_depth		= 16;
	//video_depth 	= 16;

	/* if triple buffering is enabled, turn off vsync */
	//if (triple_buffer)
	//{
	//	wait_vsync = 0;
	//	video_sync = 0;
	//}

	//blit_set_buffers( display_pages, display_page_offset );
	//logerror( "Display buffers: %d offset: 0x%x\n", display_pages, display_page_offset );

	gone_to_gfx_mode = 1;

	vsync_frame_rate = video_fps;

#if 0
	if (video_sync)
	{
		cycles_t a;
		cycles_t cps = osd_cycles_per_second();

		int i;
		/* wait some time to let everything stabilize */
		for (i = 0;i < 60;i++)
		{
//			vsync();
			sceDisplayWaitVblankStart();
			a = osd_cycles();
		}
#if 1
/* ???? */
		/* small delay for really really fast machines */
		for (i = 0;i < 100000;i++) ;
#endif
//		vsync();
		sceDisplayWaitVblankStart();
		cycles_t b;
		b = osd_cycles();

		float rate;
		rate = (float)cps/(b-a);

		//logerror("target frame rate = %3.2ffps, video frame rate = %3.2fHz\n",video_fps,rate);

		/* don't allow more than 8% difference between target and actual frame rate */
		while (rate > video_fps * 108 / 100)
			rate /= 2;

		if (rate < video_fps * 92 / 100)
		{
			osd_close_display();
			//logerror("\n");
		//	logerror("-vsync option cannot be used with this display mode:\n"
		//				"video refresh frequency = %3.2fHz, target frame rate = %3.2ffps\n",
		//				(float)cps/(b-a),video_fps);
			return 0;
		}

		//logerror("adjusted video frame rate = %3.2fHz\n",rate);
		vsync_frame_rate = rate;

		if (Machine->sample_rate)
		{
			Machine->sample_rate = Machine->sample_rate * video_fps / rate;
			//logerror("sample rate adjusted to match video freq: %d\n",Machine->sample_rate);
		}
	}
#endif
	return 1;
}





//============================================================
//	update_palette
//============================================================

static void update_palette(struct mame_display *display)
{
//TMK	if( video_depth == 8 )
//	{
//	}
//	else
	{
		// loop over dirty colors in batches of 32
		int i;
		for (i = 0; i < display->game_palette_entries; i += 32)
		{
			UINT32 dirtyflags = display->game_palette_dirty[i / 32];
			if (dirtyflags)
			{
				display->game_palette_dirty[i / 32] = 0;

				// loop over all 32 bits and update dirty entries
				int j;
				for (j = 0; j < 32; j++, dirtyflags >>= 1)
				{
					if (dirtyflags & 1)
					{
						// extract the RGB values
						rgb_t rgbvalue = display->game_palette[i + j];
						blit_lookup_low[ i + j ] =
							makecol( RGB_RED(rgbvalue), RGB_GREEN(rgbvalue), RGB_BLUE(rgbvalue) ) * cmultiply;
					}
				}
			}
		}
	}
}

//============================================================
//	init_palette
//============================================================

static void init_palette( int colors )
{

//#ifdef PSP_VRAM_BLIT
#if 0
/* この変更でさらに「重い...」だがメインメモリは(256kBytes)節約出来る筈。 */
/* 例えば rezon は たったの 128kBytes 足りないだけで動かない。(ADJUST_GU_MEMORY_KB:psp_video.h) */

	/* 変換テーブルをＶＲＡＭに確保 */
	blit_lookup_low = (UINT32 *)psp_frame_addr(tex_frame, 0, 0);
#else
/* 現状 どっち(VRAM_blit on/off)(mame_bitmap をＶＲＡＭに確保／mame_bitmap をメインメモリに確保)の方式でも、
	これ(変換テーブルをメインメモリに確保)の方が速い． */

	/* 変換テーブルをメインメモリに確保 */
	blit_lookup_low = auto_malloc( 65536*4 );
#endif //PSP_VRAM_BLIT


//TMK	if( video_depth == 8 )
//	{
//	}
//	else
	{
//#if (0!=LINK_ROT)
#if (360<LINK_ROT)
/*  mame_palettized 16ビット色 専用モード時は含まない(direct mode 専用) */
/*  0!=LINK_ROT *//*  360!=LINK_ROT *//* 9360==LINK_ROT */
		if( bitmap_depth == 32 )
		{
			//if( /*video_depth == 15 ||*/ video_depth == 16 )
			{
				/* initialise 8-8-8 to 5-5-5/5-6-5 reduction table */
				int i;
				for( i = 0; i < 65536; i++ )
				{
					blit_lookup_low[ i ] = makecol( 0, ( i >> 8 ) & 0xff, i & 0xff ) * cmultiply;
					blit_lookup_high[ i ] = makecol( i & 0xff, 0, 0 ) * cmultiply;
				}
			}
		}
		else
#endif //(0!=LINK_ROT)
		{
			/* initialize the palette to a fixed 5-5-5 mapping */
			int r;
			for (r = 0; r < 32; r++)
			{
				int g;
				for (g = 0; g < 32; g++)
				{
					int b;
					for (b = 0; b < 32; b++)
					{
						int idx = (r << 10) | (g << 5) | b;
						int rr = ( ( r * 255 ) + 15 ) / 31;
						int gg = ( ( g * 255 ) + 15 ) / 31;
						int bb = ( ( b * 255 ) + 15 ) / 31;
						blit_lookup_low[ idx ] = makecol( rr, gg, bb ) * cmultiply;
					}
				}
			}
		}
	}
}

#if 000
//============================================================
//	update_palette_debugger
//============================================================
static void update_palette_debugger(struct mame_display *display)
{
	int i;

	for (i = 0; i < display->debug_palette_entries; i ++)
	{
		// extract the RGB values
		rgb_t rgbvalue = display->debug_palette[i];
		int r = RGB_RED(rgbvalue);
		int g = RGB_GREEN(rgbvalue);
		int b = RGB_BLUE(rgbvalue);

//TMK		if( video_depth == 8 )
//		{
//			if( i < 256 )
//			{
//				RGB adjusted_palette;
//				adjusted_palette.r = r >> 2;
//				adjusted_palette.g = g >> 2;
//				adjusted_palette.b = b >> 2;
//
//				set_color( i, &adjusted_palette );
//				blit_lookup_low[ i ] = i * cmultiply;
//			}
//		}
//		else
		{
			blit_lookup_low[ i ] = makecol( r, g, b ) * cmultiply;
		}
	}
}
#endif



/* center image inside the display based on the visual area */
static void update_visible_area(struct mame_display *display)
{
//	2==int align;
//	int act_width;
//	act_width = gfx_width;

//	if (show_debugger)
//	{
//		vis_min_x = 0;
//		vis_max_x = display->debug_bitmap->width-1;
//		vis_min_y = 0;
//		vis_max_y = display->debug_bitmap->height-1;
//	}
//	else
	{
//#if (0!=LINK_ROT)
		if( video_swapxy )
		{
			vis_min_x = display->game_visible_area.min_y;
			vis_max_x = display->game_visible_area.max_y;
			vis_min_y = display->game_visible_area.min_x;
			vis_max_y = display->game_visible_area.max_x;
		}
		else
//#endif //(0!=LINK_ROT)
		{
			vis_min_x = display->game_visible_area.min_x;
			vis_max_x = display->game_visible_area.max_x;
			vis_min_y = display->game_visible_area.min_y;
			vis_max_y = display->game_visible_area.max_y;
		}
	}

	//logerror("set visible area %d-%d %d-%d\n",vis_min_x,vis_max_x,vis_min_y,vis_max_y);

	viswidth  = vis_max_x - vis_min_x + 1;
	visheight = vis_max_y - vis_min_y + 1;

	if( viswidth <= 1 || visheight <= 1 )
	{
		return;
	}

	gfx_display_lines   = visheight;
	gfx_display_columns = viswidth;

	{
		gfx_xoffset = (gfx_width - viswidth ) / 2;
		if (gfx_display_columns > gfx_width )
			gfx_display_columns = gfx_width ;

		gfx_yoffset = (gfx_height - visheight ) / 2;
			if (gfx_display_lines > gfx_height )
				gfx_display_lines = gfx_height ;

		skiplinesmin = vis_min_y;
		skiplinesmax = visheight - gfx_display_lines + vis_min_y;
		skipcolumnsmin = vis_min_x;
		skipcolumnsmax = viswidth - gfx_display_columns + vis_min_x;
//#if (0!=LINK_ROT)
		/* the skipcolumns from mame.cfg/cmdline is relative to the visible area */
		if( video_flipx )	{	skipcolumns = skipcolumnsmax;	if( gfx_skipcolumns >= 0 )	{	skipcolumns -= gfx_skipcolumns;	}	}
		else				{	skipcolumns = skipcolumnsmin;	if( gfx_skipcolumns >= 0 )	{	skipcolumns += gfx_skipcolumns;	}	}
		if( video_flipy )	{	skiplines   = skiplinesmax;		if( gfx_skiplines   >= 0 )	{	skiplines   -= gfx_skiplines;	}	}
		else				{	skiplines   = skiplinesmin;		if( gfx_skiplines   >= 0 )	{	skiplines   += gfx_skiplines;	}	}
//#endif //(0!=LINK_ROT)
		/* Just in case the visual area doesn't fit */
		if (gfx_xoffset < 0)
		{
			if( gfx_skipcolumns < 0 )
			{
				if( video_flipx )	{	skipcolumns += gfx_xoffset;	}
				else				{	skipcolumns -= gfx_xoffset;	}
			}
			gfx_xoffset=0;
		}
		if (gfx_yoffset < 0)
		{
			if( gfx_skiplines < 0 )
			{
				if( video_flipy )	{	skiplines   += gfx_yoffset;	}
				else				{	skiplines   -= gfx_yoffset;	}
			}
			gfx_yoffset=0;
		}

		/* align left hand side *///	{	align = 2;	}

		gfx_xoffset -= (gfx_xoffset &1); //% ( /*align*/2 /* * 1 xdivide */ );

		/* Failsafe against silly parameters */
		if (skiplines   < skiplinesmin)		skiplines   = skiplinesmin;
		if (skiplines   > skiplinesmax)		skiplines   = skiplinesmax;
		if (skipcolumns < skipcolumnsmin)	skipcolumns = skipcolumnsmin;
		if (skipcolumns > skipcolumnsmax)	skipcolumns = skipcolumnsmax;

	//	logerror("gfx_width = %d gfx_height = %d\n"
	//			"gfx_xoffset = %d gfx_yoffset = %d\n"
	//			"xmin %d ymin %d xmax %d ymax %d\n"
	//			"skiplines %d skipcolumns %d\n"
	//			"gfx_skiplines %d gfx_skipcolumns %d\n"
	//			"gfx_display_lines %d gfx_display_columns %d\n"
	//			"1/*xmultiply*/ %d 1/*ymultiply*/ %d\n"
	//			"skiplinesmin %d skiplinesmax %d\n"
	//			"skipcolumnsmin %d skipcolumnsmax %d\n"
	//			"video_flipx %d video_flipy %d video_swapxy %d\n",
	//			gfx_width, gfx_height,
	//			gfx_xoffset, gfx_yoffset,
	//			vis_min_x, vis_min_y, vis_max_x, vis_max_y,
	//			skiplines, skipcolumns,
	//			gfx_skiplines, gfx_skipcolumns,
	//			gfx_display_lines, gfx_display_columns,
	//			1/*xmultiply*/, 1/*ymultiply*/,
	//			skiplinesmin, skiplinesmax,
	//			skipcolumnsmin, skipcolumnsmax,
	//			video_flipx, video_flipy, video_swapxy );
//#if (0!=LINK_ROT)
		if( video_swapxy )
		{
			set_ui_visarea(skiplines, skipcolumns, skiplines+gfx_display_lines-1, skipcolumns+gfx_display_columns-1);
		}
		else
//#endif //(0!=LINK_ROT)
		{
			set_ui_visarea(skipcolumns, skiplines, skipcolumns+gfx_display_columns-1, skiplines+gfx_display_lines-1);
		}
	}

//	bitblit = bitblit_psp;
//	bitblit = bitblit_psp_mov;
//	bitblit = bitblit_psp_dma;

	/*switch(blitMode){

    case BLIT_MODE_STRETCH:           bitblit = bitblit_psp_direct;
    case BLIT_MODE_STRETCH_NO_ADJUST: bitblit = bitblit_psp_direct_noAdjust;
    case BLIT_MODE_NORMAL:            bitblit = bitblit_psp_direct_normal;

    default: bitblit = bitblit_psp_direct;
    }*/
//bitblit = bitblit_psp_direct_320;






//	if( show_debugger )
//	{
//		cmultiply = blit_setup(
//		gfx_width, gfx_height, display->debug_bitmap->depth,
//		///*video_depth*/16,
//			0, //1, 1,
//		//1/*xdivide*/, 1/*ydivide*/,
//			0, 0, 0, 0, 0 );
//	}
//	else
	if(video_swapxy){
      cmultiply = blit_setup_rot(gfx_width, gfx_height, bitmap_depth,video_attributes,video_flipx, video_flipy,
                            video_swapxy,((gfx_xoffset + (gfx_yoffset << 9)) << 1));
	} else {
     cmultiply = blit_setup(gfx_width, gfx_height, bitmap_depth,video_attributes,video_flipx, video_flipy,
                            video_swapxy,((gfx_xoffset + (gfx_yoffset << 9)) << 1));
	}
}

void setBlitMode(int bmode)
{
blitMode = bmode;

/*char algo[256];
sprintf(algo,"osd_video-setBlitMode (adentro ): me llego blitMode=='%d'",blitMode);
logWriteX(algo,"","",678);*/

}

int getBlitMode(void)
{
return blitMode;
}


void blitPreConfigure()
{
//char deathrash[512];

int gameW,gameH;

//vis_max_x - vis_min_x + 1;


if(video_swapxy){
   gameW = Machine->visible_area.max_y - Machine->visible_area.min_y + 1;
   gameH = Machine->visible_area.max_x - Machine->visible_area.min_x + 1;
  }else {
   gameW = Machine->visible_area.max_x - Machine->visible_area.min_x + 1;
   gameH = Machine->visible_area.max_y - Machine->visible_area.min_y + 1;
  }

/*sprintf(deathrash,"blitPreconfigure: gameW=='%d' , gameH=='%d'", gameW , gameH);
logWriteX(deathrash,"","",678);*/

 switch(blitMode){


   case 0://SCR_AUTO_STRETCH

        if(video_swapxy){ //C/ROTACION

          bitblit = bitblit_psp_direct_rotated_320_240;
          masterHorzStretch = (int)(480 / (480 - gameW) ) /*+ 1*/;
          masterVertStretch = (int)(272 / (272 - gameH) ) + 1;

          //masterScreenOffset = (512 * 8) + 30;

          #if 0
          switch(gameW){

          case 224: //240x224 -- xROT ---> 224x240

                bitblit = bitblit_psp_direct_rotated_320_240;
                masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
                masterHorzStretch = (int)(480 / (480 - gameW) ) /*+ 1*/;

                masterScreenOffset = (512 * 0) + 15;

               break;

          case 256:

               bitblit = bitblit_psp_direct_rotated_320_240;
               masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
               masterHorzStretch = (int)(480 / (480 - gameW) ) - 1;
               masterHorizStretchRemain = (int)( 480 / (480 % (480 - gameW) ) ) - 6;

               break;

          default:

               bitblit = bitblit_psp_direct_rotated_320_240;
               masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
               masterHorzStretch = (int)(480 / (480 - gameW) ) -1;

          }//switch
         #endif

        }
        else { //NORMAL, SIN ROTACION

         switch(gameW){

          case 240:

                bitblit = bitblit_psp_direct_320;
                masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
                masterHorzStretch = (int)(480 / (480 - gameW) ) -1;

               break;

          case 256:

               bitblit = bitblit_psp_direct_256;
               masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
               masterHorzStretch = (int)(480 / (480 - gameW) ) - 1;
               masterHorizStretchRemain = (int)( 480 / (480 % (480 - gameW) ) ) - 6;

               break;

          case 288://not working properly, fix it

               bitblit = bitblit_psp_direct_320_noAdjust;
               masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
               masterHorzStretch = (int)(480 / (480 - gameW) );

               masterScreenOffset = (512 * 0) + 24;

               break;

          case 304:

               bitblit = bitblit_psp_direct_320_noAdjust;
               masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
               masterHorzStretch = (int)(480 / (480 - gameW) );

               masterScreenOffset = (512 * 0) + 12;

               break;

          case 320:

                 bitblit = bitblit_psp_direct_320;
                 masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
                 masterHorzStretch = (int)(480 / (480 - gameW) ) -1;

               break;

          default:

                bitblit = bitblit_psp_direct_320;
                masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
                masterHorzStretch = (int)(480 / (480 - gameW) ) -1;
         }

      }//if !ROT

         break;//case 0

   case 1://SCR_STRETCH_240_320
         bitblit = bitblit_psp_direct_320;
         masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
         masterHorzStretch = (int)(480 / (480 - gameW) ) -1;
         break;

   case 2://SCR_STRETCH_240_320_NO_ADJUST
         bitblit = bitblit_psp_direct_320_noAdjust;
         masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
         masterHorzStretch = (int)(480 / (480 - gameW) );

         masterScreenOffset = (512 * 0) + 20;

         break;

   case 3://SCR_STRETCH_256
         bitblit = bitblit_psp_direct_256;
         masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
         masterHorzStretch = (int)(480 / (480 - gameW) ) - 1;
         masterHorizStretchRemain = (int)( 480 / (480 % (480 - gameW) ) ) - 6;
         break;


   case 4://SCR_STRETCH_288
         bitblit = bitblit_psp_direct_288;
         masterVertStretch = (int)(272 / (272 - gameH) ) + 1;
         masterHorzStretch = (int)(480 / (480 - gameW) );
         masterHorizStretchRemain = (int)( 480 / (480 % (480 - gameW) ) );
         break;


   case 5://SRC_X1

         if(video_swapxy)
          bitblit = bitblit_psp_direct_rotated;
         else
          bitblit = bitblit_psp_direct_normal;

         break;

   default: bitblit = bitblit_psp_direct_320;
  }

}


//============================================================
//	osd_create_display
//============================================================

int osd_create_display(const struct osd_create_params *params, UINT32 *rgb_components)
{
	struct mame_display dummy_display;
//#if (0!=LINK_ROT)
	if( video_swapxy )
	{
		video_width  = params->height;
		video_height = params->width;
		bitmap_aspect_ratio = (float)params->aspect_y / (float)params->aspect_x;
	}
	else
//#endif //(0!=LINK_ROT)
	{
		video_width  = params->width;
		video_height = params->height;
		//bitmap_aspect_ratio = (float)params->aspect_x / (float)params->aspect_y;
	}

	bitmap_depth = params->depth;
	video_fps    = params->fps;
	video_attributes = params->video_attributes;

	// clamp the frameskip value to within range
	if (frameskip < 0)					frameskip = 0;
	if (frameskip >= FRAMESKIP_LEVELS)	frameskip = FRAMESKIP_LEVELS - 1;

//	show_debugger = 0;
	gone_to_gfx_mode = 0;

	//logerror("width %d, height %d depth %d colors %d\n",video_width,video_height,bitmap_depth,params->colors);

	if (!select_display_mode(video_width,video_height,bitmap_depth,params->colors,video_attributes) )
	{
		return 1;
	}

	cmultiply = 0x00000001;
	init_palette( params->colors );

	// fill in the resulting RGB components
	if (rgb_components)
	{
#if (0!=LINK_ROT)
/* 強制[H]16ビットモード時は含まない */
		if ( bitmap_depth == 32 )
		{
		//	if( video_depth == 24 || video_depth == 32 )
		//	{
		//		rgb_components[0] = makecol(0xff, 0x00, 0x00);
		//		rgb_components[1] = makecol(0x00, 0xff, 0x00);
		//		rgb_components[2] = makecol(0x00, 0x00, 0xff);
		//	}
		//	else
			{
				rgb_components[0] = 0xff0000;
				rgb_components[1] = 0x00ff00;
				rgb_components[2] = 0x0000ff;
			}
		}
		else
#endif //(0!=LINK_ROT)
		{
			//if( video_depth == 15 || video_depth == 16 )
			{
#if defined( DIRECT15_USE_LOOKUP )
				video_attributes &= ~VIDEO_RGB_DIRECT;
			}
#else
				rgb_components[0] = makecol(0xf8, 0x00, 0x00);
				rgb_components[1] = makecol(0x00, 0xf8, 0x00);
				rgb_components[2] = makecol(0x00, 0x00, 0xf8);
			}
		//	else
#endif
		//	{
		//		rgb_components[0] = 0x7c00;
		//		rgb_components[1] = 0x03e0;
		//		rgb_components[2] = 0x001f;
		//	}
		}
	}

	// set visible area to nothing just to initialize it - it will be set by the core
	memset(&dummy_display, 0, sizeof(dummy_display));
	update_visible_area(&dummy_display);

	// indicate for later that we're just beginning
	warming_up = 1;
	return 0;
}


/* shut up the display */
void osd_close_display(void)
{
	psp_clear_screen();
	if (gone_to_gfx_mode != 0)
	{
//		if (frames_displayed > FRAMES_TO_SKIP)
//		{
//			cycles_t cps = osd_cycles_per_second();
//			printf("Average FPS: %f\nPress X button",(float)cps/(end_time-start_time)*(frames_displayed-FRAMES_TO_SKIP));
//		}
		gone_to_gfx_mode = 0;
	}
}

#if 000
static void set_debugger_focus(struct mame_display *display, int debugger_has_focus)
{
	static int temp_afs, temp_fs, temp_throttle;

	if (/*show_debugger*/0 != debugger_has_focus)
	{
	//	show_debugger = debugger_has_focus;
	//	debugger_focus_changed = 1;

	//	if (show_debugger)
	//	{
	//		// store frameskip/throttle settings
	//		temp_fs       = frameskip;
	//		temp_afs      = autoframeskip;
	//		temp_throttle = throttle;
	//		// temporarily set them to usable values for the debugger
	//		frameskip     = 0;
	//		autoframeskip = 0;
	//		throttle      = 1;
	//	}
	//	else
		{
			/* silly way to clear the screen */
			mame_bitmap *clrbitmap;

			clrbitmap = bitmap_alloc_depth(gfx_display_columns,gfx_display_lines,display->debug_bitmap->depth);
			if (clrbitmap)
			{
				fillbitmap(clrbitmap,0,NULL);
				/* three times to handle triple buffering */
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines/*, 0, 0*/ );
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines/*, 0, 0*/ );
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines/*, 0, 0*/ );
				bitmap_free(clrbitmap);
			}

			init_palette( display->game_palette_entries );

			/* mark palette dirty */
			if( display->game_palette_dirty != NULL )
			{
				int i;

				for (i = 0; i < display->game_palette_entries; i++ )
				{
					display->game_palette_dirty[ i / 32 ] |= 1 << ( i % 32 );
				}
			}
			// restore frameskip/throttle settings
			frameskip     = temp_fs;
			autoframeskip = temp_afs;
			throttle      = temp_throttle;
		}
		display->changed_flags |= ( GAME_BITMAP_CHANGED | DEBUG_BITMAP_CHANGED | GAME_PALETTE_CHANGED | DEBUG_PALETTE_CHANGED | GAME_VISIBLE_AREA_CHANGED );
	}
}
#endif

static void bitblit_dummy( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh/*, int dx, int dy*/ )
{
	//logerror("msdos/video.c: undefined bitblit() function for %d x %d!\n",xmultiply,ymultiply);
}
#if 0
INLINE void pan_display( struct mame_display *display )
{
	int pan_changed = 0;

	/* horizontal panning */
	if( ( !video_flipx && input_ui_pressed_repeat( IPT_UI_PAN_RIGHT, 1 ) ) ||
		( video_flipx && input_ui_pressed_repeat( IPT_UI_PAN_LEFT, 1 ) ) )
	{
		if (skipcolumns < skipcolumnsmax)
		{
			skipcolumns++;
			pan_changed = 1;
		}
	}
	if( ( !video_flipx && input_ui_pressed_repeat( IPT_UI_PAN_LEFT, 1 ) ) ||
		( video_flipx && input_ui_pressed_repeat( IPT_UI_PAN_RIGHT, 1 ) ) )
	{
		if (skipcolumns > skipcolumnsmin)
		{
			skipcolumns--;
			pan_changed = 1;
		}
	}
	/* vertical panning */
	if( ( !video_flipy && input_ui_pressed_repeat( IPT_UI_PAN_DOWN, 1 ) ) ||
		( video_flipy && input_ui_pressed_repeat( IPT_UI_PAN_UP, 1 ) ) )
	{
		if (skiplines < skiplinesmax)
		{
			skiplines++;
			pan_changed = 1;
		}
	}
	if( ( !video_flipy && input_ui_pressed_repeat( IPT_UI_PAN_UP, 1 ) ) ||
		( video_flipy && input_ui_pressed_repeat( IPT_UI_PAN_DOWN, 1 ) ) )
	{
		if (skiplines > skiplinesmin)
		{
			skiplines--;
			pan_changed = 1;
		}
	}

	if (pan_changed)
	{
		if( video_swapxy )
		{
			set_ui_visarea(skiplines, skipcolumns, skiplines+gfx_display_lines-1, skipcolumns+gfx_display_columns-1);
		}
		else
		{
			set_ui_visarea(skipcolumns, skiplines, skipcolumns+gfx_display_columns-1, skiplines+gfx_display_lines-1);
		}
	}
}
#endif
//============================================================
//  video_skip_this_frame ( osd_skip_this_frame mame097)
//============================================================

int /*video_skip_this_frame*/osd_skip_this_frame(void)
{
	return skiptable[frameskip][frameskip_counter];
}

char *osd_FPS_text="noFPS";
const char *osd_get_fps_text(const struct performance_info *performance)
{
	static char buffer[64/*1024*/];
	char *dest = buffer;

	// display the FPS, frameskip, percent, fps and target fps
	dest += sprintf(dest,
	//	"%s%2d%4d%%%4d/%d fps",
		"%s%2d%3d%%%3dFPS",
		autoframeskip ? "auto" : "fskp", frameskip,
		(int)(performance->game_speed_percent /*+ 0.5*/),
		(int)(performance->frames_per_second /*+ 0.5*/)/*,*/
	/*	(int)(Machine->refresh_rate + 0.5 )*/ );

#define PSP_WARNING_PERTICAL 0
#if (1==PSP_WARNING_PERTICAL)
	/* for vector games, add the number of vector updates */
//	if (Machine->drv->video_attributes & VIDEO_TYPE_VECTOR)
//	{
//		dest += sprintf(dest, "\n %d vector updates", performance->vector_updates_last_second);
//	}
//	else
		if (performance->partial_updates_this_frame > 1)
	{
		dest += sprintf(dest, "\n %d partial updates", performance->partial_updates_this_frame);
	}
#endif // (1==PSP_WARNING_PERTICAL)
#undef PSP_WARNING_PERTICAL

	osd_FPS_text=buffer;
	/* return a pointer to the static buffer */
	return buffer;
}

//void osd_set_leds(int state){}

//============================================================
//	win_orient_rect
//============================================================

void win_orient_rect(rectangle *_rect)
{
//#if (0!=LINK_ROT)
	int temp;
	// apply X/Y swap first
	if (video_swapxy)
	{
		temp = _rect->min_x; _rect->min_x = _rect->min_y; _rect->min_y = temp;
		temp = _rect->max_x; _rect->max_x = _rect->max_y; _rect->max_y = temp;
	}

	// apply X flip
	if (video_flipx)
	{
		temp = video_width - _rect->min_x - 1;
		_rect->min_x = video_width - _rect->max_x - 1;
		_rect->max_x = temp;
	}

	// apply Y flip
	if (video_flipy)
	{
		temp = video_height - _rect->min_y - 1;
		_rect->min_y = video_height - _rect->max_y - 1;
		_rect->max_y = temp;
	}
//#endif //(0!=LINK_ROT)
}

#define NOT_OVER_100 1

/* Update the display. */
//extern void scan_keyboard(void); <<--- ver
extern void readkeys(void);


void osd_update_video_and_audio(struct mame_display *display)
{
	cycles_t curr;
	#if (1==NOT_OVER_100)
	/*???*/
	cycles_t target;
	#endif //(1==NOT_OVER_100)
	static cycles_t prev_measure;
	static cycles_t this_frame_base;
	static cycles_t last_sound_update;

//	int already_synced;

	cycles_t cps = osd_cycles_per_second();

//	if (display->changed_flags & DEBUG_FOCUS_CHANGED)
//	{
//		set_debugger_focus(display, display->debug_focus);
//	}

//	if (display->debug_bitmap && input_ui_pressed(IPT_UI_TOGGLE_DEBUG))
//	{
//		set_debugger_focus(display, show_debugger ^ 1);
//	}

	// if the visible area has changed, update it
	if (display->changed_flags & GAME_VISIBLE_AREA_CHANGED)
		update_visible_area(display);

//	// if the refresh rate has changed, update it
//	if (display->changed_flags & GAME_REFRESH_RATE_CHANGED)
//	{
//		video_fps = display->game_refresh_rate;
//		sound_update_refresh_rate(display->game_refresh_rate);
//		/* todo: vsync */
//	}

	if (warming_up)
	{
		/* first time through, initialize timer */
		prev_measure = osd_cycles() - (int)((float)FRAMESKIP_LEVELS * (float)cps / video_fps);
		last_sound_update = osd_cycles() - ( (cps+cps) / video_fps ) - 1;
		warming_up=0;
	}

	if (frameskip_counter == 0)
		this_frame_base = prev_measure + (int)((float)FRAMESKIP_LEVELS * (float)cps / video_fps);

	#if (1==USE_throttle)
	int throttle_audio;
	throttle_audio = throttle;
	if( throttle_audio )
	{
		/* if too much time has passed since last sound update, disable throttling */
		/* temporarily - we wouldn't be able to keep synch anyway. */
		curr = osd_cycles();
		if ((curr - last_sound_update) > ((cps+cps) / video_fps))
			throttle_audio=0;
		last_sound_update = curr;
	}
//	already_synced = msdos_update_audio( throttle_audio );
	#endif //(1==USE_throttle)
	if (display->changed_flags & GAME_PALETTE_CHANGED)
	{
		// if the game palette has changed, update it
		//if( !0/*show_debugger*/ )
		{
			update_palette(display);
		}
	}

	if ( display->changed_flags & GAME_BITMAP_CHANGED )
	{
		#if (1==USE_throttle)
		/* now wait until it's time to update the screen */
		if (throttle)
		{
			profiler_mark(PROFILER_IDLE);
			#if 0
			if (video_sync)
			{
				static cycles_t last;
				{
				const cycles_t vvvv=(video_fps * 11 /10);
				do
				{
					v_sync();
					curr = osd_cycles();
				}
				while ((cps / (curr - last)) > (vvvv) );
				}
				last = curr;
			}
			else
			{
			#else
			{
				/* wait for video sync but use normal throttling */
				if (wait_vsync){	v_sync();}
			#endif

				curr = osd_cycles();

//TMK				if (already_synced == 0)
				{
				/* wait only if the audio update hasn't synced us already */
					target = this_frame_base + (int)((float)frameskip_counter * (float)cps / video_fps);

//					if (curr - target < 0)
					if (curr < target)
					{
						do
						{
							curr = osd_cycles();
//						} while (curr - target < 0);
						} while (curr < target);
					}
				}
			}
			profiler_mark(PROFILER_END);
		}
		else
		#else //USE_throttle
		//if (wait_vsync){	v_sync();} // 現在 blit.c(psp_font.c) でv_sync(); 取ってるのでここは無し。
		#endif //USE_throttle
			curr = osd_cycles();

		#if (1==NOT_OVER_100)
		/*???*/
		if (limit_max_speed)
		{
			/* wait only if the audio update hasn't synced us already */
			target = this_frame_base + (int)((float)frameskip_counter * (float)cps / video_fps);
			while (curr - target < 0)
			{
				curr = osd_cycles();
			}
		}
		#endif //(1==NOT_OVER_100)

		if (frameskip_counter == 0)
		{
			prev_measure = curr;
		}

//		/* for the FPS average calculation */
//		if (++frames_displayed == FRAMES_TO_SKIP)
//			start_time = curr;
//		else
//			end_time = curr;

		//if( !0/*show_debugger*/ )
		{
			int srcxoffs;	srcxoffs = skipcolumns;
			int srcyoffs;	srcyoffs = skiplines;
			int srcwidth;	srcwidth = gfx_display_columns;
			int srcheight;	srcheight = gfx_display_lines;

//#if (0!=LINK_ROT)
			if( video_flipx ){	srcxoffs += ( srcwidth  - 1 );	}
			if( video_flipy ){	srcyoffs += ( srcheight - 1 );	}
			if( video_swapxy )
			{
				int t;
				t = srcxoffs;
				srcxoffs = srcyoffs;
				srcyoffs = t;
			}
//#endif //(0!=LINK_ROT)
			/* copy the bitmap to screen memory */
			profiler_mark( PROFILER_BLIT );
			bitblit( display->game_bitmap, srcxoffs, srcyoffs, srcwidth, srcheight/*, gfx_xoffset, gfx_yoffset*/ );
			profiler_mark( PROFILER_END );
		}

		/* see if we need to give the card enough time to draw both odd/even fields of the interlaced display
			(req. for 15.75KHz Arcade Monitor Modes */
//TMK		interlace_sync();

		if(
#if (1==USE_throttle)
			throttle &&
#endif //(1==USE_throttle)
			autoframeskip)
		{if( frameskip_counter == 0 )
		{
			const struct performance_info *performance = mame_get_performance_info();

			// if we're too fast, attempt to increase the frameskip
			if (performance->game_speed_percent > 100)
			{
				frameskipadjust++;

				// but only after 3 consecutive frames where we are too fast
				if (frameskipadjust >= 3)
				{
					frameskipadjust = 0;
					if (frameskip > 0) frameskip--;
				}
			}

			// if we're too slow, attempt to decrease the frameskip
			else
			{
				// if below 80% speed, be more aggressive
				if (performance->game_speed_percent < 80)
					frameskipadjust -= (90 - performance->game_speed_percent) / 5;

				// if we're close, only force it up to frameskip 8
				else if (frameskip < 8)
					frameskipadjust--;

				// perform the adjustment
				while (frameskipadjust <= -2)
				{
					frameskipadjust += 2;
					if (frameskip < FRAMESKIP_LEVELS - 1)
						frameskip++;
				}
			}
		}
		}
	}

//	if( show_debugger )
//	{
//		if( display->changed_flags & DEBUG_PALETTE_CHANGED )
//		{
//			update_palette_debugger( display );
//		}
//		if( display->changed_flags & DEBUG_BITMAP_CHANGED || /*xdivide*/1 != 1 || /*ydivide*/1 != 1 )
//		{
//			bitblit( display->debug_bitmap, 0, 0, gfx_display_columns, gfx_display_lines/*, 0, 0*/ );
//		}
//	}
//	else
	{
#if 00
		/* Check for PGUP, PGDN and pan screen */
		pan_display( display );
#endif

#if 00
		if (input_ui_pressed(IPT_UI_FRAMESKIP_INC))
		{
			if (autoframeskip)
			{
				autoframeskip = 0;
				frameskip = 0;
			}
			else
			{
				if (frameskip == FRAMESKIP_LIMIT)
				{
					frameskip = 0;
					autoframeskip = 1;
				}
				else
					frameskip++;
			}

			// display the FPS counter for 2 seconds
			ui_show_fps_temp(2.0);

			/* reset the frame counter every time the frameskip key is pressed, so */
			/* we'll measure the average FPS on a consistent status. */
		//	frames_displayed = 0;
		}

		if (input_ui_pressed(IPT_UI_FRAMESKIP_DEC))
		{
			if (autoframeskip)
			{
				autoframeskip = 0;
				frameskip = FRAMESKIP_LIMIT;
			}
			else
			{
				if (frameskip == 0)
					autoframeskip = 1;
				else
					frameskip--;
			}

			// display the FPS counter for 2 seconds
			ui_show_fps_temp(2.0);

			/* reset the frame counter every time the frameskip key is pressed, so */
			/* we'll measure the average FPS on a consistent status. */
		//	frames_displayed = 0;
		}

		if (input_ui_pressed(IPT_UI_THROTTLE))
		{
			throttle ^= 1;

			/* reset the frame counter every time the throttle key is pressed, so */
			/* we'll measure the average FPS on a consistent status. */
		//	frames_displayed = 0;
		}
#endif
	}

	// if the LEDs have changed, update them
//	if (display->changed_flags & LED_STATE_CHANGED)
//		osd_set_leds(display->led_state);

//	frameskip_counter = (frameskip_counter + 1) % FRAMESKIP_LEVELS;
	frameskip_counter++;
	if (FRAMESKIP_LIMIT<frameskip_counter) frameskip_counter = 0;

	//poll_joysticks();
	readkeys();
	//scan_keyboard(); <--- VER
}


//#define blit_swapxy video_swapxy
//#define blit_flipx video_flipx
//#define blit_flipy video_flipy


#if 00
//============================================================
//	osd_override_snapshot
//============================================================

mame_bitmap *osd_override_snapshot(struct mame_bitmap *bitmap, rectangle *bounds)
{
	rectangle newbounds;
	mame_bitmap *copy;
	int x, y, w, h, t;

	// if we can send it in raw, no need to override anything
	if (!blit_swapxy && !blit_flipx && !blit_flipy)
		return NULL;

	// allocate a copy
	w = blit_swapxy ? bitmap->height : bitmap->width;
	h = blit_swapxy ? bitmap->width : bitmap->height;
	copy = bitmap_alloc_depth(w, h, bitmap->depth);
	if (!copy)
		return NULL;

	// populate the copy
	for (y = bounds->min_y; y <= bounds->max_y; y++)
		for (x = bounds->min_x; x <= bounds->max_x; x++)
		{
			int tx = x, ty = y;

			// apply the rotation/flipping
			if (blit_swapxy)
			{
				t = tx; tx = ty; ty = t;
			}
			if (blit_flipx)
				tx = copy->width - tx - 1;
			if (blit_flipy)
				ty = copy->height - ty - 1;

			// read the old pixel and copy to the new location
			switch (copy->depth)
			{
				case 15:
				case 16:
					*((UINT16 *)copy->base + ty * copy->rowpixels + tx) =
							*((UINT16 *)bitmap->base + y * bitmap->rowpixels + x);
					break;

				case 32:
					*((UINT32 *)copy->base + ty * copy->rowpixels + tx) =
							*((UINT32 *)bitmap->base + y * bitmap->rowpixels + x);
					break;
			}
		}

	// compute the oriented bounds
	newbounds = *bounds;

	// apply X/Y swap first
	if (blit_swapxy)
	{
		t = newbounds.min_x; newbounds.min_x = newbounds.min_y; newbounds.min_y = t;
		t = newbounds.max_x; newbounds.max_x = newbounds.max_y; newbounds.max_y = t;
	}

	// apply X flip
	if (blit_flipx)
	{
		t = copy->width - newbounds.min_x - 1;
		newbounds.min_x = copy->width - newbounds.max_x - 1;
		newbounds.max_x = t;
	}

	// apply Y flip
	if (blit_flipy)
	{
		t = copy->height - newbounds.min_y - 1;
		newbounds.min_y = copy->height - newbounds.max_y - 1;
		newbounds.max_y = t;
	}

	*bounds = newbounds;
	return copy;
}
#endif

void osd_pause(int paused)
{
}


/* ここにblitを追加すると、何故か遅くなる。 */
