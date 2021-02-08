//
// PSP malloc for VRAM
//
#include <sys/types.h>
//#include <malloc.h>
#include "pg.h"

#define VRAMTOP		(0x04000000 +0x40000000)	// +0x40000000 Non Chache?
#define VRAMSIZE	((1024*1024*2))

#define MEM_USED	0x01010101
#define MEM_FREE	0x0F0F0F0F

#define ISMEMUSED(x)	(MEM_USED ==x->is_available)
#define ISMEMFREE(x)	(MEM_FREE ==x->is_available)
#define ISUSERMEM(x)	((int)x >=VRAMTOP && (int)x <(VRAMTOP +VRAMSIZE))

static int has_initialized = 0;

struct mem_control_block {
   int is_available;
   int size;
};

#define MCBSZ	sizeof(struct mem_control_block)

static char	*alloc_buffer;
static char	*heap_end;
static int	total =0;

caddr_t _sbrk_vram(int incr);
caddr_t sbrk_vram(int incr);
void malloc_vram_init();
void malloc_vram_term();
void free_vram(void *firstbyte);
void *malloc_vram(size_t numbytes);
void *realloc_vram(void *ptr, size_t size);
void *calloc_vram(size_t __nmemb, size_t __size);
int  is_vram_addr(char *ptr);
int getmemsize_vram(void *ptr);

caddr_t _sbrk_vram (int incr)
{
char	*prev_heap_end;

	prev_heap_end = heap_end;
	if ((heap_end +incr) >= (VRAMTOP +VRAMSIZE))
	{
		printf_("_sbrk_vram:Memory Over Fllow!!");
//		exit(-1);
		return 0;
	}

	heap_end += incr;
	total += incr;

	return (caddr_t) prev_heap_end;
}

caddr_t sbrk_vram (int incr) { return _sbrk_vram(incr); }


void malloc_vram_init()
{
	heap_end =VRAMTOP +(FRAMESIZE *2);
	total =0;

	has_initialized = 1;
}

void malloc_vram_term()
{
	has_initialized = 0;
}

void free_vram(void *firstbyte)
{
struct	mem_control_block *mcb, *nmcb;
void	*current_location, *next_location;

void	*last_valid_address =sbrk_vram(0);

	current_location =firstbyte -MCBSZ;
	if (!ISUSERMEM(current_location) || 0 !=((int)current_location % 4)
	  || current_location >=last_valid_address)	// 無効なアドレス
		return;

	mcb =(struct mem_control_block *)current_location;
	if (ISMEMUSED(mcb)) {
		// メモリ開放
		mcb->is_available =MEM_FREE;
		next_location =current_location +mcb->size;
		if (last_valid_address ==next_location) // 最終ブロックならsbrk開放
			sbrk_vram(-mcb->size);
		else {
			// 次ブロックがフリーならマージする
			nmcb =(struct mem_control_block *)next_location;
			while(next_location <last_valid_address && ISMEMFREE(nmcb))
			{
				mcb->size +=nmcb->size;
				next_location +=nmcb->size;
				nmcb =(struct mem_control_block *)next_location;
			}
		}
	}

	return;
}


void *malloc_vram(size_t numbytes)
{
static void	*managed_memory_start;
void	*current_location, *save_location;
struct	mem_control_block *current_location_mcb;
void	*memory_location;
int	minsize =-1;
void	*last_valid_address;

	if(! has_initialized) {
		malloc_vram_init();
		managed_memory_start =sbrk_vram(0);
	}

	if (0 ==numbytes) return 0;

	numbytes =((numbytes+3)&~0x3) +MCBSZ;

	memory_location =save_location =0;
	current_location =managed_memory_start;
	last_valid_address =sbrk_vram(0);
	while(current_location <last_valid_address)
	{
		current_location_mcb =(struct mem_control_block *)current_location;

		if(ISMEMFREE(current_location_mcb))
		{
			if(current_location_mcb->size >=numbytes)
			{
				if (-1 ==minsize || minsize >(current_location_mcb->size -numbytes))
				{
					minsize =current_location_mcb->size -numbytes;
					save_location =current_location;

					if (current_location_mcb->size ==numbytes)
						break;
				}
			}
		}

		current_location +=current_location_mcb->size;
	}

	if (save_location)
	{
		current_location_mcb =(struct mem_control_block *)save_location;
		current_location_mcb->is_available =MEM_USED;
		memory_location =save_location +MCBSZ;
		// 余裕があれば分割する
		if (minsize >(MCBSZ*2)) {
			current_location_mcb->size =numbytes;
			void *next_location =save_location +numbytes;
			struct mem_control_block *next_location_mcb =(struct mem_control_block *)next_location;
			next_location_mcb->is_available =MEM_USED;
			next_location_mcb->size =minsize;
			free(next_location +MCBSZ);
		}

	} else {
		if (0 !=(memory_location =sbrk_vram(numbytes))) {
			current_location_mcb =memory_location;
			current_location_mcb->is_available =MEM_USED;
			current_location_mcb->size =numbytes;

			memory_location +=MCBSZ;
		}
	}

	return memory_location;
}


void *realloc_vram(void *ptr, size_t size)
{
char	*newptr;
int	copysize;

	if (0 ==ptr)
		return malloc_vram(size);

	if (0 ==size) {
		free_vram(ptr);
		return 0;
	}

	if (0 ==(newptr =(char *)malloc_vram(size)))
		return 0;

	if (0 !=(copysize =getmemsize_vram(ptr))) {
		copysize =(copysize <size? copysize: size);
		memcpy(newptr, ptr, copysize);
	}

	free_vram(ptr);

	return newptr;
}

void * calloc_vram(size_t __nmemb, size_t __size)
{
char *ptr;
size_t size =__nmemb *__size;

	if (0 ==(ptr =(char *)malloc_vram(size)))
		return 0;

	memset(ptr, 0x00,  size);

	return ptr;
}

int  is_vram_addr(char *ptr)
{
	if (ptr >=(VRAMTOP +(FRAMESIZE *2)) && ptr <(VRAMTOP +VRAMSIZE))
		return(1);
	else
		return(0);
}

int getmemsize_vram(void *ptr)
{
struct	mem_control_block *mcb;
void	*last_valid_address =sbrk_vram(0);

	mcb =(struct mem_control_block *)(ptr -MCBSZ);
	if (ISUSERMEM(mcb) && 0 ==((int)mcb % 4) && (void *)mcb <last_valid_address) {	// 有効なアドレス
		if (ISMEMUSED(mcb)) {
			return(mcb->size -MCBSZ);
		}
	}

	return(0);
}
