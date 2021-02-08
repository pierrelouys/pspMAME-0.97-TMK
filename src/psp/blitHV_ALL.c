#include "driver.h"
#include "blit.h"
//#include "pg.h"

/* このblitは「元は8ビット色か16ビット色か32ビット色」かつ
「パレットモードまたはパレットなしモード」かつ
「画面回転あり」 */
/* 註：PSPは16ビット色なので、「??ビット色(→パレット)→16ビット色」転送。拡大率は、強制１倍。 */

//#define VRAMTOP		((char *)(0x04000000 +0x40000000))	// +0x40000000 Non Chache?
#define VRAMTOP		((char *)(0x04000000))	// +0x40000000 Non Chache?
#define VRAMSIZE	1024*1024*2

#define CACHE_ON(a) (((u32)a)&~0x40000000)
#define CACHE_OFF(a) (((u32)a)|0x40000000)

#ifndef PSP_CHACHE_WRITEBACKED_ALL
//#define PSP_CHACHE_WRITEBACKED_ALL 1
  #define PSP_CHACHE_WRITEBACKED_ALL 0
#endif

//static int display_pos;
//static int display_pages;
//static int display_page_size;

//static int blit_laceline;
//static int blit_lacecolumn;

static int blitHV_screenwidth;
static int blitHV_sbpp;
//static int blit_dbpp;
//static int blit_xmultiply;
//static int blit_ymultiply;
//static int blit_sxdivide;
//static int blit_sydivide;
//static int blit_dxdivide;
//static int blit_dydivide;
//static int blit_hscanlines;
//static int blit_vscanlines;
static int blitHV_flipx;
static int blitHV_flipy;
static int blitHV_swapxy;

//static unsigned char line_buf[ 65536 ];
//static unsigned char line_buf[ 4 ];	// DMY
static unsigned char *pline_buf;
//static unsigned char line_buf[ 512 *272 *2 ];
//static int psp_dma_offset;
//static int psp_dma_offset2;

UINT32 *blit_lookup_low/*[ 65536 ]*/;
UINT32 blit_lookup_high[ 65536 ];
//UINT32 *blit_lookup_low =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32));
//UINT32 *blit_lookup_high =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32) *2);

//static unsigned char *copyline_raw( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
//{
//	return p_src;
//}



//#define pg_vramtop ((char *)0x04000000)

/* addr0  */
//unsigned char *pgGetVramAddr0(unsigned int x, unsigned int y)
//{
//	return (
//pg_vramtop+
//0x40000000+
//((x + (y << 9)) << 1) //(x*2)+(y*/*LINESIZE*/512*2)
//);
//}

static int g_offset_dx_dy;
static UINT8 my_show_fps;




static unsigned char *copyline_raw_direct( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
unsigned int n_pos;
unsigned short *p_dst =pline_buf;
unsigned short *p_src2 =p_src;

	n_pos = n_width;
	while( n_pos > 0 )
	{
		*p_dst =p_src2;
		p_dst ++;
		p_src2 ++;
		n_pos--;
	}
	return pline_buf;
}

static unsigned char *copyline_invalid( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
	//logerror( "copyline_invalid\n" );
	return p_src;
}

static unsigned char *( *blit_copyline )( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) = copyline_invalid;

#define COPYLINE( NAME ) \
static unsigned char *NAME( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) \
{ \
	unsigned int n_pos; \
	unsigned char *p_dst; \
\
	p_dst = pline_buf; \
\
	n_pos = n_width; \
	while( n_pos > 0 ) \
	{ \

#define COPYLINE_END \
		n_pos--; \
	} \
	return pline_buf; \
} \

/* 8bpp */

/* 16bpp */

COPYLINE( copyline_1x_8bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p3 & 0x0000ffff ) | ( n_p4 & 0xffff0000 ); p_dst += 4;
COPYLINE_END


#define copyline_1x_8bpp_direct_16bpp copyline_invalid

COPYLINE( copyline_1x_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_1x_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p2 << 16 ) ); p_dst += 4;
COPYLINE_END

#define copyline_1x_32bpp_palettized_16bpp copyline_invalid

COPYLINE( copyline_1x_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
COPYLINE_END


/* 24bpp */

/* 32bpp */

#define COPYLINE_SDBPP( TYPE, SBPP ) copyline_1x_##SBPP##bpp_##TYPE##_16bpp

#define COPYLINE_TYPE( TYPE ) static unsigned char *( *blit_copyline_##TYPE[ 3 ] )( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) = \
{ \
	COPYLINE_SDBPP( TYPE,  8 ), \
	COPYLINE_SDBPP( TYPE, 16 ), \
	COPYLINE_SDBPP( TYPE, 32 )  \
};

COPYLINE_TYPE( direct )
COPYLINE_TYPE( palettized )


extern char *osd_FPS_text;
//extern UINT8 wait_vsync;

void bitblit_psp_direct( mame_bitmap *bitmap, int sx, int sy, int sw, int sh)//, int dx, int dy )
{
//	int n_dstwidth;
//	int n_dstoffset;
	int n_lineoffset;
	int n_pixeloffset;
	unsigned char *p_src;

	/* Align on a quadword */
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) + ( blitHV_sbpp / 8 ) * sx;
	if( blitHV_swapxy )
	{
		n_lineoffset = ( blitHV_sbpp / 8 );
		n_pixeloffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) );
	}
	else
	{
		n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) );
		n_pixeloffset = ( blitHV_sbpp / 8 );
	}
	if( blitHV_flipx ){ n_pixeloffset *= -1;	}
	if( blitHV_flipy ){ n_lineoffset  *= -1;	}

/*	p_src += ( blit_lacecolumn * n_pixeloffset ) + ( blit_laceline * n_lineoffset );*/
/*	blit_lacecolumn++; 	blit_lacecolumn %= 1*//*blit_sxdivide*/;
/*	blit_laceline++; 	blit_laceline %= 1*//*blit_sydivide*/;
	int n_srcwidth;
	n_srcwidth = ( ( ( sw ) * (   blitHV_sbpp     / 8 )                  ) + 3 ) / 4;
//	n_dstwidth = ( ( ( sw ) * ( /*blit_dbpp*/16 / 8 ) * blit_xmultiply ) + 3 ) / 4;
//	n_dstoffset = blitHV_screenwidth;

//	pline_buf =(unsigned char *)pgGetVramAddr(/*n_xoffset*/dx, /*n_yoffset*/dy);
#if 0
	pline_buf =(unsigned char *)g_offset_dx_dy + (ppp_showframe?0:FRAMESIZE16);
#else
//	pline_buf =(unsigned char *)psp_frame_addr(draw_frame, 0/*x*/, 0/*y*/);
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);
#endif
//	pline_buf +=(unsigned char *)g_offset_dx_dy;

	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// software rendering. so cache on!
	pline_buf = (u8*)CACHE_ON(pline_buf);
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

	while( sh != 0 )
	{
//	int n_ymultiply;
//		n_ymultiply = blit_ymultiply /*- blit_hscanlines*/;
//		while( n_ymultiply != 0 )
		{
			blit_copyline( p_src, n_srcwidth, n_pixeloffset );
			pline_buf += /*LINESIZE*/512*2;
//			n_ymultiply--;
		}
		//if( blit_hscanlines )
		//{
		//	pline_buf += /*LINESIZE*/512*2;
		//}
		p_src += n_lineoffset;
		sh--;
	}
	if (my_show_fps)
	{
		/* FPS 表示 */
		//pline_buf =(void *)g_offset_dx_dy + (ppp_showframe?0:FRAMESIZE16);
		/*psp_box_fill*/psp_box_clear( draw_frame,  8,  0, 8+(7*13), 0+(5+3) );//, 0x000000 /*DEF_COLOR0*//*0*/);
	//	psp_print(      8,  0,                    0xffffff /*DEF_COLOR3*/, osd_FPS_text/*"Initialize"*/);
		{	char *s=osd_FPS_text;
		UINT16 x;	x=8;
	//	UINT16 y;	y=0;
			while (*s)
			{
				psp_putc_frame(draw_frame, x, 0/*y*/, *s, 0xffff/*color*/);
				x += 7;
				s++;
			//	if (x >= SCR_WIDTH) break;
			}
		}
	}
	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// put back cache off and writeback Dcache
	pline_buf = (u8*)CACHE_OFF(pline_buf);
	sceKernelDcacheWritebackInvalidateAll();
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

#if 0
	my_pgScreenFlip();
#else
//	if (wait_vsync){	v_sync();}/* ここでは要らない。src/psp/psp_video.c/psp_flip_screen();参照せよ。 */
	psp_flip_screen(/*1*/);
#endif
}

//static int sbpp_pos( int sbpp )
//{
//	switch(sbpp){
//	case 8:	return 0;
//	case 16:return 1;
//	default:return 2;
//	}
//}

extern SETTING setting;/* for PSP */
unsigned long blit_setup(
int dw, int dh, int sbpp,
//int dbpp,
int video_attributes,
//int xmultiply, int ymultiply,
//int xdivide, int ydivide,
//int dummy_vscanlines, int dummy_hscanlines,
int flipx, int flipy, int swapxy,
int g_offset_in
 )
{
	int i;

	g_offset_dx_dy=g_offset_in;

//	memset( line_buf, 0x00, sizeof( line_buf ) );
//	psp_dma_offset =0;
//	pline_buf =line_buf;
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	my_show_fps=setting.show_fps_ON_OFF;

//	if( sbpp == 15 ){	blitHV_sbpp = 16;		}
//	else			{	blitHV_sbpp = sbpp;	}
	blitHV_sbpp = sbpp;
	if( blitHV_sbpp == 15 ) blitHV_sbpp++;

//	if( dbpp == 15 ){	blit_dbpp = 16;		}
//	else			{	blit_dbpp = dbpp;	}

//	blit_xmultiply = xmultiply;
//	blit_ymultiply = ymultiply;
//	blit_sxdivide = 1;//xdivide;
//	blit_sydivide = 1;//ydivide;
//	blit_dxdivide = 1;//xdivide;
//	blit_dydivide = 1;//ydivide;
	//blit_vscanlines = vscanlines;
	//blit_hscanlines = hscanlines;
	blitHV_flipx = flipx;
	blitHV_flipy = flipy;
	blitHV_swapxy = swapxy;

	blitHV_screenwidth = ( dw /* / blit_dxdivide */ );

	/* disable scanlines if not multiplying */
	//if( /*blit_*/xmultiply > 1 && blit_vscanlines != 0 )
	//{		blit_vscanlines = 1;	}
	//else{	blit_vscanlines  = 0;	}
	//if( blit_ymultiply > 1 && blit_hscanlines != 0 )
	//{		blit_hscanlines = 1;	}
	//else{	blit_hscanlines = 0;	}

//	blit_lacecolumn %= 1/*blit_sxdivide*/;
//	blit_laceline   %= 1/*blit_sydivide*/;

	if( video_attributes & VIDEO_RGB_DIRECT )
	{
		if( blitHV_sbpp == 16/*blit_dbpp*/ && !blitHV_swapxy && !blitHV_flipx )//&& /*blit_*/xmultiply == 1 )
		{
// bitblit_psp_direct
//			blit_copyline = copyline_raw;
			blit_copyline = copyline_raw_direct;
			//psp_printf("copyline_raw");
		}
		else
		{
			blit_copyline = blit_copyline_direct[ ((8==blitHV_sbpp)?(0):((16==blitHV_sbpp)?(1):(2))) ];
		}
	}
	else
	{
		if( blitHV_sbpp == 16/*blit_dbpp*/ && !blitHV_swapxy && !blitHV_flipx && /*blit_*//*xmultiply*/1 == 1 && /*blit_dxdivide == 1 &&*/ blitHV_sbpp == 8 )
		{
// bitblit_psp_direct
//			blit_copyline = copyline_raw;
			blit_copyline = copyline_raw_direct;
			//psp_printf("copyline_raw");
		}
		else
		{
			blit_copyline = blit_copyline_palettized[ ((8==blitHV_sbpp)?(0):((16==blitHV_sbpp)?(1):(2))) ];
		}
	}

		for( i = 0; i < 65536; i++ )
		{
			blit_lookup_high[ i ] = ( blit_lookup_high[ i ] & 0xffff ) * 0x00010001;
			blit_lookup_low[ i ] = ( blit_lookup_low[ i ] & 0xffff ) * 0x00010001;
		}

	return /*blit_cmultiply*/0x00010001;
}

//void blit_set_buffers( int pages, int page_size ){}
