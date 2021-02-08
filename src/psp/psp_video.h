//============================================================
//
//	psp_video.c - PSP GU video functions
//
//============================================================

#ifndef PSP_VIDEO_H
#define PSP_VIDEO_H

#define SCR_WIDTH			480
#define SCR_HEIGHT			272
#define BUF_WIDTH			512
#define FRAMESIZE			(BUF_WIDTH * SCR_HEIGHT * sizeof(u16))
#define FRAMESIZE32 		(BUF_WIDTH * SCR_HEIGHT * sizeof(u32))

#define SLICE_SIZE			64 // change this to experiment with different page-cache sizes
#define TEXTURE_FLAGS		(GU_TEXTURE_16BIT | GU_COLOR_5551 | GU_VERTEX_16BIT | GU_TRANSFORM_2D)

#define MAKECOL15(r, g, b)	(((b & 0xf8) << 7) | ((g & 0xf8) << 2) | ((r & 0xf8) >> 3))
#define GETR15(col) 		(((col << 3) & 0xf8) | ((col >>  2) & 0x07))
#define GETG15(col) 		(((col >> 2) & 0xf8) | ((col >>  7) & 0x07))
#define GETB15(col) 		(((col >> 7) & 0xf8) | ((col >> 12) & 0x07))

#define MAKECOL32(r, g, b)	(0xff000000 | ((b & 0xff) << 16) | ((g & 0xff) << 8) | (r & 0xff))
#define GETR32(col) 		((col >>  0) & 0xff)
#define GETG32(col) 		((col >>  8) & 0xff)
#define GETB32(col) 		((col >> 16) & 0xff)

#define COLOR_BLACK 		  0,  0,  0
#define COLOR_RED			255,  0,  0
#define COLOR_GREEN 		  0,255,  0
#define COLOR_BLUE			  0,  0,255
#define COLOR_YELLOW		255,255,  0
#define COLOR_PURPLE		255,  0,255
#define COLOR_CYAN			  0,255,255
#define COLOR_WHITE 		255,255,255
#define COLOR_GRAY			127,127,127
#define COLOR_DARKRED		127,  0,  0
#define COLOR_DARKGREEN 	  0,127,  0
#define COLOR_DARKBLUE		  0,  0,127
#define COLOR_DARKYELLOW	127,127,  0
#define COLOR_DARKPURPLE	127,  0,127
#define COLOR_DARKCYAN		  0,127,127
#define COLOR_DARKGRAY		 63, 63, 63

#define GU_FRAME_ADDR(frame)		(u16 *)((u32)frame | 0x44000000)
#define CNVCOL15TO32(c) 			((GETB15(c) << 16) | (GETG15(c) << 8) | GETR15(c))

#define SWIZZLED_UV(tex, u, v)		&tex[((((v) & ~7) << 9) | (((u) & ~7) << 3))]
#define SWIZZLED_8x8(tex, idx)		&tex[(idx) << 6]
#define SWIZZLED_16x16(tex, idx)	&tex[((idx & ~31) << 8) | ((idx & 31) << 7)]
#define SWIZZLED_32x32(tex, idx)	&tex[((idx & ~15) << 10) | ((idx & 15) << 8)]
#define SWIZZLED_GAP				((BUF_WIDTH << 3) - 8*8)


struct Vertex
{
	u16 u, v;
	u16 color;
	s16 x, y, z;
};

#ifndef STRUCT_RECTANGLE
#define STRUCT_RECTANGLE
typedef struct _rectangle rectangle;
struct _rectangle
{
	int min_x;//int min_x;//int left;
	int min_y;//int max_x;//int top;
	int max_x;//int min_y;//int right;
	int max_y;//int max_y;//int bottom;
};

	#define MAME_COMPATIBLES_RECT 1
	#if (1==MAME_COMPATIBLES_RECT)
	/* ŒÝŠ· */
		#define MAME_RECTSET(x0,x1,y0,y1) {x0,x1,y0,y1}
		#define  PSP_RECTSET(x0,y0,x1,y1) {x0,y0,x1,y1}
	#else
	/* ”ñŒÝŠ· */
		#define MAME_RECTSET(x0,x1,y0,y1) {x0,y0,x1,y1}
		#define  PSP_RECTSET(x0,y0,x1,y1) {x0,y0,x1,y1}
	#endif

#endif // STRUCT_RECTANGLE

typedef struct rect_t
{
	int left;
	int top;
	int right;
	int bottom;
} RECT;

#ifdef seta
	#define ADJUST_GU_MEMORY_KB (1)
	/* rezon 128kBytes‘«‚è‚È‚¢‚Æ“®‚©‚È‚¢ (;^;) */
#endif // seta

#ifndef ADJUST_GU_MEMORY_KB
	//#define ADJUST_GU_MEMORY_KB (128)
	#define ADJUST_GU_MEMORY_KB (1)
#endif // ADJUST_GU_MEMORY_KB

#define GU_MAX_MEMORY (ADJUST_GU_MEMORY_KB * 1024)

extern u8 gulist[GU_MAX_MEMORY];

extern void *show_frame;
extern void *draw_frame;
extern void *depth_frame;
extern void *work_frame;
extern void *tex_frame;
extern RECT full_rect;

//void psp_wait_vsync(void);
//void psp_wait_vsync_frame(int count);
void psp_flip_screen(void);//int vsync);

void psp_video_init(void);
void psp_video_exit(void);
u16 *psp_frame_offs(void *frame, int offs);
u16 *psp_frame_addr(void *frame, int x, int y);
void psp_clear_screen(void);
void psp_clear_frame(void *frame);
void psp_clear_rect(void *frame, const rectangle *cliprect);
void psp_fill_frame(void *frame, u32 color);
void psp_fill_rect(void *frame, u32 color, const rectangle *cliprect);
void psp_box_rect( void *frame, u16 color, const rectangle *cliprect);

void psp_copy_rect(void *src, void *dst, const rectangle *src_rect, const rectangle *dst_rect);

void psp_box_frame(void *frame, int x1, int y1, int x2, int y2, u16 color);
//void psp_box_fill(	int x1, int y1, int x2, int y2, u16 color);
void psp_box_fill(void *frame,	int x1, int y1, int x2, int y2, u16 color);

#endif /* PSP_VIDE_H */
