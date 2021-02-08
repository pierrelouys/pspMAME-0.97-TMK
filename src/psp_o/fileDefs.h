#include <psptypes.h>
#include <pspaudio.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <pspgu.h>
#include <pspiofilemgr.h>
#include <pspkernel.h>
#include <psppower.h>
#include <psprtc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


#define O_RDONLY	PSP_O_RDONLY
#define O_WRONLY	PSP_O_WRONLY
#define O_RDWR		PSP_O_RDWR
#define O_APPEND	PSP_O_APPEND
#define O_CREAT 	PSP_O_CREAT
#define O_TRUNC 	PSP_O_TRUNC
#define O_EXCL		PSP_O_EXCL
#define O_NONBLOCK	PSP_O_NBLOCK
#define O_NOCTTY	PSP_O_NOWAIT

#undef SEEK_SET
#undef SEEK_CUR
#undef SEEK_END

#define SEEK_SET	PSP_SEEK_SET
#define SEEK_CUR	PSP_SEEK_CUR
#define SEEK_END	PSP_SEEK_END

#undef S_IFMT
#undef S_IFLNK
#undef S_IFDIR
#undef S_IFREG
#undef S_IROTH
#undef S_IWOTH
#undef S_IXOTH

#define S_IFMT		FIO_SO_IFMT
#define S_IFLNK 	FIO_SO_IFLNK
#define S_IFDIR 	FIO_SO_IFDIR
#define S_IFREG 	FIO_SO_IFREG
#define S_IROTH 	FIO_SO_IROTH
#define S_IWOTH 	FIO_SO_IWOTH
#define S_IXOTH 	FIO_SO_IXOTH

#define MAX_PATH 512

#define CHECKFILE(file)		if (file < 3) return -1

//from takka (or tnk?)
#ifndef ALIGN_N_DATA
	//#define ALIGN_N_DATA __attribute__((aligned(64)))
	#define ALIGN_N_DATA(nnn) __attribute__((aligned(nnn)))
#endif // ALIGN_N_DATA

#define PSP_ALIGN_DATA ALIGN_N_DATA(64)
