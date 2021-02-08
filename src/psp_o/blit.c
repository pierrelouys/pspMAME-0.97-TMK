#include "driver.h"
#include "blit.h"
#include "pg.h"
#include <oslib/oslib.h>

//#define VRAMTOP		((char *)(0x04000000 +0x40000000))	// +0x40000000 Non Chache?
#define VRAMTOP		((char *)(0x04000000))	// +0x40000000 Non Chache?
#define VRAMSIZE	1024*1024*2

static int display_pos;
static int display_pages;
static int display_page_size;

static int blit_laceline;
static int blit_lacecolumn;

static int blit_screenwidth;
static int blit_sbpp;
static int blit_dbpp;
static int blit_xmultiply;
static int blit_ymultiply;
static int blit_sxdivide;
static int blit_sydivide;
static int blit_dxdivide;
static int blit_dydivide;
static int blit_hscanlines;
static int blit_vscanlines;
static int blit_flipx;
static int blit_flipy;
static int blit_swapxy;
static unsigned long blit_cmultiply = 0x00000001;

//static unsigned char line_buf[ 65536 ];
static unsigned char line_buf[ 4 ];	// DMY
static unsigned char *pline_buf;
//static unsigned char line_buf[ 512 *272 *2 ];
static int psp_dma_offset;
static int psp_dma_offset2;

UINT32 blit_lookup_low[ 65536 ];
UINT32 blit_lookup_high[ 65536 ];
//UINT32 *blit_lookup_low =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32));
//UINT32 *blit_lookup_high =VRAMTOP +VRAMSIZE -(65536 *sizeof(UINT32) *2);

static unsigned char *copyline_raw( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo )
{
	return p_src;
}

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
	logerror( "copyline_invalid\n" );
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

COPYLINE( copyline_1x_8bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x000000ff ) | ( n_p2 & 0x0000ff00 ) | ( n_p3 & 0x00ff0000 ) | ( n_p4 & 0xff000000 ); p_dst += 4;
COPYLINE_END

#define copyline_1xs_8bpp_palettized_8bpp copyline_1x_8bpp_palettized_8bpp

COPYLINE( copyline_2x_8bpp_palettized_8bpp )
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

#define copyline_2xs_8bpp_palettized_8bpp copyline_2x_8bpp_palettized_8bpp

COPYLINE( copyline_3x_8bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x00ffffff ) | ( n_p2 & 0xff000000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0x0000ffff ) | ( n_p3 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p3 & 0x000000ff ) | ( n_p4 & 0xffffff00 ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_8bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xff000000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0x000000ff ) | ( n_p3 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p4 & 0x00ffff00 ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4x_8bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

#define copyline_4xs_8bpp_palettized_8bpp copyline_4x_8bpp_palettized_8bpp

#define copyline_1x_8bpp_direct_8bpp copyline_invalid
#define copyline_1xs_8bpp_direct_8bpp copyline_invalid
#define copyline_2x_8bpp_direct_8bpp copyline_invalid
#define copyline_2xs_8bpp_direct_8bpp copyline_invalid
#define copyline_3x_8bpp_direct_8bpp copyline_invalid
#define copyline_3xs_8bpp_direct_8bpp copyline_invalid
#define copyline_4x_8bpp_direct_8bpp copyline_invalid
#define copyline_4xs_8bpp_direct_8bpp copyline_invalid

COPYLINE( copyline_1x_16bpp_palettized_8bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = ( n_p1 & 0x00ff ) | ( n_p2 & 0xff00 ); p_dst += 2;
COPYLINE_END

#define copyline_1xs_16bpp_palettized_8bpp copyline_1x_16bpp_palettized_8bpp

COPYLINE( copyline_2x_16bpp_palettized_8bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
	*( (UINT16 *)p_dst ) = n_p2; p_dst += 2;
COPYLINE_END

#define copyline_2xs_16bpp_palettized_8bpp   copyline_2x_16bpp_palettized_8bpp

COPYLINE( copyline_3x_16bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x00ffffff ) | ( n_p2 & 0xff000000 ); p_dst += 4;
	*( (UINT16 *)p_dst ) = ( n_p2 & 0x0000ffff ); p_dst += 2;
COPYLINE_END

COPYLINE( copyline_3xs_16bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xff000000 ); p_dst += 4;
	*( (UINT16 *)p_dst ) = ( n_p2 & 0x000000ff ); p_dst += 2;
COPYLINE_END

COPYLINE( copyline_4x_16bpp_palettized_8bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

#define copyline_4xs_16bpp_palettized_8bpp copyline_4x_16bpp_palettized_8bpp

#define copyline_1x_16bpp_direct_8bpp copyline_1x_16bpp_palettized_8bpp
#define copyline_1xs_16bpp_direct_8bpp copyline_1xs_16bpp_palettized_8bpp
#define copyline_2x_16bpp_direct_8bpp copyline_2x_16bpp_palettized_8bpp
#define copyline_2xs_16bpp_direct_8bpp copyline_2xs_16bpp_palettized_8bpp
#define copyline_3x_16bpp_direct_8bpp copyline_3x_16bpp_palettized_8bpp
#define copyline_3xs_16bpp_direct_8bpp copyline_3xs_16bpp_palettized_8bpp
#define copyline_4x_16bpp_direct_8bpp copyline_4x_16bpp_palettized_8bpp
#define copyline_4xs_16bpp_direct_8bpp copyline_4xs_16bpp_palettized_8bpp

#define copyline_1x_32bpp_palettized_8bpp copyline_invalid
#define copyline_1xs_32bpp_palettized_8bpp copyline_invalid
#define copyline_2x_32bpp_palettized_8bpp copyline_invalid
#define copyline_2xs_32bpp_palettized_8bpp copyline_invalid
#define copyline_3x_32bpp_palettized_8bpp copyline_invalid
#define copyline_3xs_32bpp_palettized_8bpp copyline_invalid
#define copyline_4x_32bpp_palettized_8bpp copyline_invalid
#define copyline_4xs_32bpp_palettized_8bpp copyline_invalid

COPYLINE( copyline_1x_32bpp_direct_8bpp )
	UINT8 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT8 *)p_dst ) = n_p1; p_dst += 1;
COPYLINE_END

#define copyline_1xs_32bpp_direct_8bpp copyline_1x_32bpp_direct_8bpp

COPYLINE( copyline_2x_32bpp_direct_8bpp )
	UINT16 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
COPYLINE_END

#define copyline_2xs_32bpp_direct_8bpp copyline_2x_32bpp_direct_8bpp

COPYLINE( copyline_3x_32bpp_direct_8bpp )
	UINT16 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
	*( (UINT8 *)p_dst ) = n_p1; p_dst += 1;
COPYLINE_END

COPYLINE( copyline_3xs_32bpp_direct_8bpp )
	UINT16 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_4x_32bpp_direct_8bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

#define copyline_4xs_32bpp_direct_8bpp copyline_4x_32bpp_direct_8bpp

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

#define copyline_1xs_8bpp_palettized_16bpp   copyline_1x_8bpp_palettized_16bpp

COPYLINE( copyline_2x_8bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

#define copyline_2xs_8bpp_palettized_16bpp copyline_2x_8bpp_palettized_16bpp

COPYLINE( copyline_3x_8bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p3 & 0x0000ffff ) | ( n_p4 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_8bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0x0000ffff ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p4 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p4 & 0x0000ffff ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4x_8bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

#define copyline_4xs_8bpp_palettized_16bpp copyline_4x_8bpp_palettized_16bpp

#define copyline_1x_8bpp_direct_16bpp copyline_invalid
#define copyline_1xs_8bpp_direct_16bpp copyline_invalid
#define copyline_2x_8bpp_direct_16bpp copyline_invalid
#define copyline_2xs_8bpp_direct_16bpp copyline_invalid
#define copyline_3x_8bpp_direct_16bpp copyline_invalid
#define copyline_3xs_8bpp_direct_16bpp copyline_invalid
#define copyline_4x_8bpp_direct_16bpp copyline_invalid
#define copyline_4xs_8bpp_direct_16bpp copyline_invalid

COPYLINE( copyline_1x_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); p_dst += 4;
COPYLINE_END

#define copyline_1xs_16bpp_palettized_16bpp copyline_1x_16bpp_palettized_16bpp

COPYLINE( copyline_2x_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

#define copyline_2xs_16bpp_palettized_16bpp copyline_2x_16bpp_palettized_16bpp

COPYLINE( copyline_3x_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ) | ( n_p2 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0xffff0000 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0x0000ffff ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4x_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_16bpp_palettized_16bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 & 0x0000ffff ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_1x_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p2 << 16 ) ); p_dst += 4;
COPYLINE_END

#define copyline_1xs_16bpp_direct_16bpp copyline_1x_16bpp_direct_16bpp

COPYLINE( copyline_2x_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 | ( n_p2 << 16 ) ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_2xs_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3x_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p2 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 | ( n_p2 << 16 ) ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 << 16 ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4x_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 | ( n_p2 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 | ( n_p2 << 16 ) ); p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_16bpp_direct_16bpp )
	UINT16 n_p1;
	UINT16 n_p2;

	n_p1 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;
	n_p2 = *( (UINT16 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = ( n_p1 | ( n_p1 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p2 | ( n_p2 << 16 ) ); p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

#define copyline_1x_32bpp_palettized_16bpp copyline_invalid
#define copyline_1xs_32bpp_palettized_16bpp copyline_invalid
#define copyline_2x_32bpp_palettized_16bpp copyline_invalid
#define copyline_2xs_32bpp_palettized_16bpp copyline_invalid
#define copyline_3x_32bpp_palettized_16bpp copyline_invalid
#define copyline_3xs_32bpp_palettized_16bpp copyline_invalid
#define copyline_4x_32bpp_palettized_16bpp copyline_invalid
#define copyline_4xs_32bpp_palettized_16bpp copyline_invalid

COPYLINE( copyline_1x_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
COPYLINE_END

#define copyline_1xs_32bpp_direct_16bpp copyline_1x_32bpp_direct_16bpp

COPYLINE( copyline_2x_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

#define copyline_2xs_32bpp_direct_16bpp copyline_2x_32bpp_direct_16bpp

COPYLINE( copyline_3x_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT16 *)p_dst ) = n_p1; p_dst += 2;
COPYLINE_END

COPYLINE( copyline_3xs_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_4x_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_32bpp_direct_16bpp )
	UINT32 n_p1;

	n_p1 = blit_lookup_high[ *( (UINT32 *)p_src ) >> 16 ] | blit_lookup_low[ *( (UINT32 *)p_src ) & 0xffff ];
	p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = ( n_p1 & 0x0000ffff ); p_dst += 4;
COPYLINE_END

/* 24bpp */

COPYLINE( copyline_1x_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
COPYLINE_END

#define copyline_1xs_8bpp_palettized_24bpp copyline_1x_8bpp_palettized_24bpp

COPYLINE( copyline_2x_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_2xs_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_3x_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_3xs_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_4x_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_4xs_8bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 6;
COPYLINE_END

#define copyline_1x_8bpp_direct_24bpp copyline_invalid
#define copyline_1xs_8bpp_direct_24bpp copyline_invalid
#define copyline_2x_8bpp_direct_24bpp copyline_invalid
#define copyline_2xs_8bpp_direct_24bpp copyline_invalid
#define copyline_3x_8bpp_direct_24bpp copyline_invalid
#define copyline_3xs_8bpp_direct_24bpp copyline_invalid
#define copyline_4x_8bpp_direct_24bpp copyline_invalid
#define copyline_4xs_8bpp_direct_24bpp copyline_invalid

COPYLINE( copyline_1x_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
COPYLINE_END

#define copyline_1xs_16bpp_palettized_24bpp copyline_1x_16bpp_palettized_24bpp

COPYLINE( copyline_2x_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_2xs_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_3x_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_3xs_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_4x_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_4xs_16bpp_palettized_24bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 6;
COPYLINE_END

#define copyline_1x_16bpp_direct_24bpp copyline_1x_16bpp_palettized_24bpp
#define copyline_1xs_16bpp_direct_24bpp copyline_1xs_16bpp_palettized_24bpp
#define copyline_2x_16bpp_direct_24bpp copyline_2x_16bpp_palettized_24bpp
#define copyline_2xs_16bpp_direct_24bpp copyline_2xs_16bpp_palettized_24bpp
#define copyline_3x_16bpp_direct_24bpp copyline_3x_16bpp_palettized_24bpp
#define copyline_3xs_16bpp_direct_24bpp copyline_3xs_16bpp_palettized_24bpp
#define copyline_4x_16bpp_direct_24bpp copyline_4x_16bpp_palettized_24bpp
#define copyline_4xs_16bpp_direct_24bpp copyline_4xs_16bpp_palettized_24bpp

#define copyline_1x_32bpp_palettized_24bpp copyline_invalid
#define copyline_1xs_32bpp_palettized_24bpp copyline_invalid
#define copyline_2x_32bpp_palettized_24bpp copyline_invalid
#define copyline_2xs_32bpp_palettized_24bpp copyline_invalid
#define copyline_3x_32bpp_palettized_24bpp copyline_invalid
#define copyline_3xs_32bpp_palettized_24bpp copyline_invalid
#define copyline_4x_32bpp_palettized_24bpp copyline_invalid
#define copyline_4xs_32bpp_palettized_24bpp copyline_invalid

COPYLINE( copyline_1x_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
COPYLINE_END

#define copyline_1xs_32bpp_direct_24bpp copyline_1x_32bpp_direct_24bpp

COPYLINE( copyline_2x_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_2xs_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_3x_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_3xs_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
COPYLINE_END

COPYLINE( copyline_4x_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
COPYLINE_END

COPYLINE( copyline_4xs_32bpp_direct_24bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 3;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 6;
COPYLINE_END

/* 32bpp */

COPYLINE( copyline_1x_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

#define copyline_1xs_8bpp_palettized_32bpp copyline_1x_8bpp_palettized_32bpp

COPYLINE( copyline_2x_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_2xs_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_3x_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_4x_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_8bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;
	UINT32 n_p3;
	UINT32 n_p4;

	n_p1 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p3 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;
	n_p4 = blit_lookup_low[ *( p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p3; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p4; p_dst += 8;
COPYLINE_END

#define copyline_1x_8bpp_direct_32bpp copyline_invalid
#define copyline_1xs_8bpp_direct_32bpp copyline_invalid
#define copyline_2x_8bpp_direct_32bpp copyline_invalid
#define copyline_2xs_8bpp_direct_32bpp copyline_invalid
#define copyline_3x_8bpp_direct_32bpp copyline_invalid
#define copyline_3xs_8bpp_direct_32bpp copyline_invalid
#define copyline_4x_8bpp_direct_32bpp copyline_invalid
#define copyline_4xs_8bpp_direct_32bpp copyline_invalid

COPYLINE( copyline_1x_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

#define copyline_1xs_16bpp_palettized_32bpp copyline_1x_16bpp_palettized_32bpp

COPYLINE( copyline_2x_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_2xs_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_3x_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_4x_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_16bpp_palettized_32bpp )
	UINT32 n_p1;
	UINT32 n_p2;

	n_p1 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;
	n_p2 = blit_lookup_low[ *( (UINT16 *)p_src ) ]; p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p2; p_dst += 8;
COPYLINE_END

#define copyline_1x_16bpp_direct_32bpp copyline_1x_16bpp_palettized_32bpp
#define copyline_1xs_16bpp_direct_32bpp copyline_1xs_16bpp_palettized_32bpp
#define copyline_2x_16bpp_direct_32bpp copyline_2x_16bpp_palettized_32bpp
#define copyline_2xs_16bpp_direct_32bpp copyline_2xs_16bpp_palettized_32bpp
#define copyline_3x_16bpp_direct_32bpp copyline_3x_16bpp_palettized_32bpp
#define copyline_3xs_16bpp_direct_32bpp copyline_3xs_16bpp_palettized_32bpp
#define copyline_4x_16bpp_direct_32bpp copyline_4x_16bpp_palettized_32bpp
#define copyline_4xs_16bpp_direct_32bpp copyline_4xs_16bpp_palettized_32bpp

#define copyline_1x_32bpp_palettized_32bpp copyline_invalid
#define copyline_1xs_32bpp_palettized_32bpp copyline_invalid
#define copyline_2x_32bpp_palettized_32bpp copyline_invalid
#define copyline_2xs_32bpp_palettized_32bpp copyline_invalid
#define copyline_3x_32bpp_palettized_32bpp copyline_invalid
#define copyline_3xs_32bpp_palettized_32bpp copyline_invalid
#define copyline_4x_32bpp_palettized_32bpp copyline_invalid
#define copyline_4xs_32bpp_palettized_32bpp copyline_invalid

COPYLINE( copyline_1x_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

#define copyline_1xs_32bpp_direct_32bpp copyline_1x_32bpp_direct_32bpp

COPYLINE( copyline_2x_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_2xs_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_3x_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_3xs_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
COPYLINE_END

COPYLINE( copyline_4x_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
COPYLINE_END

COPYLINE( copyline_4xs_32bpp_direct_32bpp )
	UINT32 n_p1;

	n_p1 = *( (UINT32 *)p_src ); p_src += n_pixelmodulo;

	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 4;
	*( (UINT32 *)p_dst ) = n_p1; p_dst += 8;
COPYLINE_END


#define COPYLINE_VSL( TYPE, SBPP, DBPP, XMULTIPLY, VSL ) copyline_##XMULTIPLY##x##VSL##_##SBPP##bpp_##TYPE##_##DBPP##bpp

#define COPYLINE_XMULTIPLY( TYPE, SBPP, DBPP, XMULTIPLY ) \
{ \
	COPYLINE_VSL( TYPE, SBPP, DBPP, XMULTIPLY, ), \
	COPYLINE_VSL( TYPE, SBPP, DBPP, XMULTIPLY, s ) \
} \

#define COPYLINE_DBPP( TYPE, SBPP, DBPP ) \
{ \
	COPYLINE_XMULTIPLY( TYPE, SBPP, DBPP, 1 ), \
	COPYLINE_XMULTIPLY( TYPE, SBPP, DBPP, 2 ), \
	COPYLINE_XMULTIPLY( TYPE, SBPP, DBPP, 3 ), \
	COPYLINE_XMULTIPLY( TYPE, SBPP, DBPP, 4 ) \
} \

#define COPYLINE_SBPP( TYPE, SBPP ) \
{ \
	COPYLINE_DBPP( TYPE, SBPP, 8 ), \
	COPYLINE_DBPP( TYPE, SBPP, 16 ), \
	COPYLINE_DBPP( TYPE, SBPP, 24 ), \
	COPYLINE_DBPP( TYPE, SBPP, 32 ) \
} \

#define COPYLINE_TYPE( TYPE ) static unsigned char *( *blit_copyline_##TYPE[ 3 ][ 4 ][ 4 ][ 2 ] )( unsigned char *p_src, unsigned int n_width, int n_pixelmodulo ) = \
{ \
	COPYLINE_SBPP( TYPE, 8 ), \
	COPYLINE_SBPP( TYPE, 16 ), \
	COPYLINE_SBPP( TYPE, 32 ) \
}; \

COPYLINE_TYPE( direct )
COPYLINE_TYPE( palettized )


#define BITBLIT( type ) void bitblit_##type( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy ) \
{ \
	int n_line; \
	int n_lines; \
	int n_columns; \
	int n_xoffset; \
	int n_yoffset; \
	int n_ymultiply; \
	int n_srcwidth; \
	int n_dstwidth; \
	int n_dstoffset; \
	int n_lineoffset; \
	int n_pixeloffset; \
	unsigned char *p_src; \
	unsigned long n_dstaddress; \
\
	/* Align on a quadword */ \
	n_xoffset = ( dx / blit_dxdivide ); \
	n_yoffset = ( dy / blit_dydivide ); \
	n_columns = sw; \
	n_lines = sh; \
	p_src = ( (UINT8 *)bitmap->line[ sy ] ) + ( blit_sbpp / 8 ) * sx; \
	if( blit_swapxy ) \
	{ \
		n_lineoffset = ( blit_sbpp / 8 ); \
		n_pixeloffset = ( ( (UINT8 *)bitmap->line[ 1 ] ) - ( (UINT8 *)bitmap->line[ 0 ] ) ); \
	} \
	else \
	{ \
		n_lineoffset = ( ( (UINT8 *)bitmap->line[ 1 ] ) - ( (UINT8 *)bitmap->line[ 0 ] ) ); \
		n_pixeloffset = ( blit_sbpp / 8 ); \
	} \
	if( blit_flipx ) \
	{ \
		n_pixeloffset *= -1; \
	} \
	if( blit_flipy ) \
	{ \
		n_lineoffset *= -1; \
	} \
\
	p_src += ( blit_lacecolumn * n_pixeloffset ) + ( blit_laceline * n_lineoffset ); \
	blit_lacecolumn++; \
	blit_lacecolumn %= blit_sxdivide; \
	blit_laceline++; \
	blit_laceline %= blit_sydivide; \
\
	n_lineoffset *= blit_sydivide; \
	n_pixeloffset *= blit_sxdivide; \
	n_srcwidth = ( ( ( ( n_columns + blit_sxdivide - 1 ) / blit_sxdivide ) * ( blit_sbpp / 8 ) ) + 3 ) / 4; \
	n_line = ( n_lines + blit_sydivide - 1 ) / blit_sydivide; \
	n_dstwidth = ( ( ( ( n_columns + blit_sxdivide - 1 ) / blit_sxdivide ) * ( blit_dbpp / 8 ) * blit_xmultiply ) + 3 ) / 4; \
	n_dstoffset = blit_screenwidth; \

#define BLITSCREEN_END }


/* psp */
BITBLIT( psp )
{
int i;
	//n_dstaddress = 0xa0000 + ( n_yoffset * n_dstoffset ) + n_xoffset;
	n_dstaddress =(unsigned long)pgGetVramAddr(n_xoffset, n_yoffset);

	while( n_line != 0 )
	{
		n_ymultiply = blit_ymultiply - blit_hscanlines;
		while( n_ymultiply != 0 )
		{
			//_dosmemputl( blit_copyline( p_src, n_srcwidth, n_pixeloffset ), n_dstwidth, n_dstaddress );
//			memcpy( (char *)n_dstaddress, (char *)blit_copyline( p_src, n_srcwidth, n_pixeloffset ), n_dstwidth *4 );
			short *iptr =(short *)blit_copyline( p_src, n_srcwidth, n_pixeloffset );
			short *optr =(short *)n_dstaddress;
			for (i=0;i<n_dstwidth*2;i++) {
				*optr =*iptr;
				iptr++;
				optr++;
			}
			n_dstaddress += (n_dstoffset *2);
			n_ymultiply--;
		}
		if( blit_hscanlines )
		{
			n_dstaddress += (n_dstoffset *2);
		}
		p_src += n_lineoffset;
		n_line--;
	}

//	pgcLocate(0,3);
//	pgcPuthex8(uclock());

	pgScreenFlip();
}
BLITSCREEN_END

BITBLIT( psp_mov )
{
	//n_dstaddress = 0xa0000 + ( n_yoffset * n_dstoffset ) + n_xoffset;
	n_xoffset +=(n_xoffset % 2);
	n_dstaddress =(unsigned long)pgGetVramAddr(n_xoffset, n_yoffset);

	unsigned long *optr =(unsigned long *)n_dstaddress;
	while( n_line != 0 )
	{
		n_ymultiply = blit_ymultiply - blit_hscanlines;
		while( n_ymultiply != 0 )
		{
			//_dosmemputl( blit_copyline( p_src, n_srcwidth, n_pixeloffset ), n_dstwidth, n_dstaddress );
//			memcpy( (char *)n_dstaddress, (char *)blit_copyline( p_src, n_srcwidth, n_pixeloffset ), n_dstwidth *4 );
			unsigned long *iptr =(unsigned long *)blit_copyline( p_src, n_srcwidth, n_pixeloffset );
//			for (int i=0;i<n_dstwidth*2;i++) {
//				*optr =*iptr;
//				iptr++;
//				optr++;
//			}
			__memcpy4a(optr, iptr, n_dstwidth);
//			n_dstaddress += (n_dstoffset *2);
			optr += LINESIZE/2 -n_dstwidth;
			n_ymultiply--;
		}
		if( blit_hscanlines )
		{
//			n_dstaddress += (n_dstoffset *2);
			optr += LINESIZE/2 -n_dstwidth;
		}
		p_src += n_lineoffset;
		n_line--;
	}

//	pgcLocate(0,3);
//	pgcPuthex8(uclock());

	pgScreenFlip();
}
BLITSCREEN_END

BITBLIT( psp_dma )
{
	n_dstaddress =(unsigned long)pgGetVramAddr(n_xoffset, n_yoffset);
	psp_dma_offset2 =psp_dma_offset =n_xoffset *2 +512 *2 *n_yoffset;
	pline_buf =line_buf +psp_dma_offset;

	while( n_line != 0 )
	{
		n_ymultiply = blit_ymultiply - blit_hscanlines;
		while( n_ymultiply != 0 )
		{
			blit_copyline( p_src, n_srcwidth, n_pixeloffset );
			psp_dma_offset +=1024;
			pline_buf +=1024;
			n_ymultiply--;
		}
		if( blit_hscanlines )
		{
			psp_dma_offset +=1024;
			pline_buf +=1024;
		}
		p_src += n_lineoffset;
		n_line--;
	}
    //unsigned long n_dstaddress;
	sceDmacMemcpy((void*)&n_dstaddress, &line_buf[psp_dma_offset2], psp_dma_offset -psp_dma_offset2);

	pgScreenFlip();
}
BLITSCREEN_END

BITBLIT( psp_direct )
{
	pline_buf =(unsigned char *)pgGetVramAddr(n_xoffset, n_yoffset);

	while( n_line != 0 )
	{
		n_ymultiply = blit_ymultiply - blit_hscanlines;
		while( n_ymultiply != 0 )
		{
			blit_copyline( p_src, n_srcwidth, n_pixeloffset );

			pline_buf += LINESIZE*2;
			n_ymultiply--;
		}
		if( blit_hscanlines )
		{
			pline_buf += LINESIZE*2;
		}
		p_src += n_lineoffset;
		n_line--;
	}

	pgScreenFlip();
}
BLITSCREEN_END

static int sbpp_pos( int sbpp )
{
	if( sbpp == 8 )
	{
		return 0;
	}
	else if( sbpp == 16 )
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

static int dbpp_pos( int dbpp )
{
	return ( dbpp / 8 ) - 1;
}

unsigned long blit_setup( int dw, int dh, int sbpp, int dbpp, int video_attributes, int xmultiply, int ymultiply, int xdivide, int ydivide,
	int vscanlines, int hscanlines, int flipx, int flipy, int swapxy )
{
	int i;
	int cmultiply;
	int cmask;

	if( sbpp == 15 )
	{
		blit_sbpp = 16;
	}
	else
	{
		blit_sbpp = sbpp;
	}
	if( dbpp == 15 )
	{
		blit_dbpp = 16;
	}
	else
	{
		blit_dbpp = dbpp;
	}

	blit_xmultiply = xmultiply;
	blit_ymultiply = ymultiply;
	blit_sxdivide = xdivide;
	blit_sydivide = ydivide;
	blit_dxdivide = xdivide;
	blit_dydivide = ydivide;
	blit_vscanlines = vscanlines;
	blit_hscanlines = hscanlines;
	blit_flipx = flipx;
	blit_flipy = flipy;
	blit_swapxy = swapxy;

	blit_screenwidth = ( dw / blit_dxdivide );

	/* don't multiply & divide at the same time if possible */
	if( ( blit_xmultiply % blit_sxdivide ) == 0 )
	{
		blit_xmultiply /= blit_sxdivide;
		blit_sxdivide = 1;
	}
	if( ( blit_ymultiply % blit_sydivide ) == 0 )
	{
		blit_ymultiply /= blit_sydivide;
		blit_sydivide = 1;
	}

	/* disable scanlines if not multiplying */
	if( blit_xmultiply > 1 && blit_vscanlines != 0 )
	{
		blit_vscanlines = 1;
	}
	else
	{
		blit_vscanlines  = 0;
	}
	if( blit_ymultiply > 1 && blit_hscanlines != 0 )
	{
		blit_hscanlines = 1;
	}
	else
	{
		blit_hscanlines = 0;
	}

	blit_lacecolumn %= blit_sxdivide;
	blit_laceline %= blit_sydivide;

	if( video_attributes & VIDEO_RGB_DIRECT )
	{
		if( blit_sbpp == blit_dbpp && !blit_swapxy && !blit_flipx && blit_xmultiply == 1 && blit_dxdivide == 1 )
		{
// bitblit_psp_direct
//			blit_copyline = copyline_raw;
			blit_copyline = copyline_raw_direct;
			printf_("copyline_raw");
		}
		else
		{
			blit_copyline = blit_copyline_direct[ sbpp_pos( blit_sbpp )][ dbpp_pos( blit_dbpp ) ][ blit_xmultiply - 1 ][ blit_vscanlines ];
		}
	}
	else
	{
		if( blit_sbpp == blit_dbpp && !blit_swapxy && !blit_flipx && blit_xmultiply == 1 && blit_dxdivide == 1 && blit_sbpp == 8 )
		{
// bitblit_psp_direct
//			blit_copyline = copyline_raw;
			blit_copyline = copyline_raw_direct;
			printf_("copyline_raw");
		}
		else
		{
			blit_copyline = blit_copyline_palettized[ sbpp_pos( blit_sbpp ) ][ dbpp_pos( blit_dbpp ) ][ blit_xmultiply - 1 ][ blit_vscanlines ];
		}
	}

	memset( line_buf, 0x00, sizeof( line_buf ) );
	psp_dma_offset =0;
	pline_buf =line_buf;

	if( blit_dbpp == 8 )
	{
		if( blit_xmultiply == 2 && blit_vscanlines != 0 )
		{
			cmultiply = 0x00010001;
		}
		else if( blit_xmultiply == 4 && blit_vscanlines != 0 )
		{
			cmultiply = 0x00010101;
		}
		else
		{
			cmultiply = 0x01010101;
		}
		cmask = 0xff;
	}
	else if( blit_dbpp == 16 )
	{
		if( blit_xmultiply == 2 && blit_vscanlines != 0 )
		{
			cmultiply = 0x00000001;
		}
		else
		{
			cmultiply = 0x00010001;
		}
		cmask = 0xffff;
	}
	else
	{
		cmultiply = 0x00000001;
		cmask = 0xffffffff;
	}
	if( blit_cmultiply != cmultiply )
	{
		blit_cmultiply = cmultiply;
		for( i = 0; i < 65536; i++ )
		{
			blit_lookup_high[ i ] = ( blit_lookup_high[ i ] & cmask ) * blit_cmultiply;
			blit_lookup_low[ i ] = ( blit_lookup_low[ i ] & cmask ) * blit_cmultiply;
		}
	}
	return blit_cmultiply;
}

void blit_set_buffers( int pages, int page_size )
{
	display_pos = 0;
	display_pages = pages;
	display_page_size = page_size;
}
