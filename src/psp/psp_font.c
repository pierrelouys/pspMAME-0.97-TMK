//============================================================
//
//	psp_font.c - PSP font draw functions
//
//============================================================
#include "driver.h"


//#include "psp_main.h"
#include <stdarg.h>

#define USE_CONVERT_TABLE	0

#if USE_CONVERT_TABLE
#include "font/fontTable.c"
#endif


#define FONT_TYPE_HAN	0
#define FONT_TYPE_ZEN	1

//#define isascii(c)		((c) >= 0x20 && (c) <= 0x7e)
#define is_mame_ascii(c)	((c) >= 0x10 && (c) <= 0x7e)
#define iskana(c)			((c) >= 0xa0 && (c) <= 0xdf)
#define issjis1(c)			(((c) >= 0x81 && (c) <= 0x9f) | ((c) >= 0xe0 && (c) <= 0xfc))
#define issjis2(c)			((c) >= 0x40 && (c) <= 0xfc && (c) != 0x7f)


//============================================================
//	LOCAL VARIABLES
//============================================================

//#define MAX_COLS	96
//#define MAX_ROWS	22




//============================================================
//	font_get_code
//============================================================

INLINE UINT16 font_get_code(const char *s, int *type)
{
	UINT8 c1 = (UINT8)s[0];
	UINT8 c2 = (UINT8)s[1];
	UINT16 code;

	if (issjis1(c1) && issjis2(c2))
	{
#if USE_CONVERT_TABLE
		code = sjis_table[((c1 << 8) | c2) - 0x8140];
#else
		static const UINT16 font404[] =
		{
			0xA2AF, 11,
			0xA2C2, 8,
			0xA2D1, 11,
			0xA2EB, 7,
			0xA2FA, 4,
			0xA3A1, 15,
			0xA3BA, 7,
			0xA3DB, 6,
			0xA3FB, 4,
			0xA4F4, 11,
			0xA5F7, 8,
			0xA6B9, 8,
			0xA6D9, 38,
			0xA7C2, 15,
			0xA7F2, 13,
			0xA8C1, 720,
			0xCFD4, 43,
			0xF4A5, 1030,
			0,0
		};
		UINT16 i, font_code;

		// Shift_JIS
		code = (c1 << 8) | c2;

		// Shift_JIS -> EUC
		if (code >= 0xe000) code -= 0x4000;
		code = ((((code >> 8) & 0xff) - 0x81) << 9) + (code & 0xff);
		if ((code & 0xff) >= 0x80) code--;
		if ((code & 0xff) >= 0x9e) code += 0x62;
		else code -= 0x40;
		code += 0x2121 + 0x8080;

		// EUC -> font code
		font_code = (((code >> 8) & 0xff) - 0xa1) * (0xff - 0xa1) + (code & 0xff) - 0xa1;

		i = 0;
		while (font404[i])
		{
			if (code >= font404[i])
			{
				if (code <= font404[i] + font404[i + 1] - 1)
				{
					font_code--;
					break;
				}
				font_code -= font404[i + 1];
			}
			i += 2;
		}

		code = font_code;
#endif
		*type = FONT_TYPE_ZEN;
	}
	else
	{
		code = (c1 & 0x60) ? c1 - 0x20 : 0;
		if (code & 0x80) code -= 0x20;

		*type = FONT_TYPE_HAN;
	}

	return code;
}



//============================================================
//	psp_putcw (‘SŠpŠ¿Žš)
//============================================================

void psp_putcw_frame(void *frame, UINT16 x, UINT16 y, UINT16 code, UINT16 color)
{
	UINT16 *src = (UINT16 *)&zenkaku_font10[code * 10];
	UINT16 *dst = psp_frame_addr(frame/*draw_frame,*/, x, y);
	UINT16 data;

	y = 10;
	while (y--)
//	for (y = 0; y < 10; y++)
	{
		data = *src++;
		if (data & (1 << 0)) dst[0] = color;
		if (data & (1 << 1)) dst[1] = color;
		if (data & (1 << 2)) dst[2] = color;
		if (data & (1 << 3)) dst[3] = color;
		if (data & (1 << 4)) dst[4] = color;
		if (data & (1 << 5)) dst[5] = color;
		if (data & (1 << 6)) dst[6] = color;
		if (data & (1 << 7)) dst[7] = color;
		if (data & (1 << 8)) dst[8] = color;
		if (data & (1 << 9)) dst[9] = color;
		dst += BUF_WIDTH;
	}
}


//============================================================
//	psp_putc (”¼Šp•¶Žš)
//============================================================

void psp_putc_frame(void *frame, UINT16 x, UINT16 y, UINT8 code, UINT16 color)
{
	UINT8 *src = (UINT8 *)&hankaku_font5x10[(code & 0x7f) << 3];
	UINT16 *dst = psp_frame_addr(frame/*draw_frame,*/, x, y + 1);
	UINT8 data;

//	y = 8/*10*/;
//	while (y--)
	for (y = 0; y < 8; y++)
	{
		data = *src++;
		if (data & 0x80) dst[0] = color;
		if (data & 0x40) dst[1] = color;
		if (data & 0x20) dst[2] = color;
		if (data & 0x10) dst[3] = color;
		if (data & 0x08) dst[4] = color;
		if (data & 0x04) dst[5] = color;
		if (data & 0x02) dst[6] = color;
		if (data & 0x01) dst[7] = color;
		dst += BUF_WIDTH;
	}
}



//============================================================
//	psp_print
//============================================================

void psp_print(UINT16 x, UINT16 y, UINT16 color, const char *text, ...)
{
	char *s, buf[256];
	int type;
	va_list arg;

	va_start(arg, text);
	vsprintf(buf, text, arg);
	va_end(arg);

	s = buf;

	while (*s)
	{
		UINT16 code = font_get_code(s, &type);

		if (type == FONT_TYPE_ZEN)
		{
			psp_putcw_frame(draw_frame, x, y, code, color);
			x += 10;
			s +=  2;
		}
		else
		{
			if (isascii((UINT8)(*s))) psp_putc_frame(draw_frame, x, y, *s, color);
			x += 7;
			s++;
		}
		if (x >= SCR_WIDTH)
		{
			x  =  0;
			y += 10;
			if (y >= (SCR_HEIGHT-10))
			{
				break;
			}
		}
	}
}
#if 1
//============================================================
//	psp_virtial_console_system
//============================================================

static UINT16 console_x=0;
static UINT16 console_y=0;
static UINT16 console_color=PSP_COLOR_WHITE;

//============================================================
//	psp_print_locate
//============================================================

void psp_print_locate(UINT16 x, UINT16 y)
{
	console_x=x;
	console_y=y;
}

//============================================================
//	psp_print_color
//============================================================

void psp_print_color(UINT16 set_color)
{
	console_color=set_color;
}

//============================================================
//	psp_printf_bbb (for debug virtual terminal)
//============================================================

void psp_printf_bbb(const char *text, ...)
{
	char *s, buf[256];
	int type;
	va_list arg;

	va_start(arg, text);
	vsprintf(buf, text, arg);
	va_end(arg);

	s = buf;

	while (*s)
	{
		UINT16 code = font_get_code(s, &type);

		if (type == FONT_TYPE_ZEN)
		{
			psp_putcw_frame(show_frame, console_x, console_y, code, console_color);
			console_x += 10;
			s += 2;
		}
		else
		{
			if (is_mame_ascii((UINT8)(*s)))
			{
				psp_putc_frame(show_frame, console_x, console_y, *s, console_color);
			}
			else switch ((UINT8)(*s))
			{
			case '\n':
			case '\r':
			case 0x00:
				console_x=SCR_WIDTH;/* ‰üs */
			}
			console_x += 7;
			s++;
		}
		if (console_x >= SCR_WIDTH)
		{
			console_x=0;
			console_y+=10;
			if (console_y >= (250))
			{
			//	psp_print(0, (250), MAKECOL15(255,0,0), "<<Press Any KEY!>>");
			//	psp_flip_screen();
			//	v_sync();
			//	Confirm_Control();
			//	psp_fill_frame(draw_frame, 0);
				psp_fill_frame(show_frame, 0);
				console_y=0;
				console_x=0;
			}
		}
	}
	v_sync();
}
#endif

int printfnw_(const char *format)
{
psp_printf_bbb(format);
return 0;
}





/* ‰½ŒÌ‚©‚±‚±‚ª‘¬‚¢ */

//#include "driver.h"
#if (0==LINK_ROT)
	/* force NO ROTATE 00 mame_palettized 16 mode */
	#include "blit00_PAL.c"
#elif (360==LINK_ROT)
	/* MAME ROTATE 360    mame_palettized 16 mode */
	#include "blitHV_PAL.c"
#elif (1000==LINK_ROT)
	/* force NO ROTATE 00 mame_palettized 16 mode()gu-blit */
	#include "blit1000_PAL.c"
#elif (9360==LINK_ROT)
	/* MAME ROTATE 360    mame_palettized/direct mode */
	#include "blitHV_ALL.c"
#endif // LINK_ROT




