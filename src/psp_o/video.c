#define DIRECT15_USE_LOOKUP ( 1 )

#include <stdio.h>
#include <string.h>
#include <memory.h>

#include "driver.h"
#include "mamedbg.h"
#include "vidhrdw/vector.h"

#include <math.h>
#include "blit.h"
#include "syscall.h"

#define makecol(r,g,b) ((((b>>3) & 0x1F)<<10)|(((g>>3) & 0x1F)<<5)|(((r>>3) & 0x1F)<<0)|0x8000)

typedef struct RGB
{
   unsigned char r, g, b;
   unsigned char filler;
} RGB;

// from sound.c
extern void sound_update_refresh_rate(float newrate);

static int warming_up;

/* tweak values for centering tweaked modes */
int center_x;
int center_y;

double screen_aspect = (480.0 / 272.0);
int keep_aspect = 1;

/* in msdos/sound.c */
int msdos_update_audio( int throttle );

/* in msdos/input.c */
void poll_joysticks(void);

int video_flipx;
int video_flipy;
int video_swapxy;

static void bitblit_dummy( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy );
void ( *bitblit )( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy ) = bitblit_dummy;

extern const char *g_s_resolution;
int gfx_depth;
static int bitmap_depth;
static int video_depth,video_attributes;
static int video_width;
static int video_height;
static float video_fps;
static double bitmap_aspect_ratio;

// 256 color palette reduction
#define BLACK_PEN ( 0 )
#define WHITE_PEN ( 1 )
#define RESERVED_PENS ( 2 )
#define TOTAL_PENS ( 256 )
#define VERBOSE ( 0 )
#define PEDANTIC ( 0 )
static int pen_usage_count[ TOTAL_PENS ];
static RGB shrinked_palette[ TOTAL_PENS ];
static unsigned int rgb6_to_pen[ 64 ][ 64 ][ 64 ];
static unsigned char remap_color[ 65536 ];

static int palette_bit;
#define MAX_PALETTE_BITS ( 5 )

static int *scale_palette_r;
static int *scale_palette_g;
static int *scale_palette_b;

static int scale_palette[ 8 ][ 256 ];

static struct
{
	int r;
	int g;
	int b;
} palette_bits[] =
{
	/*	r	g	b */
	{	6,	6,	6	},
	{	5,	5,	5	},
	{	4,	4,	4	},
	{	3,	3,	3	},
	{	3,	3,	2	},
};

int frameskip,autoframeskip;
#define FRAMESKIP_LEVELS 12

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

static const int waittable[FRAMESKIP_LEVELS][FRAMESKIP_LEVELS] =
{
	{ 1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 2,1,1,1,1,1,1,1,1,1,1,0 },
	{ 2,1,1,1,1,0,2,1,1,1,1,0 },
	{ 2,1,1,0,2,1,1,0,2,1,1,0 },
	{ 2,1,0,2,1,0,2,1,0,2,1,0 },
	{ 2,0,2,1,0,2,0,2,1,0,2,0 },
	{ 2,0,2,0,2,0,2,0,2,0,2,0 },
	{ 2,0,2,0,0,3,0,2,0,0,3,0 },
	{ 3,0,0,3,0,0,3,0,0,3,0,0 },
	{ 4,0,0,0,4,0,0,0,4,0,0,0 },
	{ 6,0,0,0,0,0,6,0,0,0,0,0 },
	{12,0,0,0,0,0,0,0,0,0,0,0 }
};

/* type of monitor output- */
/* Standard PC, NTSC, PAL or Arcade */
int monitor_type;

int use_keyboard_leds;
int vgafreq;
int always_synced;
int video_sync;
int wait_vsync;
int triple_buffer;
float vsync_frame_rate;
int skiplines;
int skipcolumns;
int gfx_skiplines;
int gfx_skipcolumns;
int vscanlines;
int hscanlines;
int stretch;
int use_mmx;
int mmxlfb;
int use_tweaked;
int use_vesa;
char *resolution;
char *mode_desc;
int gfx_mode;
int gfx_width;
int gfx_height;
static int vis_min_x,vis_max_x,vis_min_y,vis_max_y;
static int xdivide;
static int ydivide;
/* indicates unchained video mode (req. for 15.75KHz Arcade Monitor Modes)*/
int unchained;
/* flags for lowscanrate modes */
int scanrate15KHz;

static int viswidth;
static int visheight;
static int skiplinesmax;
static int skipcolumnsmax;
static int skiplinesmin;
static int skipcolumnsmin;
static int show_debugger,debugger_focus_changed;

static int reglen = 0;  /* for VGA modes */
static int videofreq;   /* for VGA modes */

int gfx_xoffset;
int gfx_yoffset;
int gfx_display_lines;
int gfx_display_columns;
static unsigned long cmultiply;
static int xmultiply,ymultiply;
int throttle;       /* toggled by F10 */

static int gone_to_gfx_mode;
static int frameskip_counter;
static int frameskipadjust;
static int frames_displayed;
static cycles_t start_time,end_time;    /* to calculate fps average on exit */
#define FRAMES_TO_SKIP 20       /* skip the first few frames from the FPS calculation */
							/* to avoid counting the copyright and info screens */


#define RM_TO_LINEAR(addr)    ((((addr) & 0xFFFF0000) >> 12) + ((addr) & 0xFFFF))

/*
 * This function tries to find the best display mode.
 */
static int select_display_mode(int width,int height,int depth,int colors,int attributes)
{
	int i;
	struct mode_adjust *adjust_array;
	int display_pages;
	int display_page_offset;
	int found;

	display_pages = 1;
	display_page_offset = 0;
	unchained = 0;
	xdivide = 1;
	ydivide = 1;
	found = 0;
	videofreq = vgafreq;

	gfx_width = 512;
	gfx_height = 272;
	gfx_depth =16;
	video_depth = 16;

	/* if triple buffering is enabled, turn off vsync */
	if (triple_buffer)
	{
		wait_vsync = 0;
		video_sync = 0;
	}

	blit_set_buffers( display_pages, display_page_offset );
	logerror( "Display buffers: %d offset: 0x%x\n", display_pages, display_page_offset );

	gone_to_gfx_mode = 1;

	vsync_frame_rate = video_fps;

	if (video_sync)
	{
		cycles_t a,b;
		float rate;
		cycles_t cps = osd_cycles_per_second();

		/* wait some time to let everything stabilize */
		for (i = 0;i < 60;i++)
		{
//			vsync();
			sceDisplayWaitVblankStart();
			a = osd_cycles();
		}

		/* small delay for really really fast machines */
		for (i = 0;i < 100000;i++) ;

//		vsync();
		sceDisplayWaitVblankStart();
		b = osd_cycles();

		rate = (float)cps/(b-a);

		logerror("target frame rate = %3.2ffps, video frame rate = %3.2fHz\n",video_fps,rate);

		/* don't allow more than 8% difference between target and actual frame rate */
		while (rate > video_fps * 108 / 100)
			rate /= 2;

		if (rate < video_fps * 92 / 100)
		{
			osd_close_display();
			logerror("-vsync option cannot be used with this display mode:\n"
						"video refresh frequency = %3.2fHz, target frame rate = %3.2ffps\n",
						(float)cps/(b-a),video_fps);
			return 0;
		}

		logerror("adjusted video frame rate = %3.2fHz\n",rate);
			vsync_frame_rate = rate;

		if (Machine->sample_rate)
		{
			Machine->sample_rate = Machine->sample_rate * video_fps / rate;
			logerror("sample rate adjusted to match video freq: %d\n",Machine->sample_rate);
		}
	}

	return 1;
}


//============================================================
//	build_rgb_to_pen
//============================================================

static void build_rgb_to_pen( void )
{
	int i;
	int r;
	int g;
	int b;

	for( r = 0; r < 64; r++ )
	{
		for( g = 0; g < 64; g++ )
		{
			for( b = 0; b < 64; b++ )
			{
				rgb6_to_pen[ r ][ g ][ b ] = TOTAL_PENS;
			}
		}
	}
	rgb6_to_pen[ 0 ][ 0 ][ 0 ] = BLACK_PEN;
	rgb6_to_pen[ scale_palette_r[ 255 ] ][ scale_palette_g[ 255 ] ][ scale_palette_b[ 255 ] ] = WHITE_PEN;

	for( i = RESERVED_PENS; i < TOTAL_PENS; i++ )
	{
		if( pen_usage_count[ i ] > 0 )
		{
			r = shrinked_palette[ i ].r;
			g = shrinked_palette[ i ].g;
			b = shrinked_palette[ i ].b;

			if( rgb6_to_pen[ r ][ g ][ b ] == TOTAL_PENS )
			{
				int j;
				int max;

				rgb6_to_pen[ r ][ g ][ b ] = i;
				max = pen_usage_count[ i ];

				/* to reduce flickering during remaps, find the pen used by most colors */
				for( j = i + 1; j < TOTAL_PENS; j++ )
				{
					if( pen_usage_count[ j ] > max &&
							r == shrinked_palette[ j ].r &&
							g == shrinked_palette[ j ].g &&
							b == shrinked_palette[ j ].b )
					{
						rgb6_to_pen[ r ][ g ][ b ] = j;
						max = pen_usage_count[ j ];
					}
				}
			}
		}
	}
}


//============================================================
//	compress_palette
//============================================================

static int compress_palette( struct mame_display *display )
{
	int i;
	int j;
	int r;
	int saved;
	int g;
	int b;
	int updated;

	build_rgb_to_pen();

	saved = 0;
	updated = 0;

	for( i = 0; i < display->game_palette_entries; i++ )
	{
		/* merge pens of the same color */
		rgb_t rgbvalue = display->game_palette[ i ];
		r = scale_palette_r[ RGB_RED( rgbvalue ) ];
		g = scale_palette_g[ RGB_GREEN( rgbvalue ) ];
		b = scale_palette_b[ RGB_BLUE( rgbvalue ) ];

		j = rgb6_to_pen[ r ][ g ][ b ];

		if( j != TOTAL_PENS && ( blit_lookup_low[ i ] & 0xff ) != j )
		{
			pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ]--;
			if( pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ] == 0 )
			{
				saved++;
				updated = 1;
			}
			blit_lookup_low[ i ] = j * cmultiply;
			pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ]++;
			if( remap_color[ i ] )
			{
				remap_color[ i ] = 0;
				updated = 1;
			}
		}
	}

#if VERBOSE
	logerror( "Compressed the palette, saving %d pens\n", saved );
#endif

	return updated;
}


//============================================================
//	update_palette
//============================================================

#define PALETTE_DIRTY( i ) ( display->game_palette_dirty[ ( i ) / 32 ] & ( 1 << ( ( i ) % 32 ) ) )
#define PALETTE_DIRTY_CLEAR( i ) display->game_palette_dirty[ ( i ) / 32 ] &= ~( 1 << ( ( i ) % 32 ) )

static void update_palette(struct mame_display *display)
{
	int i;
	int j;

//TMK	if( video_depth == 8 )
//	{
//		int need;
//
//		need = 0;
//
//		for( i = 0; i < display->game_palette_entries; i++ )
//		{
//			if( PALETTE_DIRTY ( i ) )
//			{
//				int pen = ( blit_lookup_low[ i ] & 0xff );
//				if( pen_usage_count[ pen ] == 1 )
//				{
//					// extract the RGB values
//					rgb_t rgbvalue = display->game_palette[ i ];
//
//					PALETTE_DIRTY_CLEAR( i );
//
//					shrinked_palette[ pen ].r = scale_palette_r[ RGB_RED( rgbvalue ) ];
//					shrinked_palette[ pen ].g = scale_palette_g[ RGB_GREEN( rgbvalue ) ];
//					shrinked_palette[ pen ].b = scale_palette_b[ RGB_BLUE( rgbvalue ) ];
//					set_color( pen, &shrinked_palette[ pen ] );
//				}
//				else
//				{
//					if( pen < RESERVED_PENS )
//					{
//						/* the color uses a reserved pen, the only thing we can do is remap it */
//						for( j = i; j < display->game_palette_entries; j++ )
//						{
//							if( ( blit_lookup_low[ j ] & 0xff ) == pen && PALETTE_DIRTY( j ) )
//							{
//								PALETTE_DIRTY_CLEAR( j );
//
//								remap_color[ j ] = 1;
//							}
//						}
//					}
//					else
//					{
//						// extract the RGB values
//						rgb_t rgbvalue = display->game_palette[ i ];
//						int r = scale_palette_r[ RGB_RED( rgbvalue ) ];
//						int g = scale_palette_g[ RGB_GREEN( rgbvalue ) ];
//						int b = scale_palette_b[ RGB_BLUE( rgbvalue ) ];
//
//						/* the pen is shared with other colors, let's see if all of them */
//						/* have been changed to the same value */
//						for( j = 0; j < display->game_palette_entries; j++ )
//						{
//							if( ( blit_lookup_low[ j ] & 0xff ) == pen )
//							{
//								rgb_t rgbvalue2 = display->game_palette[ j ];
//								int r2 = scale_palette_r[ RGB_RED( rgbvalue2 ) ];
//								int g2 = scale_palette_g[ RGB_GREEN( rgbvalue2 ) ];
//								int b2 = scale_palette_b[ RGB_BLUE( rgbvalue2 ) ];
//								if ( !PALETTE_DIRTY( j ) || r != r2 || g != g2 || b != b2 )
//								{
//									break;
//								}
//							}
//						}
//
//						if( j == display->game_palette_entries )
//						{
//							/* all colors sharing this pen still are the same, so we */
//							/* just change the palette. */
//							shrinked_palette[ pen ].r = r;
//							shrinked_palette[ pen ].g = g;
//							shrinked_palette[ pen ].b = b;
//							set_color( pen, &shrinked_palette[ pen ] );
//
//							for( j = i; j < display->game_palette_entries; j++ )
//							{
//								if( ( blit_lookup_low[ j ] & 0xff ) == pen && PALETTE_DIRTY( j ) )
//								{
//									PALETTE_DIRTY_CLEAR( j );
//								}
//							}
//						}
//						else
//						{
//							/* the colors sharing this pen now are different, we'll */
//							/* have to remap them. */
//							for( j = i; j < display->game_palette_entries; j++ )
//							{
//								if( ( blit_lookup_low[ j ] & 0xff ) == pen && PALETTE_DIRTY( j ) )
//								{
//									PALETTE_DIRTY_CLEAR( j );
//									remap_color[ j ] = 1;
//								}
//							}
//						}
//					}
//				}
//			}
//			if( remap_color[ i ] )
//			{
//				need++;
//			}
//		}
//
//		if( need > 0 )
//		{
//			int reuse_pens;
//			int ran_out;
//			int avail;
//			int first_free_pen;
//
//			avail = 0;
//			for( i = 0; i < TOTAL_PENS; i++ )
//			{
//				if( pen_usage_count[ i ] == 0 )
//				{
//					avail++;
//				}
//			}
//
//			reuse_pens = 0;
//			if( need > avail )
//			{
//#if VERBOSE
//				logerror( "Need %d new pens; %d available. I'll reuse some pens.\n", need, avail );
//#endif
//				reuse_pens = 1;
//				build_rgb_to_pen();
//			}
//
//			for( ;; )
//			{
//				ran_out = 0;
//				for( i = 0; i < display->game_palette_entries; i++ )
//				{
//					if( remap_color[ i ] )
//					{
//						rgb_t rgbvalue = display->game_palette[ i ];
//						int r = scale_palette_r[ RGB_RED( rgbvalue ) ];
//						int g = scale_palette_g[ RGB_GREEN( rgbvalue ) ];
//						int b = scale_palette_b[ RGB_BLUE( rgbvalue ) ];
//
//						pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ]--;
//
//						if( reuse_pens )
//						{
//							int pen = rgb6_to_pen[ r ][ g ][ b ];
//							if( pen != TOTAL_PENS )
//							{
//								blit_lookup_low[ i ] = pen * cmultiply;
//								remap_color[ i ] = 0;
//							}
//						}
//
//						/* if we still haven't found a pen, choose a new one */
//						if( remap_color[ i ] )
//						{
//							/* if possible, reuse the last associated pen */
//							if( pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ] == 0 )
//							{
//								logerror( "reuse last associated pen %d %d\n", i, blit_lookup_low[ i ] & 0xff );
//								remap_color[ i ] = 0;
//							}
//							else
//							{
//								/* allocate a new pen */
//								first_free_pen = RESERVED_PENS;
//								while( first_free_pen < TOTAL_PENS && pen_usage_count[ first_free_pen ] > 0 )
//								{
//									first_free_pen++;
//								}
//
//								if( first_free_pen < TOTAL_PENS )
//								{
//									blit_lookup_low[ i ] = first_free_pen * cmultiply;
//									remap_color[ i ] = 0;
//								}
//								else if( reuse_pens )
//								{
//									/* if the pen doesn't match any of the
//										palette entries then just change it */
//									int pen = ( blit_lookup_low[ i ] & 0xff );
//									if( pen >= RESERVED_PENS )
//									{
//										int rr = shrinked_palette[ pen ].r;
//										int gg = shrinked_palette[ pen ].g;
//										int bb = shrinked_palette[ pen ].b;
//										for( j = 0; j < display->game_palette_entries; j++ )
//										{
//											if( ( blit_lookup_low[ j ] & 0xff ) == pen )
//											{
//												rgb_t rgbvalue2 = display->game_palette[ j ];
//												int r2 = scale_palette_r[ RGB_RED( rgbvalue2 ) ];
//												int g2 = scale_palette_g[ RGB_GREEN( rgbvalue2 ) ];
//												int b2 = scale_palette_b[ RGB_BLUE( rgbvalue2 ) ];
//												if ( rr == r2 && gg == g2 && bb == b2 )
//												{
//													break;
//												}
//											}
//										}
//										if( j == display->game_palette_entries )
//										{
//											logerror( "reuse current pen %d %d\n", i, blit_lookup_low[ i ] & 0xff );
//											remap_color[ i ] = 0;
//										}
//									}
//								}
//							}
//							if( !remap_color[ i ] )
//							{
//								int pen = ( blit_lookup_low[ i ] & 0xff );
//								int rr = shrinked_palette[ pen ].r;
//								int gg = shrinked_palette[ pen ].g;
//								int bb = shrinked_palette[ pen ].b;
//								if( rgb6_to_pen[ rr ][ gg ][ bb ] == pen )
//								{
//									rgb6_to_pen[ rr ][ gg ][ bb ] = TOTAL_PENS;
//								}
//
//								shrinked_palette[ pen ].r = r;
//								shrinked_palette[ pen ].g = g;
//								shrinked_palette[ pen ].b = b;
//								set_color( pen, &shrinked_palette[ pen ] );
//
//								if( rgb6_to_pen[ r ][ g ][ b ] == TOTAL_PENS)
//								{
//									rgb6_to_pen[ r ][ g ][ b ] = pen;
//								}
//							}
//							else
//							{
//								ran_out++;
//							}
//						}
//						pen_usage_count[ ( blit_lookup_low[ i ] & 0xff ) ]++;
//					}
//				}
//				if( ran_out == 0 )
//				{
//					break;
//				}
//
//				if( !reuse_pens )
//				{
//					/* Ran out of pens! Let's see what we can do. */
//					/* from now on, try to reuse already allocated pens */
//					reuse_pens = 1;
//				}
//				else if( !compress_palette( display ) )
//				{
//					/* no free pens, reduce color accuracy */
//					if( palette_bit < MAX_PALETTE_BITS - 1 )
//					{
//						palette_bit++;
//						scale_palette_r = scale_palette[ palette_bits[ palette_bit ].r ];
//						scale_palette_g = scale_palette[ palette_bits[ palette_bit ].g ];
//						scale_palette_b = scale_palette[ palette_bits[ palette_bit ].b ];
//
//						for( j = 0; j < TOTAL_PENS; j++ )
//						{
//							shrinked_palette[ j ].r = scale_palette_r[ ( ( shrinked_palette[ j ].r * 255 ) + 31 ) / 63 ];
//							shrinked_palette[ j ].g = scale_palette_g[ ( ( shrinked_palette[ j ].g * 255 ) + 31 ) / 63 ];
//							shrinked_palette[ j ].b = scale_palette_b[ ( ( shrinked_palette[ j ].b * 255 ) + 31 ) / 63 ];
//							set_color( j, &shrinked_palette[ j ] );
//						}
//						compress_palette( display );
//#ifdef MAME_DEBUG
//						{
//							usrintf_showmessage( "need %d colors switching to r%dg%db%d", ran_out,
//								palette_bits[ palette_bit ].r,
//								palette_bits[ palette_bit ].g,
//								palette_bits[ palette_bit ].b );
//						}
//#endif
//						logerror( "switching to r%dg%db%d\n", palette_bits[ palette_bit ].r,
//							palette_bits[ palette_bit ].g, palette_bits[ palette_bit ].b );
//					}
//					else
//					{
//						/* this should not happen */
//						usrintf_showmessage( "Error: Palette overflow -%d", ran_out );
//
//						logerror( "Error: no way to shrink the palette to 256 colors, left out %d colors.\n", ran_out );
//#if VERBOSE
//						logerror( "color list:\n");
//						for( i = 0; i < display->game_palette_entries; i++ )
//						{
//							rgb_t rgbvalue = display->game_palette[ i ];
//							int r = scale_palette_r[ RGB_RED( rgbvalue ) ];
//							int g = scale_palette_g[ RGB_GREEN( rgbvalue ) ];
//							int b = scale_palette_b[ RGB_BLUE( rgbvalue ) ];
//							logerror( "%02x %02x %02x\n", r, g, b );
//						}
//#endif
//						break;
//					}
//				}
//			}
//
//#if PEDANTIC
//			{
//				int pen;
//
//				/* invalidate unused pens to make bugs in color allocation evident. */
//				for( pen = 0; pen < TOTAL_PENS; pen++ )
//				{
//					if( pen_usage_count[ pen ] == 0 )
//					{
//						shrinked_palette[ pen ].r = scale_palette_r[ rand() & 0xff ];
//						shrinked_palette[ pen ].g = scale_palette_g[ rand() & 0xff ];
//						shrinked_palette[ pen ].b = scale_palette_b[ rand() & 0xff ];
//						set_color( pen, &shrinked_palette[ pen ] );
//					}
//				}
//			}
//#endif
//		}
//	}
//	else
	{
		// loop over dirty colors in batches of 32
		for (i = 0; i < display->game_palette_entries; i += 32)
		{
			UINT32 dirtyflags = display->game_palette_dirty[i / 32];
			if (dirtyflags)
			{
				display->game_palette_dirty[i / 32] = 0;
				
				// loop over all 32 bits and update dirty entries
				for (j = 0; j < 32; j++, dirtyflags >>= 1)
				{
					if (dirtyflags & 1)
					{
						// extract the RGB values
						rgb_t rgbvalue = display->game_palette[i + j];
						int r = RGB_RED(rgbvalue);
						int g = RGB_GREEN(rgbvalue);
						int b = RGB_BLUE(rgbvalue);

						blit_lookup_low[ i + j ] = makecol( r, g, b ) * cmultiply;
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
	int i;
	int r;
	int g;
	int b;

//TMK	if( video_depth == 8 )
//	{
//		if( colors < 32768 )
//		{
//			/* r6g6b6 */
//			palette_bit = 0;
//		}
//		else
//		{
//			/* r3g3b2 */
//			palette_bit = MAX_PALETTE_BITS - 1;
//		}
//		scale_palette_r = scale_palette[ palette_bits[ palette_bit ].r ];
//		scale_palette_g = scale_palette[ palette_bits[ palette_bit ].g ];
//		scale_palette_b = scale_palette[ palette_bits[ palette_bit ].b ];
//
//		for( i = 0; i < 256; i++ )
//		{
//			scale_palette[ 7 ][ i ] = 0;
//			scale_palette[ 6 ][ i ] = ( ( ( i >> 2 ) * 63 ) + 31 ) / 63;
//			scale_palette[ 5 ][ i ] = ( ( ( i >> 3 ) * 63 ) + 15 ) / 31;
//			scale_palette[ 4 ][ i ] = ( ( ( i >> 4 ) * 63 ) + 7 ) / 15;
//			scale_palette[ 3 ][ i ] = ( ( ( i >> 5 ) * 63 ) + 3 ) / 7;
//			scale_palette[ 2 ][ i ] = ( ( ( i >> 6 ) * 63 ) + 1 ) / 3;
//			scale_palette[ 1 ][ i ] = 0;
//			scale_palette[ 0 ][ i ] = 0;
//		}
//
//		if( video_attributes & VIDEO_RGB_DIRECT )
//		{
//			if( bitmap_depth == 32 )
//			{
//				/* initialise 8-8-8 to 3-3-2 reduction table */
//				for( i = 0; i < 65536; i++ )
//				{
//					r = ( ( ( i & 255 ) * 7 ) + 127 ) / 255;
//					blit_lookup_high[ i ] = ( r << 5 ) * cmultiply;
//
//					g = ( ( ( ( i >> 8 ) & 255 ) * 7 ) + 127 ) / 255;
//					b = ( ( ( i & 255 ) * 3 ) + 127 ) / 255;
//					blit_lookup_low[ i ] = ( ( g << 2 ) | b ) * cmultiply;
//				}
//			}
//			else if( bitmap_depth == 15 )
//			{
//				/* initialise 5-5-5 to 3-3-2 reduction table */
//				for( i = 0; i < 65536; i++ )
//				{
//					r = ( ( ( ( i >> 10 ) & 31 ) * 7 ) + 15 ) / 31;
//					g = ( ( ( ( i >> 5 ) & 31 ) * 7 ) + 15 ) / 31;
//					b = ( ( ( i & 31 ) * 3 ) + 15 ) / 31;
//					blit_lookup_low[ i ] = ( ( r << 5 ) | ( g << 2 ) | b ) * cmultiply;
//				}
//			}
//			/* initialize the palette to a fixed 3-3-2 mapping */
//			for( r = 0; r < 8; r++ )
//			{
//				for( g = 0; g < 8; g++ )
//				{
//					for( b = 0; b < 4; b++ )
//					{
//						int idx = ( r << 5 ) | ( g << 2 ) | b;
//						int rr = ( ( r * 255 ) + 3 ) / 7;
//						int gg = ( ( g * 255 ) + 3 ) / 7;
//						int bb = ( ( b * 255 ) + 1 ) / 3;
//
//						shrinked_palette[ idx ].r = scale_palette_r[ rr ];
//						shrinked_palette[ idx ].g = scale_palette_g[ gg ];
//						shrinked_palette[ idx ].b = scale_palette_b[ bb ];
//						set_color( idx, &shrinked_palette[ idx ] );
//					}
//				}
//			}
//		}
//		else
//		{
//			/* initialise palette lookup table */
//			for( i = 0; i < 256; i++ )
//			{
//				pen_usage_count[ i ] = 0;
//				shrinked_palette[ i ].r = scale_palette_r[ 0 ];
//				shrinked_palette[ i ].g = scale_palette_g[ 0 ];
//				shrinked_palette[ i ].b = scale_palette_b[ 0 ];
//				set_color( i, &shrinked_palette[ i ] );
//			}
//
//			shrinked_palette[ WHITE_PEN ].r = scale_palette_r[ 255 ];
//			shrinked_palette[ WHITE_PEN ].g = scale_palette_g[ 255 ];
//			shrinked_palette[ WHITE_PEN ].b = scale_palette_b[ 255 ];
//			set_color( WHITE_PEN, &shrinked_palette[ WHITE_PEN ] );
//
//			for( i = 0; i < 65536; i++ )
//			{
//				blit_lookup_low[ i ] = BLACK_PEN * cmultiply;
//			}
//			pen_usage_count[ BLACK_PEN ] = 65536;
//		}
//	}
//	else
	{
		if( bitmap_depth == 32 )
		{
			if( video_depth == 15 || video_depth == 16 )
			{
				/* initialise 8-8-8 to 5-5-5/5-6-5 reduction table */
				for( i = 0; i < 65536; i++ )
				{
					blit_lookup_low[ i ] = makecol( 0, ( i >> 8 ) & 0xff, i & 0xff ) * cmultiply;
					blit_lookup_high[ i ] = makecol( i & 0xff, 0, 0 ) * cmultiply;
				}
			}
		}
		else
		{
			/* initialize the palette to a fixed 5-5-5 mapping */
			for (r = 0; r < 32; r++)
			{
				for (g = 0; g < 32; g++)
				{
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

/* center image inside the display based on the visual area */
static void update_visible_area(struct mame_display *display)
{
	int align;
	int act_width;

	act_width = gfx_width;

	if (show_debugger)
	{
		vis_min_x = 0;
		vis_max_x = display->debug_bitmap->width-1;
		vis_min_y = 0;
		vis_max_y = display->debug_bitmap->height-1;
	}
	else
	{
		if( video_swapxy )
		{
			vis_min_x = display->game_visible_area.min_y;
			vis_max_x = display->game_visible_area.max_y;
			vis_min_y = display->game_visible_area.min_x;
			vis_max_y = display->game_visible_area.max_x;
		}
		else
		{
			vis_min_x = display->game_visible_area.min_x;
			vis_max_x = display->game_visible_area.max_x;
			vis_min_y = display->game_visible_area.min_y;
			vis_max_y = display->game_visible_area.max_y;
		}
	}

	logerror("set visible area %d-%d %d-%d\n",vis_min_x,vis_max_x,vis_min_y,vis_max_y);

	viswidth  = vis_max_x - vis_min_x + 1;
	visheight = vis_max_y - vis_min_y + 1;

	if( viswidth <= 1 || visheight <= 1 )
	{
		return;
	}

	if (show_debugger)
	{
		xmultiply = ymultiply = 1;
	}
	else
	{
		/* setup xmultiply to handle SVGA driver's (possible) double width */
		xmultiply = act_width / gfx_width;
		ymultiply = 1;

		if( ( video_attributes & VIDEO_TYPE_VECTOR ) == 0 )
		{
			if( stretch )
			{
				int xm;
				int ym;

				if( keep_aspect )
				{
					int reqwidth;
					int reqheight;
					double adjusted_ratio;

					adjusted_ratio = ( ( (double)gfx_width / (double)gfx_height ) / screen_aspect ) * bitmap_aspect_ratio;

					reqwidth = ( gfx_width / viswidth ) * viswidth;
					reqheight = (int)((double)reqwidth / adjusted_ratio);

					// scale up if too small
					if (reqwidth < viswidth)
					{
						reqwidth = viswidth;
						reqheight = (int)((double)reqwidth / adjusted_ratio);
					}
					if (reqheight < visheight)
					{
						reqheight = visheight;
						reqwidth = (int)((double)reqheight * adjusted_ratio);
					}

					// scale down if too big
					if (reqwidth > gfx_width)
					{
						reqwidth = ( gfx_width / viswidth ) * viswidth;
						reqheight = (int)((double)reqwidth / adjusted_ratio);
					}
					if (reqheight > gfx_height)
					{
						reqheight = ( gfx_height / visheight ) * visheight;
						reqwidth = (int)((double)reqheight * adjusted_ratio);
					}

#define VISDIV ( 2 )
					/* round to 33% of visible size, clamp at screen size */
					reqwidth = ( reqwidth * VISDIV ) + viswidth;
					if( reqwidth > gfx_width * VISDIV )
					{
						reqwidth = gfx_width * VISDIV;
					}
					reqheight = ( reqheight * VISDIV ) + visheight;
					if( reqheight > gfx_height * VISDIV )
					{
						reqheight = gfx_height * VISDIV;
					}

					xm = ( reqwidth / viswidth ) / VISDIV;
					ym = ( reqheight / visheight ) / VISDIV;
				}
				else
				{
					xm = ( gfx_width / viswidth );
					ym = ( gfx_height / visheight );
				}
				if( xm < 1 )
				{
					xm = 1;
				}
				if( ym < 1 )
				{
					ym = 1;
				}

				xmultiply *= xm;
				ymultiply *= ym;
			}
			else
			{
				if ((video_attributes & VIDEO_PIXEL_ASPECT_RATIO_MASK) == VIDEO_PIXEL_ASPECT_RATIO_1_2)
				{
					if (video_swapxy)
					{
						xmultiply *= 2;
					}
					else
					{
						ymultiply *= 2;
					}
				}
				else if ((video_attributes & VIDEO_PIXEL_ASPECT_RATIO_MASK) == VIDEO_PIXEL_ASPECT_RATIO_2_1)
				{
					if (video_swapxy)
					{
						ymultiply *= 2;
					}
					else
					{
						xmultiply *= 2;
					}
				}
			}
		}

		if (xmultiply > MAX_X_MULTIPLY) xmultiply = MAX_X_MULTIPLY;
		if (ymultiply > MAX_Y_MULTIPLY) ymultiply = MAX_Y_MULTIPLY;
	}

	gfx_display_lines = visheight;
	gfx_display_columns = viswidth;

	if (show_debugger)
	{
		gfx_xoffset = 0;
		gfx_yoffset = 0;
		skiplines = 0;
		skiplinesmin = 0;
		skiplinesmax = 0;
		skipcolumns = 0;
		skipcolumnsmin = 0;
		skipcolumnsmax = 0;
	}
	else
	{
		gfx_xoffset = (act_width - viswidth * xmultiply) / 2;
		if (gfx_display_columns > act_width / xmultiply)
			gfx_display_columns = act_width / xmultiply;

		gfx_yoffset = (gfx_height - visheight * ymultiply) / 2;
			if (gfx_display_lines > gfx_height / ymultiply)
				gfx_display_lines = gfx_height / ymultiply;

		skiplinesmin = vis_min_y;
		skiplinesmax = visheight - gfx_display_lines + vis_min_y;
		skipcolumnsmin = vis_min_x;
		skipcolumnsmax = viswidth - gfx_display_columns + vis_min_x;

		/* the skipcolumns from mame.cfg/cmdline is relative to the visible area */
		if( video_flipx )
		{
			skipcolumns = skipcolumnsmax;
			if( gfx_skipcolumns >= 0 )
			{
				skipcolumns -= gfx_skipcolumns;
			}
		}
		else
		{
			skipcolumns = skipcolumnsmin;
			if( gfx_skipcolumns >= 0 )
			{
				skipcolumns += gfx_skipcolumns;
			}
		}
		if( video_flipy )
		{
			skiplines   = skiplinesmax;
			if( gfx_skiplines >= 0 )
			{
				skiplines -= gfx_skiplines;
			}
		}
		else
		{
			skiplines   = skiplinesmin;
			if( gfx_skiplines >= 0 )
			{
				skiplines += gfx_skiplines;
			}
		}

		/* Just in case the visual area doesn't fit */
		if (gfx_xoffset < 0)
		{
			if( gfx_skipcolumns < 0 )
			{
				if( video_flipx )
				{
					skipcolumns += gfx_xoffset;
				}
				else
				{
					skipcolumns -= gfx_xoffset;
				}
			}
			gfx_xoffset = 0;
		}
		if (gfx_yoffset < 0)
		{
			if( gfx_skiplines < 0 )
			{
				if( video_flipy )
				{
					skiplines   += gfx_yoffset;
				}
				else
				{
					skiplines   -= gfx_yoffset;
				}
			}
			gfx_yoffset = 0;
		}

		/* align left hand side */
		if( unchained )
		{
			align = 8;
		}
		else if( video_depth == 8 )
		{
			align = 4;
		}
		else if( video_depth == 15 || video_depth == 16 )
		{
			align = 2;
		}
		else if( video_depth == 24 )
		{
			align = 12;
		}
		else
		{
			align = 1;
		}

		gfx_xoffset -= gfx_xoffset % ( align * xdivide );

		/* Failsafe against silly parameters */
		if (skiplines < skiplinesmin)
			skiplines = skiplinesmin;
		if (skipcolumns < skipcolumnsmin)
			skipcolumns = skipcolumnsmin;
		if (skiplines > skiplinesmax)
			skiplines = skiplinesmax;
		if (skipcolumns > skipcolumnsmax)
			skipcolumns = skipcolumnsmax;

		logerror("gfx_width = %d gfx_height = %d\n"
				"gfx_xoffset = %d gfx_yoffset = %d\n"
				"xmin %d ymin %d xmax %d ymax %d\n"
				"skiplines %d skipcolumns %d\n"
				"gfx_skiplines %d gfx_skipcolumns %d\n"
				"gfx_display_lines %d gfx_display_columns %d\n"
				"xmultiply %d ymultiply %d\n"
				"xdivide %d ydivide %d unchained %d\n"
				"skiplinesmin %d skiplinesmax %d\n"
				"skipcolumnsmin %d skipcolumnsmax %d\n"
				"video_flipx %d video_flipy %d video_swapxy %d\n",
				gfx_width, gfx_height,
				gfx_xoffset, gfx_yoffset,
				vis_min_x, vis_min_y, vis_max_x, vis_max_y,
				skiplines, skipcolumns,
				gfx_skiplines, gfx_skipcolumns,
				gfx_display_lines, gfx_display_columns,
				xmultiply, ymultiply,
				xdivide, ydivide, unchained,
				skiplinesmin, skiplinesmax,
				skipcolumnsmin, skipcolumnsmax,
				video_flipx, video_flipy, video_swapxy );

		if( video_swapxy )
		{
			set_ui_visarea(skiplines, skipcolumns, skiplines+gfx_display_lines-1, skipcolumns+gfx_display_columns-1);
		}
		else
		{
			set_ui_visarea(skipcolumns, skiplines, skipcolumns+gfx_display_columns-1, skiplines+gfx_display_lines-1);
		}
	}

//	bitblit = bitblit_psp;
//	bitblit = bitblit_psp_mov;
//	bitblit = bitblit_psp_dma;
	bitblit = bitblit_psp_direct;

	if( show_debugger )
	{
		cmultiply = blit_setup( gfx_width, gfx_height, display->debug_bitmap->depth, video_depth,
			0, 1, 1, xdivide, ydivide,
			0, 0, 0, 0, 0 );
	}
	else
	{
		cmultiply = blit_setup( gfx_width, gfx_height, bitmap_depth, video_depth,
			video_attributes, xmultiply, ymultiply, xdivide, ydivide,
			vscanlines, hscanlines, video_flipx, video_flipy, video_swapxy );
	}
}



//============================================================
//	osd_create_display
//============================================================

int osd_create_display(const struct osd_create_params *params, UINT32 *rgb_components)
{
	struct mame_display dummy_display;

	if( video_swapxy )
	{
		video_width = params->height;
		video_height = params->width;
		bitmap_aspect_ratio = (double)params->aspect_y / (double)params->aspect_x;
	}
	else
	{
		video_width = params->width;
		video_height = params->height;
		bitmap_aspect_ratio = (double)params->aspect_x / (double)params->aspect_y;
	}

	bitmap_depth = params->depth;
	video_fps = params->fps;
	video_attributes = params->video_attributes;

	// clamp the frameskip value to within range
	if (frameskip < 0)
		frameskip = 0;
	if (frameskip >= FRAMESKIP_LEVELS)
		frameskip = FRAMESKIP_LEVELS - 1;

	show_debugger = 0;
	gone_to_gfx_mode = 0;

	logerror("width %d, height %d depth %d colors %d\n",video_width,video_height,bitmap_depth,params->colors);

	if (!select_display_mode(video_width,video_height,bitmap_depth,params->colors,video_attributes) )
	{
		return 1;
	}

	cmultiply = 0x00000001;
	init_palette( params->colors );

	// fill in the resulting RGB components
	if (rgb_components)
	{
		if ( bitmap_depth == 32 )
		{
			if( video_depth == 24 || video_depth == 32 )
			{
				rgb_components[0] = makecol(0xff, 0x00, 0x00);
				rgb_components[1] = makecol(0x00, 0xff, 0x00);
				rgb_components[2] = makecol(0x00, 0x00, 0xff);
			}
			else
			{
				rgb_components[0] = 0xff0000;
				rgb_components[1] = 0x00ff00;
				rgb_components[2] = 0x0000ff;
			}
		}
		else
		{
			if( video_depth == 15 || video_depth == 16 )
			{
#if defined( DIRECT15_USE_LOOKUP )
				video_attributes &= ~VIDEO_RGB_DIRECT;
			}
#else
				rgb_components[0] = makecol(0xf8, 0x00, 0x00);
				rgb_components[1] = makecol(0x00, 0xf8, 0x00);
				rgb_components[2] = makecol(0x00, 0x00, 0xf8);
			}
			else
#endif
			{
				rgb_components[0] = 0x7c00;
				rgb_components[1] = 0x03e0;
				rgb_components[2] = 0x001f;
			}
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
	if (gone_to_gfx_mode != 0)
	{
		if (frames_displayed > FRAMES_TO_SKIP)
		{
			cycles_t cps = osd_cycles_per_second();
			printf("Average FPS: %f\n",(double)cps/(end_time-start_time)*(frames_displayed-FRAMES_TO_SKIP));
		}
	}
}

static void set_debugger_focus(struct mame_display *display, int debugger_has_focus)
{
	static int temp_afs, temp_fs, temp_throttle;

	if (show_debugger != debugger_has_focus)
	{
		show_debugger = debugger_has_focus;
		debugger_focus_changed = 1;

		if (show_debugger)
		{
			// store frameskip/throttle settings
			temp_fs       = frameskip;
			temp_afs      = autoframeskip;
			temp_throttle = throttle;

			// temporarily set them to usable values for the debugger
			frameskip     = 0;
			autoframeskip = 0;
			throttle      = 1;
		}
		else
		{
			/* silly way to clear the screen */
			struct mame_bitmap *clrbitmap;

			clrbitmap = bitmap_alloc_depth(gfx_display_columns,gfx_display_lines,display->debug_bitmap->depth);
			if (clrbitmap)
			{
				fillbitmap(clrbitmap,0,NULL);
				/* three times to handle triple buffering */
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines, 0, 0 );
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines, 0, 0 );
				bitblit( clrbitmap, 0, 0, gfx_display_columns, gfx_display_lines, 0, 0 );
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


static void bitblit_dummy( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy )
{
	logerror("msdos/video.c: undefined bitblit() function for %d x %d!\n",xmultiply,ymultiply);
}

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


int osd_skip_this_frame(void)
{
	return skiptable[frameskip][frameskip_counter];
}


const char *osd_get_fps_text(const struct performance_info *performance)
{
	static char buffer[1024];
	char *dest = buffer;
	
	// display the FPS, frameskip, percent, fps and target fps
	dest += sprintf(dest, "%s%2d%4d%%%4d/%d fps", 
			autoframeskip ? "auto" : "fskp", frameskip, 
			(int)(performance->game_speed_percent + 0.5), 
			(int)(performance->frames_per_second + 0.5),
			(int)(Machine->refresh_rate + 0.5));

	/* for vector games, add the number of vector updates */
	if (Machine->drv->video_attributes & VIDEO_TYPE_VECTOR)
	{
		dest += sprintf(dest, "\n %d vector updates", performance->vector_updates_last_second);
	}
	else if (performance->partial_updates_this_frame > 1)
	{
		dest += sprintf(dest, "\n %d partial updates", performance->partial_updates_this_frame);
	}
	
	/* return a pointer to the static buffer */
	return buffer;
}

void osd_set_leds(int state)
{
}

//============================================================
//	win_orient_rect
//============================================================

void win_orient_rect(struct rectangle *_rect)
{
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
}

/* Update the display. */
void osd_update_video_and_audio(struct mame_display *display)
{
	cycles_t curr, target;
	static cycles_t prev_measure,this_frame_base;
	static cycles_t last_sound_update;

	int already_synced;
	int throttle_audio;
	cycles_t cps = osd_cycles_per_second();

	if (display->changed_flags & DEBUG_FOCUS_CHANGED)
	{
		set_debugger_focus(display, display->debug_focus);
	}

	if (display->debug_bitmap && input_ui_pressed(IPT_UI_TOGGLE_DEBUG))
	{
		set_debugger_focus(display, show_debugger ^ 1);
	}

	// if the visible area has changed, update it
	if (display->changed_flags & GAME_VISIBLE_AREA_CHANGED)
		update_visible_area(display);

	// if the refresh rate has changed, update it
	if (display->changed_flags & GAME_REFRESH_RATE_CHANGED)
	{
		video_fps = display->game_refresh_rate;
		sound_update_refresh_rate(display->game_refresh_rate);
		/* todo: vsync */
	}

	if (warming_up)
	{
		/* first time through, initialize timer */
		prev_measure = osd_cycles() - (int)((double)FRAMESKIP_LEVELS * (double)cps / video_fps);
		last_sound_update = osd_cycles() - ( 2 * cps / video_fps ) - 1;
		warming_up = 0;
	}

	if (frameskip_counter == 0)
		this_frame_base = prev_measure + (int)((double)FRAMESKIP_LEVELS * (double)cps / video_fps);

	throttle_audio = throttle;
	if( throttle_audio )
	{
		/* if too much time has passed since last sound update, disable throttling */
		/* temporarily - we wouldn't be able to keep synch anyway. */
		curr = osd_cycles();
		if ((curr - last_sound_update) > 2 * cps / video_fps)
			throttle_audio = 0;
		last_sound_update = curr;
	}
	already_synced = msdos_update_audio( throttle_audio );

	if (display->changed_flags & GAME_PALETTE_CHANGED)
	{
		// if the game palette has changed, update it
		if( !show_debugger )
		{
			update_palette(display);
		}
	}

	if ( display->changed_flags & GAME_BITMAP_CHANGED )
	{
		/* now wait until it's time to update the screen */
		if (throttle)
		{
			profiler_mark(PROFILER_IDLE);
			if (video_sync)
			{
				static cycles_t last;
	
				do
				{
//					vsync();
					sceDisplayWaitVblankStart();
					curr = osd_cycles();
				} while (cps / (curr - last) > video_fps * 11 /10);

				last = curr;
			}
			else
			{
				/* wait for video sync but use normal throttling */
				if (wait_vsync)
//					vsync();
					sceDisplayWaitVblankStart();
	
				curr = osd_cycles();

//TMK				if (already_synced == 0)
				{
				/* wait only if the audio update hasn't synced us already */
					target = this_frame_base + (int)((double)frameskip_counter * (double)cps / video_fps);

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
			curr = osd_cycles();

		if (frameskip_counter == 0)
		{
			prev_measure = curr;
		}

		/* for the FPS average calculation */
		if (++frames_displayed == FRAMES_TO_SKIP)
			start_time = curr;
		else
			end_time = curr;

		if( !show_debugger )
		{
			int dstxoffs;
			int dstyoffs;
			int srcxoffs;
			int srcyoffs;
			int srcwidth;
			int srcheight;

			dstxoffs = gfx_xoffset;
			dstyoffs = gfx_yoffset;

			srcxoffs = skipcolumns;
			srcyoffs = skiplines;

			srcwidth = gfx_display_columns;
			srcheight = gfx_display_lines;

			if( video_flipx )
			{
				srcxoffs += ( srcwidth - 1 );
			}
			if( video_flipy )
			{
				srcyoffs += ( srcheight - 1 );
			}
			if( video_swapxy )
			{
				int t;
				t = srcxoffs;
				srcxoffs = srcyoffs;
				srcyoffs = t;
			}
#if 0
			/* not working yet */
//			logerror( "game_bitmap_update %d %d %d %d\n", display->game_bitmap_update.min_x, display->game_bitmap_update.min_y, display->game_bitmap_update.max_x, display->game_bitmap_update.max_y );
//			logerror( "%d %d %d %d %d %d\n", dstxoffs, dstyoffs, srcxoffs, srcyoffs, srcwidth, srcheight );
			{
				struct rectangle updatebounds = display->game_bitmap_update;

				srcxoffs = 0;
				srcyoffs = 0;

				// adjust for more optimal bounds

				if( ( !video_swapxy && video_flipx ) || ( video_swapxy && video_flipy ) )
				{
					srcxoffs += updatebounds.max_x;
				}
				else
				{
					srcxoffs += updatebounds.min_x;
				}
				if( ( !video_swapxy && video_flipy ) || ( video_swapxy && video_flipx ) )
				{
					srcyoffs += updatebounds.max_y;
				}
				else
				{
					srcyoffs += updatebounds.min_y;
				}
				if( video_swapxy )
				{
					srcheight = updatebounds.max_x - updatebounds.min_x + 1;
					srcwidth = updatebounds.max_y - updatebounds.min_y + 1;
				}
				else
				{
					srcwidth = updatebounds.max_x - updatebounds.min_x + 1;
					srcheight = updatebounds.max_y - updatebounds.min_y + 1;
				}
				win_orient_rect(&updatebounds);

				dstxoffs += ( updatebounds.min_x - skipcolumns ) * xmultiply;
				dstyoffs += ( updatebounds.min_y - skiplines ) * ymultiply;
			}
//			logerror( "%d %d %d %d %d %d\n", dstxoffs, dstyoffs, srcxoffs, srcyoffs, srcwidth, srcheight );
#endif

			/* copy the bitmap to screen memory */
			profiler_mark( PROFILER_BLIT );
//			if (zvg_enabled != 2)
//			{
				bitblit( display->game_bitmap, srcxoffs, srcyoffs, srcwidth, srcheight, dstxoffs, dstyoffs );
//			}
			profiler_mark( PROFILER_END );
		}

		/* see if we need to give the card enough time to draw both odd/even fields of the interlaced display
			(req. for 15.75KHz Arcade Monitor Modes */
//TMK		interlace_sync();

		if( throttle && autoframeskip && frameskip_counter == 0 )
		{
			const struct performance_info *performance = mame_get_performance_info();
	
			// if we're too fast, attempt to increase the frameskip
			if (performance->game_speed_percent >= 99.5)
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

	if( show_debugger )
	{
		if( display->changed_flags & DEBUG_PALETTE_CHANGED )
		{
			update_palette_debugger( display );
		}
		if( display->changed_flags & DEBUG_BITMAP_CHANGED || xdivide != 1 || ydivide != 1 )
		{
			bitblit( display->debug_bitmap, 0, 0, gfx_display_columns, gfx_display_lines, 0, 0 );
		}
	}
	else
	{
		/* Check for PGUP, PGDN and pan screen */
		pan_display( display );

		if (input_ui_pressed(IPT_UI_FRAMESKIP_INC))
		{
			if (autoframeskip)
			{
				autoframeskip = 0;
				frameskip = 0;
			}
			else
			{
				if (frameskip == FRAMESKIP_LEVELS-1)
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
			frames_displayed = 0;
		}

		if (input_ui_pressed(IPT_UI_FRAMESKIP_DEC))
		{
			if (autoframeskip)
			{
				autoframeskip = 0;
				frameskip = FRAMESKIP_LEVELS-1;
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
			frames_displayed = 0;
		}

		if (input_ui_pressed(IPT_UI_THROTTLE))
		{
			throttle ^= 1;

			/* reset the frame counter every time the throttle key is pressed, so */
			/* we'll measure the average FPS on a consistent status. */
			frames_displayed = 0;
		}
	}

	// if the LEDs have changed, update them
	if (display->changed_flags & LED_STATE_CHANGED)
		osd_set_leds(display->led_state);

	frameskip_counter = (frameskip_counter + 1) % FRAMESKIP_LEVELS;

	poll_joysticks();
}


#define blit_swapxy video_swapxy
#define blit_flipx video_flipx
#define blit_flipy video_flipy

//============================================================
//	osd_override_snapshot
//============================================================

struct mame_bitmap *osd_override_snapshot(struct mame_bitmap *bitmap, struct rectangle *bounds)
{
	struct rectangle newbounds;
	struct mame_bitmap *copy;
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


void osd_pause(int paused)
{
}
