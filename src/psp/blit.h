/* blit.h */

#define BLIT_MODE_STRETCH           0
#define BLIT_MODE_STRETCH_NO_ADJUST 1
#define BLIT_MODE_NORMAL            2

//void bitblit_psp(		 mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
//void bitblit_psp_dma(	 mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
//void bitblit_psp_mov(	 mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_256( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_288( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_320( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_320_noAdjust( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_normal( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_rotated( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );
void bitblit_psp_direct_rotated_320_240( struct mame_bitmap *bitmap, int sx, int sy, int sw, int sh );


extern int blitMode;

int getBlitMode(void);
void setBlitMode(int bmode);

//all used for stretching purposes
extern int masterVertStretch;
extern int masterHorzStretch;
extern int masterHorizStretchRemain;

extern int masterScreenOffset;

void blitPreConfigure();


//void bitblit_vga( mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy );
//void bitblit_unchained_vga( mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy );
//void bitblit_vesa( mame_bitmap *bitmap, int sx, int sy, int sw, int sh, int dx, int dy );

unsigned long blit_setup(
int dw, int dh, int sbpp,
//int dbpp,
int video_attributes,
//int xmultiply, int ymultiply,
//int xdivide, int ydivide,
//int dummy_vscanlines, int dummy_hscanlines,
int flipx, int flipy,
int swapxy,
int g_offset_in
 );


//void blit_set_buffers( int pages, int page_size );

//#define MAX_X_MULTIPLY 4
//#define MAX_Y_MULTIPLY 4

//extern UINT32 blit_lookup_low[ 65536 ];
extern UINT32 *blit_lookup_low/*[ 65536 ]*/;
//#if (0!=LINK_ROT)
//#if (1000!=LINK_ROT)
#if (5000<LINK_ROT)

/* 強制[H]16ビットモード時は含まない */

/*[mode 9360 only]*/
extern UINT32 blit_lookup_high[ 65536 ];
//#endif //(0!=LINK_ROT)
#endif //(0!=LINK_ROT)

//extern UINT32 *blit_lookup_low;
//extern UINT32 *blit_lookup_high;
