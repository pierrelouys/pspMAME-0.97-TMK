#include "syscall.h"

#define HANDLE_MAX	1536
#define VRAM_MALLOC	1

typedef struct _MHDL {
	int Hdl;
	size_t Size;
	void *Ptr;
} mhandle;
static mhandle mHdl[HANDLE_MAX];

void * malloc(size_t __size);
void * calloc(size_t __nmemb, size_t __size);
void * realloc(void *__r, size_t __size);
void free(void *__r);
static void p_init(void);
static int getFreeHandle(void);
static int PtrToHandle(void *pPtr);

static int mInit =0;

#ifdef VRAM_MALLOC
void malloc_vram_term();
void free_vram(void *firstbyte);
void *malloc_vram(size_t numbytes);
void *realloc_vram(void *ptr, size_t size);
void *calloc_vram(size_t __nmemb, size_t __size);
int  is_vram_addr(void *ptr);
#endif

void * malloc(size_t __size)
{
int hIdx, hBlk;
void *ptr;

	p_init();

	if (0 ==__size) return 0;

	if (-1 ==(hIdx =getFreeHandle()))
		return 0;

	if (0 >=(hBlk =sceKernelAllocPartitionMemory(2, "UserBlock", 0, __size, 0)))
	{
#ifdef VRAM_MALLOC
		return(malloc_vram(__size));
#endif
		return 0;
	}
	if (0 ==(ptr =sceKernelGetBlockHeadAddr(hBlk)))
		return 0;

	mHdl[hIdx].Hdl =hBlk;
	mHdl[hIdx].Size =__size;
	mHdl[hIdx].Ptr =ptr;

	return(ptr);
}

void * calloc(size_t __nmemb, size_t __size)
{
char *ptr;
size_t size =__nmemb *__size;

	if (0 ==(ptr =(char *)malloc(size)))
		return 0;

	memset(ptr, 0x00, size);

	return ptr;
}

// 再割り当て (および移動) された メモリ ブロックへの void ポインタを返します。
//サイズを 0 にし、バッファ引数を NULL 以外にした場合、
//またはメモリ不足のためブロックを指定されたサイズに拡張できない場合は、戻り値は NULL になります。
//前者の場合は元のブロックを解放しますが、後者の場合は元のブロックを変更しません。

void * realloc(void *__r, size_t __size)
{
char *ptr, *optr;
int hIdx, hNIdx;
size_t size;

	if (0 ==__size) {
		free(__r);
		return 0;
	}

#ifdef VRAM_MALLOC
	if (is_vram_addr(__r))
		return(realloc_vram(__r, __size));
#endif

	if (0 ==(ptr =(char *)malloc(__size)))
		return 0;

	if (0 <=(hIdx =PtrToHandle(__r))) {
		size =(mHdl[hIdx].Size <__size)?mHdl[hIdx].Size:__size;
		memcpy(ptr, __r, size);
	}

	free(__r);

	return(ptr);
}

void free(void *__r)
{
int hIdx;

	if (0 ==__r)
		return;

#ifdef VRAM_MALLOC
	if (is_vram_addr(__r)) {
		free_vram(__r);
		return;
	}
#endif

	if (0 <=(hIdx =PtrToHandle(__r))) {
		sceKernelFreePartitionMemory(mHdl[hIdx].Hdl);

		mHdl[hIdx].Hdl =0;
		mHdl[hIdx].Size =0;
		mHdl[hIdx].Ptr =0;
	}
}

static void p_init(void)
{
	if (1 ==mInit) return;

	for(int i=0; i <HANDLE_MAX; i++) {
		mHdl[i].Hdl =0;
		mHdl[i].Size =0;
		mHdl[i].Ptr =0;
	}

	mInit =1;
}

void malloc_term()
{
#ifdef VRAM_MALLOC
	malloc_vram_term();
#endif

	for(int i=HANDLE_MAX-1; i >=0; i--)
		if (mHdl[i].Hdl)
			free(mHdl[i].Ptr);
}

static int getFreeHandle(void)
{
	for(int i=0; i <HANDLE_MAX; i++)
		if (0 ==mHdl[i].Hdl)
			return i;

	printf_("malloc:getFreeHandle HANDLE_MAX over!!");

	return -1;
}

static int PtrToHandle(void *pPtr)
{
	for(int i=0; i <HANDLE_MAX; i++)
		if (pPtr ==mHdl[i].Ptr && 0 !=mHdl[i].Hdl)
			return i;

	return -1;
}
