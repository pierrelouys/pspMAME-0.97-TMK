//============================================================
//
//	psp_video.c - PSP GU video functions
//
//============================================================

#include "driver.h"//#include "psp_main.h"
#include "psp_video.h"



//============================================================
//	GLOBAL VARIABLES
//============================================================

UINT8 __attribute__((aligned(16))) gulist[GU_MAX_MEMORY];
void *show_frame;
void *draw_frame;
void *depth_frame;
void *work_frame;
void *tex_frame;
//void *set_frame;

//UINT32 psp_BG_color32;
//RECT full_rect = { 0, 0, SCR_WIDTH, SCR_HEIGHT };//???


#if 0
//============================================================
//	clear_vram
//============================================================

static void clear_vram(void)
{
	int i = 2048 * 1024 >> 2;
	UINT32 *vptr = (UINT32 *)0x44000000;

	while (i--) *vptr++ = 0;
}
#endif

#if 0
//============================================================
//	psp_wait_vsync
//============================================================

//void psp_wait_vsync(void)
//{
//	sceDisplayWaitVblankStart();
//}
#endif

#if 0
//============================================================
//	psp_wait_vsync_frame
//============================================================

//void psp_wait_vsync_frame(int count)
//{
//	while (count--) sceDisplayWaitVblankStart();
//}
#endif


//============================================================
//	psp_flip_screen
//============================================================
extern UINT8 wait_vsync;
#if 1
static UINT8 swap_buf;/* v-sync on でも速度低下なし！！！方式 */
#endif
void psp_flip_screen(void)//int vsync)
{
#if 0
/* 従来の方式(本物の v-sync では速度低下する) */
//	if (vsync)      {sceDisplayWaitVblankStart();/* v_sync(); */}
//	if (wait_vsync) {sceDisplayWaitVblankStart();/* v_sync(); */}
	show_frame = draw_frame;
	draw_frame = sceGuSwapBuffers();
#else

/* v-sync on でも速度低下なし！！！方式(ruka氏。ありがとおっ”) */

//	#define aaa_PIXELSIZE	1				//in short
	#define aaa_LINESIZE	512 			//in short
//	#define aaa_FRAMESIZE	0x44000 		//in byte
//	#define aaa_pg_vramtop ((char *)0x04000000)
	swap_buf^=1;
	if(swap_buf)
	{
		draw_frame = ((char *)0x04044000);
		show_frame = ((char *)0x04000000);
	//	sceDisplaySetFrameBuf(((char *)0x04000000), aaa_LINESIZE, 1,
	//		(wait_vsync ? PSP_DISPLAY_SETBUF_NEXTFRAME: PSP_DISPLAY_SETBUF_IMMEDIATE));
	}
	else
	{
		draw_frame = ((char *)0x04000000);
		show_frame = ((char *)0x04044000);
	//	sceDisplaySetFrameBuf(((char *)0x04044000), aaa_LINESIZE, 1,
	//		(wait_vsync ? PSP_DISPLAY_SETBUF_NEXTFRAME: PSP_DISPLAY_SETBUF_IMMEDIATE));
	}
//	sceDisplaySetFrameBuf((aaa_pg_vramtop+(swap_buf?0:aaa_FRAMESIZE)), aaa_LINESIZE, 1,
//		(wait_vsync ? PSP_DISPLAY_SETBUF_NEXTFRAME: PSP_DISPLAY_SETBUF_IMMEDIATE));
	sceDisplaySetFrameBuf((show_frame), aaa_LINESIZE, 1,
		(wait_vsync ? PSP_DISPLAY_SETBUF_NEXTFRAME: PSP_DISPLAY_SETBUF_IMMEDIATE));
//	#undef aaa_pg_vramtop
//	#undef aaa_FRAMESIZE
	#undef aaa_LINESIZE
//	#undef aaa_PIXELSIZE
#endif
}
/* 注：「sceDisplayWaitVblankCB()」を使う方式もあるらしい（謎） */
//      0x36cdfade,sceDisplayWaitVblank         （描画開始位置で待つ？）
//      0x8eb9ec49,sceDisplayWaitVblankCB       （描画開始位置のコールバック？）
//      0x984c27e7,sceDisplayWaitVblankStart    （描画終了位置で待つ？）
//      0x46f186c3,sceDisplayWaitVblankStartCB  （描画終了位置のコールバック？）

//============================================================
//	psp_video_init
//============================================================

void psp_video_init(void)
{
#if 1
	swap_buf=0;/* v-sync on でも速度低下なし！！！方式 */
#endif
	draw_frame  = (void *)(FRAMESIZE * 0);
	show_frame  = (void *)(FRAMESIZE * 1);
	work_frame  = (void *)(FRAMESIZE * 2);
	tex_frame   = (void *)(FRAMESIZE * 3);
	depth_frame = NULL;

	sceGuInit();
	sceGuDisplay(GU_FALSE);
	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBuffer(GU_PSM_5551, draw_frame, BUF_WIDTH);
	sceGuDispBuffer(SCR_WIDTH, SCR_HEIGHT, show_frame, BUF_WIDTH);
	sceGuOffset(2048 - (SCR_WIDTH / 2), 2048 - (SCR_HEIGHT / 2));
	sceGuViewport(2048, 2048, SCR_WIDTH, SCR_HEIGHT);

	sceGuEnable(GU_SCISSOR_TEST);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);

	sceGuDisable(GU_ALPHA_TEST);
	sceGuAlphaFunc(GU_LEQUAL, 0, 1);

	sceGuDisable(GU_DEPTH_TEST);
	sceGuDepthRange(65535, 0);
	sceGuDepthFunc(GU_GEQUAL);
	sceGuDepthMask(GU_TRUE);

	sceGuEnable(GU_TEXTURE_2D);
	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexScale(1.0f / BUF_WIDTH, 1.0f / BUF_WIDTH);
	sceGuTexOffset(0, 0);
	sceGuTexFunc(GU_TFX_REPLACE, GU_TCC_RGBA);
	sceGuTexFlush();

	sceGuClearDepth(0);
	sceGuClearColor(0);
	sceGuClear(GU_COLOR_BUFFER_BIT);

	sceGuFinish();
	sceGuSync(0, 0);

	//clear_vram();
	v_sync();
	psp_flip_screen(/*1*/);

	sceGuDisplay(GU_TRUE);
}



//============================================================
//	psp_video_exit
//============================================================

void psp_video_exit(void)
{
	sceGuDisplay(GU_FALSE);
	sceGuTerm();
	//clear_vram();
}



//============================================================
//	psp_frame_offs
//============================================================

UINT16 *psp_frame_offs(void *frame, int offs)
{
	return (UINT16 *)( ((UINT32)frame | 0x44000000) + (offs) );
}

//============================================================
//	psp_frame_addr
//============================================================

UINT16 *psp_frame_addr(void *frame, int x, int y)
{
	return (UINT16 *)( ((UINT32)frame | 0x44000000) + ((x + (y << 9)) << 1) );
}


#if 1
//============================================================
//	psp_clear_frame
//============================================================

void psp_clear_frame(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif

//============================================================
//	psp_clear_screen
//============================================================

void psp_clear_screen(void)
{
	psp_clear_frame(show_frame);
	psp_clear_frame(draw_frame);
}


#if 0
//============================================================
//	psp_clear_rect
//============================================================

void psp_clear_rect(void *frame, const rectangle *cliprect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(cliprect->min_x, cliprect->min_y, cliprect->max_x + 1, cliprect->max_y + 1);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif

#if 1
/* psp_main.c psp_menu.c */
//============================================================
//	psp_fill_frame
//============================================================

void psp_fill_frame(void *frame, UINT32 color32)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClearColor(color32);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif

#if 0
//============================================================
//	psp_fill_rect
//============================================================

void psp_fill_rect(void *frame, UINT32 color, const rectangle *cliprect)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(cliprect->min_x, cliprect->min_y, cliprect->max_x + 1, cliprect->max_y + 1);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif



#if 0
//============================================================
//	psp_clear_depth
//============================================================

void psp_clear_depth(void *frame)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDepthBuffer(frame, BUF_WIDTH);
	sceGuScissor(0, 0, BUF_WIDTH, SCR_HEIGHT);
	sceGuClear(GU_DEPTH_BUFFER_BIT);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif

#if 0
//============================================================
//	psp_copy_rect
//============================================================

void psp_copy_rect(void *src, void *dst, const rectangle *src_rect, const rectangle *dst_rect)
{
	int j, sw, dw, sh, dh;
	struct Vertex *vertices;

	sw = (src_rect->max_x + 1) - src_rect->min_x;
	dw = (dst_rect->max_x + 1) - dst_rect->min_x;
	sh = (src_rect->max_y + 1) - src_rect->min_y;
	dh = (dst_rect->max_y + 1) - dst_rect->min_y;

	sceGuStart(GU_DIRECT, gulist);

	sceGuDrawBufferList(GU_PSM_5551, dst, BUF_WIDTH);
	sceGuScissor(dst_rect->min_x, dst_rect->min_y, dst_rect->max_x + 1, dst_rect->max_y + 1);
	sceGuDisable(GU_ALPHA_TEST);

	sceGuTexMode(GU_PSM_5551, 0, 0, GU_FALSE);
	sceGuTexImage(0, 512, 512, BUF_WIDTH, GU_FRAME_ADDR(src));
	if (sw == dw && sh == dh)
		sceGuTexFilter(GU_NEAREST, GU_NEAREST);
	else
		sceGuTexFilter(GU_LINEAR, GU_LINEAR);

	for (j = 0; (j + SLICE_SIZE) < sw; j = j + SLICE_SIZE)
	{
    	vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->min_x + j;
		vertices[0].v = src_rect->min_y;
		vertices[0].x = dst_rect->min_x + j * dw / sw;
		vertices[0].y = dst_rect->min_y;

		vertices[1].u = src_rect->min_x + j + SLICE_SIZE;
		vertices[1].v = src_rect->max_y + 1;
		vertices[1].x = dst_rect->min_x + (j + SLICE_SIZE) * dw / sw;
		vertices[1].y = dst_rect->max_y + 1;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	if (j < sw)
	{
		vertices = (struct Vertex *)sceGuGetMemory(2 * sizeof(struct Vertex));

		vertices[0].u = src_rect->min_x + j;
		vertices[0].v = src_rect->min_y;
		vertices[0].x = dst_rect->min_x + j * dw / sw;
		vertices[0].y = dst_rect->min_y;

		vertices[1].u = src_rect->max_x + 1;
		vertices[1].v = src_rect->max_y + 1;
		vertices[1].x = dst_rect->max_x + 1;
		vertices[1].y = dst_rect->max_y + 1;

		sceGuDrawArray(GU_SPRITES, TEXTURE_FLAGS, 2, 0, vertices);
	}

	sceGuFinish();
	sceGuSync(0, 0);
}
#endif

#if 0
//============================================================
//	psp_box_rect
//============================================================

void psp_box_rect(void *frame, UINT16 color, const rectangle *cliprect)
{
	UINT16 *vptr = psp_frame_addr(frame, 0, 0);
	int i;
	for (i = cliprect->min_x; i <= cliprect->max_x; i++)
	{
		vptr[i + ((cliprect->min_y) << 9)] = color;
		vptr[i + ((cliprect->max_y) << 9)] = color;
	}
	for (i = cliprect->min_y; i <= cliprect->max_y; i++)
	{
		vptr[(cliprect->min_x) + (i << 9)] = color;
		vptr[(cliprect->max_x) + (i << 9)] = color;
	}
}
#endif

#if 1
//============================================================
//	psp_box
//============================================================

void psp_box_frame(void *frame, int x1, int y1, int x2, int y2, UINT16 color)
{
	UINT16 *vptr = psp_frame_addr(/*draw_*/frame, 0, 0);
	int i;

	for (i = x1; i <= x2; i++)
	{
		vptr[i + (y1 << 9)] = color;
		vptr[i + (y2 << 9)] = color;
	}
	for (i = y1; i <= y2; i++)
	{
		vptr[x1 + (i << 9)] = color;
		vptr[x2 + (i << 9)] = color;
	}
}
#endif

#if 1
//============================================================
//	psp_box_clear
//============================================================
/* 註：「rect構造体」使うより、コードが小さくて速い。 */
void psp_box_clear(void *frame, int x1, int y1, int x2, int y2/*, UINT32 color32*/)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(x1, y1, x2 + 1, y2 + 1);
	//sceGuClearColor(psp_BG_color32);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	//sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif
#if 0
//============================================================
//	psp_fill_rect
//============================================================

void psp_box_fill(void *frame, int x1, int y1, int x2, int y2, UINT16 color)
{
	sceGuStart(GU_DIRECT, gulist);
	sceGuDrawBufferList(GU_PSM_5551, frame, BUF_WIDTH);
	sceGuScissor(x1, y1, x2 + 1, y2 + 1);
	sceGuClearColor(color);
	sceGuClear(GU_COLOR_BUFFER_BIT);
	sceGuClearColor(0);
	sceGuFinish();
	sceGuSync(0, 0);
}
#endif
#if 0
//============================================================
//	psp_box fill
//============================================================
/*memo & test*/
void psp_box_fill(int x1, int y1, int x2, int y2, UINT16 color)
{
	rectangle tbounds;
	tbounds.min_x=x1;	tbounds.max_x=x2;
	tbounds.min_y=y1;	tbounds.max_y=y2;

//	psp_fill_rect( draw_frame, 0xffffff, &tbounds);
	psp_fill_rect( draw_frame,   color, &tbounds);
}
#endif
