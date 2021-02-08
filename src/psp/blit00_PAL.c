#include "driver.h"
#include "blit.h"
//#include "pg.h"

/* このblitは「（元も）16ビット色専用」かつ
「パレットモードあり専用」かつ
「画面回転なし」 */
/* 註：このエミュでは、PSP側は(転送負荷を考慮して) 16ビット色固定に設定してあるので、

「16ビット色(→パレット)→16ビット色」転送。拡大率は、強制１倍。 */

//#define VRAMTOP		((char *)(0x04000000 +0x40000000))	// +0x40000000 Non Chache?
//#define VRAMTOP		((char *)(0x04000000))	// +0x40000000 Non Chache?
//#define VRAMSIZE	1024*1024*2

#define CACHE_ON(a) (((u32)a)&~0x40000000)
#define CACHE_OFF(a) (((u32)a)|0x40000000)

#ifndef PSP_CHACHE_WRITEBACKED_ALL
//#define PSP_CHACHE_WRITEBACKED_ALL 1
  #define PSP_CHACHE_WRITEBACKED_ALL 0
#endif

static int blit_screenwidth;

UINT32 *blit_lookup_low/*[ 65536 ]*/;
//UINT32 blit_lookup_low[ 65536 ];
//UINT32 blit_lookup_high[ 65536 ];/*dummy*/
//UINT32 *blit_lookup_low =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32));
//UINT32 *blit_lookup_high =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32) *2);

//extern unsigned char pgGetNowFrame(void);




//#define	FRAMESIZE16			(512 * 272 * sizeof(u16))
//#define pg_vramtop ((void *)0x04000000)

#if 0
/* addr0  */
/*static*/ void *pgGetVramAddr0(unsigned int x, unsigned int y)
{
	return (
//((void *)0x44000000)+
(void *) ((x + (y << 9)) << 1) //(x*2)+(y*/*LINESIZE*/512*2)
);
}
#endif

static int g_offset_dx_dy;
static UINT8 my_show_fps;



extern char *osd_FPS_text;
//extern UINT8 wait_vsync;


//allmost perfect fit...
//para 256 va como trompada...
void bitblit_psp_direct_288( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh )
{
	int n_lineoffset;
	n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) )-sw-sw;
    UINT32 pp;
	unsigned char *p_src;
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) +sx+sx;

    int flaggerV = 0;
    int flaggerH = 0;
    int flaggerHR = 0;

    int estrecharResto = 0;

	unsigned char *pline_buf;
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	while( sh != 0 ) 	{  //{

	       unsigned char *p_dst; 	p_dst = pline_buf;
           //unsigned char *p_dstX2; 	//p_dstX2 = pline_buf2;
           unsigned int n_pos; 	n_pos = sw;
           //flagger = 1;
           //skip^=1;
           flaggerH = 0;
           flaggerHR = 0;

           while( n_pos > 0 ) {


              if( !estrecharResto ) {

                if(flaggerH == masterHorzStretch){

                   pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );

                    p_dst += 4;

                    *( (UINT32 *)p_dst ) = pp;

                    flaggerH = 0;
                }else {  //blit normal SIN reset estrechaResto

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;


                }//if horzStretch

              } else {//if estrecharResto -- blit normal + reset estrechaResto

                  pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                   estrecharResto = 0;
                   flaggerH = 0;
             }

                if(flaggerHR == masterHorizStretchRemain){
                   estrecharResto = 1;
                   flaggerHR = 0;//resetear aca, asi no se pierde resolucion ( sigue contabilizando
                   // mas alla del valor  estrecharResto)
                }

                flaggerH++;
                flaggerHR++;
                p_src += 4;
                p_dst += 4;
                n_pos-=2;
           }//(sw / 2)*4


         //}
		  pline_buf += /*LINESIZE*/512*2; //if X2 => la caga!

          if(flaggerV == masterVertStretch){
            //pline_buf += 512*2;
            flaggerV = 0;
            p_src -= (sw / 2)*4;
            p_src -= n_lineoffset;
		    sh++;
            //logWriteX("entro","entro","entro",666);
          }

          flaggerV++;
          p_src += n_lineoffset;
		  sh--;
    }

	psp_flip_screen(/*1*/);

}


//allmost perfect fit...
//para 256 va como trompada...
void bitblit_psp_direct_256( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh )
{
	int n_lineoffset;
	n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) )-sw-sw;
    UINT32 pp;
	unsigned char *p_src;
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) +sx+sx;

    int flaggerV = 0;
    int flaggerH = 0;
    int flaggerHR = 0;

    int estrecharResto = 0;

	unsigned char *pline_buf;
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	while( sh != 0 ) 	{  //{

	       unsigned char *p_dst; 	p_dst = pline_buf;
           //unsigned char *p_dstX2; 	//p_dstX2 = pline_buf2;
           unsigned int n_pos; 	n_pos = sw;
           //flagger = 1;
           //skip^=1;
           flaggerH = 0;
           flaggerHR = 0;

           while( n_pos > 0 ) {


              if( !estrecharResto ) {

                if(flaggerH == masterHorzStretch){

                   pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );

                    p_dst += 4;

                    *( (UINT32 *)p_dst ) = pp;

                    flaggerH = 0;
                }else {  //blit normal SIN reset estrechaResto

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;


                }//if horzStretch

              } else {//if estrecharResto -- blit normal + reset estrechaResto

                  pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                   estrecharResto = 0;
                   flaggerH = 0;
             }

                if(flaggerHR == masterHorizStretchRemain){
                   estrecharResto = 1;
                   flaggerHR = 0;//resetear aca, asi no se pierde resolucion ( sigue contabilizando
                   // mas alla del valor  estrecharResto)
                }

                flaggerH++;
                flaggerHR++;
                p_src += 4;
                p_dst += 4;
                n_pos-=2;
           }//(sw / 2)*4


         //}
		  pline_buf += /*LINESIZE*/512*2; //if X2 => la caga!

          if(flaggerV == masterVertStretch){
            //pline_buf += 512*2;
            flaggerV = 0;
            p_src -= (sw / 2)*4;
            p_src -= n_lineoffset;
		    sh++;
            //logWriteX("entro","entro","entro",666);
          }

          flaggerV++;
          p_src += n_lineoffset;
		  sh--;
    }

	psp_flip_screen(/*1*/);

}

void bitblit_psp_direct_320( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh )
{
	int n_lineoffset;
	n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) )-sw-sw;
    UINT32 pp;
	unsigned char *p_src;
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) +sx+sx;

    int flaggerV = 0;
    int flaggerH = 0;

	unsigned char *pline_buf;
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	while( sh != 0 ) 	{  //{

	       unsigned char *p_dst; 	p_dst = pline_buf;
           //unsigned char *p_dstX2; 	//p_dstX2 = pline_buf2;
           unsigned int n_pos; 	n_pos = sw;
           //flagger = 1;
           //skip^=1;
           flaggerH = 0;

           while( n_pos > 0 ) {


                if(flaggerH == masterHorzStretch){

                   pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );

                    p_dst += 4;

                    *( (UINT32 *)p_dst ) = pp;

                    flaggerH = 0;
                }else {

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                }

                flaggerH++;
                p_src += 4;
                p_dst += 4;
                n_pos-=2;
           }//(sw / 2)*4


         //}
		  pline_buf += /*LINESIZE*/512*2; //if X2 => la caga!

          if(flaggerV == masterVertStretch){
            //pline_buf += 512*2;
            flaggerV = 0;
            p_src -= (sw / 2)*4;
            p_src -= n_lineoffset;
		    sh++;
            //logWriteX("entro","entro","entro",666);
          }

          flaggerV++;
          p_src += n_lineoffset;
		  sh--;
    }

	psp_flip_screen(/*1*/);

}

void bitblit_psp_direct_320_noAdjust( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh )
{
	int n_lineoffset;
	n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) )-sw-sw;
    UINT32 pp;
	unsigned char *p_src;
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) +sx+sx;

    //screen X,Y == (512 * y) + x
    //screen 0,Y == (512 * y)
    g_offset_dx_dy = masterScreenOffset;//(512 * 0) + 20;

    int flaggerV = 0;
    int flaggerH = 0;

	unsigned char *pline_buf;
	pline_buf =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	while( sh != 0 ) 	{  //{

	       unsigned char *p_dst; 	p_dst = pline_buf;
           unsigned int n_pos; 	n_pos = sw;
           flaggerH = 0;

           while( n_pos > 0 ) {


                if(flaggerH == masterHorzStretch){

                   pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );

                    p_dst += 4;

                    *( (UINT32 *)p_dst ) = pp;

                    flaggerH = 0;
                }else {

                    pp =
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
                    ( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );


                    *( (UINT32 *)p_dst ) = pp;

                }

                flaggerH++;
                p_src += 4;
                p_dst += 4;
                n_pos-=2;
           }//(sw / 2)*4

		  pline_buf += /*LINESIZE*/512*2; //if X2 => la caga!

          if(flaggerV == masterVertStretch){
            flaggerV = 0;
            p_src -= (sw / 2)*4;
            p_src -= n_lineoffset;
		    sh++;
          }

          flaggerV++;
          p_src += n_lineoffset;
		  sh--;
    }
  psp_flip_screen(/*1*/);

}

void bitblit_psp_direct_normal( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh )
{
	int n_lineoffset;
	n_lineoffset = ( ( (UINT8 *)/*Machine->scr*/bitmap->line[ 1 ] ) - ( (UINT8 *)/*Machine->scr*/bitmap->line[ 0 ] ) )-sw-sw;

	unsigned char *p_src;
	p_src = ( (UINT8 *)/*Machine->scr*/bitmap->line[ sy ] ) +sx+sx;

	unsigned char *pline_buf;
//	pline_buf =(unsigned char *)g_offset_dx_dy + (pgGetNowFrame()?0:FRAMESIZE16);
#if 0
//	pline_buf =(unsigned char *)g_offset_dx_dy + (ppp_showframe?0:FRAMESIZE16);
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
{
	unsigned char *p_dst; 	p_dst = pline_buf;
	unsigned int n_pos; 	n_pos = sw;
	while( n_pos > 0 )
	{
	*( (UINT32 *)p_dst ) =
( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src   ) ] & 0x0000ffff ) |
( (UINT32)blit_lookup_low[ *( (UINT16 *)p_src+1 ) ] & 0xffff0000 );

		p_src += 4;
		p_dst += 4;
		n_pos-=2;
	}
}
		pline_buf += /*LINESIZE*/512*2;

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

extern SETTING setting;/* for PSP */
unsigned long blit_setup(
int dw, int dh, int dummy_sbpp,
//int dbpp,
int video_attributes,
//int xmultiply, int ymultiply,
//int xdivide, int ydivide,
//int dummy_vscanlines, int dummy_hscanlines,
int dummy_flipx, int dummy_flipy, int dummy_swapxy,
int g_offset_in
 )
{
	int i;

	//g_offset_dx_dy=g_offset_in;
	g_offset_dx_dy=0*512;

	my_show_fps=setting.show_fps_ON_OFF;

	blit_screenwidth = ( dw /* / blit_dxdivide */ );

		for( i = 0; i < 65536; i++ )
		{
//			blit_lookup_high[ i ] = ( blit_lookup_high[ i ] & cmask ) * blit_cmultiply;
			blit_lookup_low[ i ] = ( blit_lookup_low[ i ] & /*cmask*/0xffff ) * 0x00010001;
		}

	return /*blit_cmultiply*/0x00010001;
}

//void blit_set_buffers( int pages, int page_size ){}

/*******************************************************************************************************
 --ROTACION--
 *******************************************************************************************************/

static unsigned char *pline_buf_rot;

static int blitHV_sbpp;
static int blitHV_flipx;
static int blitHV_flipy;
static int blitHV_swapxy;
static int blitHV_screenwidth;


static unsigned char *copyline_raw_direct( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
unsigned int n_pos;
unsigned short *p_dst =pline_buf_rot;
unsigned short *p_src2 =p_src;

	n_pos = n_width;
	while( n_pos > 0 )
	{
		*p_dst =p_src2;
		p_dst ++;
		p_src2 ++;
		n_pos--;
	}
	return pline_buf_rot;
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
	p_dst = pline_buf_rot; \
\
	n_pos = n_width; \
	while( n_pos > 0 ) \
	{ \

#define COPYLINE_END \
		n_pos--; \
	} \
	return pline_buf_rot; \
} \

/* 8bpp */

/* 16bpp */

#define TRANS_2PIX \
{\
	const UINT32 n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;\
	const UINT32 n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;\
	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); \
	p_dst += 4;\
}

COPYLINE( copyline_1x_8bpp_palettized_16bpp )
	TRANS_2PIX
	TRANS_2PIX
COPYLINE_END


COPYLINE( copyline_1x_16bpp_palettized_16bpp )
	TRANS_2PIX
COPYLINE_END

#define copyline_1x_32bpp_palettized_16bpp copyline_invalid


/* 24bpp */

/* 32bpp */

#define COPYLINE_SDBPP( TYPE, SBPP ) copyline_1x_##SBPP##bpp_##TYPE##_16bpp

#define COPYLINE_TYPE( TYPE ) static unsigned char *( *blit_copyline_##TYPE[ 3 ] )( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) = \
{ \
	COPYLINE_SDBPP( TYPE,  8 ), \
	COPYLINE_SDBPP( TYPE, 16 ), \
	COPYLINE_SDBPP( TYPE, 32 )  \
};


/*
static unsigned char *( *blit_copyline_palettized[ 3 ] )( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) =
{
	COPYLINE_SDBPP( palettized,  8 ),
	COPYLINE_SDBPP( palettized, 16 ),
	COPYLINE_SDBPP( palettized, 32 )
};*/

//COPYLINE_TYPE( direct )
COPYLINE_TYPE( palettized )

static unsigned char *copiatorRotten_normal( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
	unsigned int n_pos;
	unsigned char *p_dst;

	p_dst = pline_buf_rot;

	n_pos = n_width;

    while( n_pos > 0 )  {

       {

        UINT32 n_p1, n_p2;

         n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
         n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;


         *( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 );

        p_dst += 4;
       }

		n_pos--;
	}

return pline_buf_rot;
}

static unsigned char *copiatorRotten_320_240( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
	unsigned int n_pos;
	unsigned char *p_dst;

	p_dst = pline_buf_rot;

	n_pos = n_width;

    int cont = 0;

	while( n_pos > 0 )  {

       {

        UINT32 n_p1, n_p2;

        if(cont == masterHorzStretch){

            n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
            n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

            *( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p1 & 0xffff0000 );

            p_dst += 4;

            *( (UINT32 *)p_dst ) = ( n_p2 & 0x0000ffff ) | ( n_p2 & 0xffff0000 );

          cont = 0;
        }
        else {

         n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
         n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;


         *( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 );

        }

        p_dst += 4;
       }

		n_pos--;

	cont++;
	}

return pline_buf_rot;
}


void bitblit_psp_direct_rotated( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh)//, int dx, int dy )
{
//	int n_dstwidth;
//	int n_dstoffset;
	int n_lineoffset;
	int n_pixeloffset;
	unsigned char *p_src;

	/* Align on a quadword */
	p_src = ( (UINT8 *)bitmap->line[ sy ] ) + ( blitHV_sbpp / 8 ) * sx;

	//if( blitHV_swapxy )
	{
		n_lineoffset = ( blitHV_sbpp / 8 );
		n_pixeloffset = ( ( (UINT8 *)bitmap->line[ 1 ] ) - ( (UINT8 *)bitmap->line[ 0 ] ) );
	}
	/*else
	{
		//n_lineoffset = ( ( (UINT8 *) bitmap->line[ 1 ] ) - ( (UINT8 *) bitmap->line[ 0 ] ) );
		/*n_pixeloffset = ( blitHV_sbpp / 8 );
	}*/

	if( blitHV_flipx ){ n_pixeloffset *= -1;	}
	if( blitHV_flipy ){ n_lineoffset  *= -1;	}

	int n_srcwidth;
	n_srcwidth = ( ( ( sw ) * (   blitHV_sbpp     / 8 )                  ) + 3 ) / 4;

	pline_buf_rot =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// software rendering. so cache on!
	pline_buf_rot = (u8*)CACHE_ON(pline_buf_rot);
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

	while( sh != 0 ) 	{   {

            blit_copyline( p_src, n_srcwidth, n_pixeloffset );
			pline_buf_rot += /*LINESIZE*/512*2;


		}

		p_src += n_lineoffset;
		sh--;
	}

	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// put back cache off and writeback Dcache
	pline_buf_rot = (u8*)CACHE_OFF(pline_buf_rot);
	sceKernelDcacheWritebackInvalidateAll();
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

	psp_flip_screen(/*1*/);
}


void bitblit_psp_direct_rotated_320_240( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh)//, int dx, int dy )
{
	int n_lineoffset;
	int n_pixeloffset;
	unsigned char *p_src;
	int flaggerV = 0;
    unsigned char *p_src_aux;

	/* Align on a quadword */
	p_src = ( (UINT8 *)bitmap->line[ sy ] ) + ( blitHV_sbpp / 8 ) * sx;

		n_lineoffset = ( blitHV_sbpp / 8 );
		n_pixeloffset = ( ( (UINT8 *)bitmap->line[ 1 ] ) - ( (UINT8 *)bitmap->line[ 0 ] ) );

	if( blitHV_flipx ) n_pixeloffset *= -1;//<--- jackal, pbaction: siempre hay
    if( blitHV_flipy ) n_lineoffset  *= -1;

	int n_srcwidth;
	n_srcwidth = ( ( ( sw ) * (   blitHV_sbpp     / 8 )                  ) + 3 ) / 4;

	pline_buf_rot =(unsigned char *)psp_frame_offs(draw_frame, 0);//(512 * 8) + 30

	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// software rendering. so cache on!
	pline_buf_rot = (u8*)CACHE_ON(pline_buf_rot);
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

    while( sh != 0 ) 	{   {

      p_src_aux = p_src;

      copiatorRotten_320_240( p_src, n_srcwidth, n_pixeloffset );

      pline_buf_rot += /*LINESIZE*/512*2;

	  }

        if(flaggerV == masterVertStretch){
            flaggerV = 0;
            p_src = p_src_aux;
            p_src -= n_lineoffset;
		    sh++;
          }

          flaggerV++;

		p_src += n_lineoffset;
		sh--;
	}

	#if (1==PSP_CHACHE_WRITEBACKED_ALL)
	// put back cache off and writeback Dcache
	pline_buf_rot = (u8*)CACHE_OFF(pline_buf_rot);
	sceKernelDcacheWritebackInvalidateAll();
	#endif //(1==PSP_CHACHE_WRITEBACKED_ALL)

	psp_flip_screen(/*1*/);
}


unsigned long blit_setup_rot(
int dw, int dh, int sbpp,
//int dbpp,
int video_attributes,
int flipx, int flipy, int swapxy,
int g_offset_in
 )
{
	int i;

    g_offset_dx_dy=g_offset_in;

	pline_buf_rot =(unsigned char *)psp_frame_offs(draw_frame, g_offset_dx_dy);

	my_show_fps=setting.show_fps_ON_OFF;

	blitHV_sbpp = sbpp;
	if( blitHV_sbpp == 15 ) blitHV_sbpp++;

	blitHV_flipx = flipx;
	blitHV_flipy = flipy;
	blitHV_swapxy = swapxy;

	blitHV_screenwidth = ( dw /* / blit_dxdivide */ );

	{

		if( blitHV_sbpp == 16/*blit_dbpp*/ && !blitHV_swapxy && !blitHV_flipx && /*blit_*//*xmultiply*/1 == 1 && /*blit_dxdivide == 1 &&*/ blitHV_sbpp == 8 )
		{
			blit_copyline = copyline_raw_direct;
		}
		else
		{

			blit_copyline = blit_copyline_palettized[ ((8==blitHV_sbpp)?(0):((16==blitHV_sbpp)?(1):(2))) ];
		}
	}

		for( i = 0; i < 65536; i++ )
		{
			blit_lookup_low[ i ] = ( blit_lookup_low[ i ] & 0xffff ) * 0x00010001;
		}

	return /*blit_cmultiply*/0x00010001;
}



