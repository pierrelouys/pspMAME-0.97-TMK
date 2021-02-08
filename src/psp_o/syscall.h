/* system call prototype for PSP */

#ifndef _SYSCALL_H_INCLUDED
#define _SYSCALL_H_INCLUDED


typedef unsigned long long UINT_64;
typedef unsigned int UINT_32;
typedef unsigned short UINT_16;
typedef unsigned char UINT_8;
typedef unsigned int size_t;


/******************************************************************************/
/* sceDisplay */
//void sceDisplayWaitVblankStart(void);
//void sceDisplaySetMode(int,int,int);
//void sceDisplaySetFrameBuf(char *topaddr,int linesize,int pixelsize,int);


/******************************************************************************/
/* sceCtrl */
/* Index for the two analog directions */
#define CTRL_ANALOG_X   0
#define CTRL_ANALOG_Y   1

/* Button bit masks */
#define CTRL_SQUARE      0x8000
#define CTRL_TRIANGLE   0x1000
#define CTRL_CIRCLE      0x2000
#define CTRL_CROSS      0x4000
#define CTRL_UP         0x0010
#define CTRL_DOWN      0x0040
#define CTRL_LEFT      0x0080
#define CTRL_RIGHT      0x0020
#define CTRL_START      0x0008
#define CTRL_SELECT      0x0001
#define CTRL_LTRIGGER   0x0100
#define CTRL_RTRIGGER   0x0200
#define	CTRL_HOLD	0x00020000

/* Returned control data */
typedef struct _ctrl_data
{
	UINT_32 frame;	// マイクロ秒？
	UINT_32 buttons;	// ボタン情報
	UINT_8  analog[2];	// 0:X 1:Y
	UINT_8  dummy[6];
} ctrl_data_t;

/* Not 100% sure on this, init with 0 */
void sceCtrlInit(int unknown);
/* Pass 1 to enable analogue mode */
void sceCtrlSetAnalogMode(int on);
/* Read in the controller data. Unknown should be set to 1 */
//void sceCtrlReadBufferPositive(ctrl_data_t* paddata, int unknown);
//void sceCtrlPeekBufferPositive(ctrl_data_t* paddata, int unknown);

/******************************************************************************/
/* IoFileMgrForUser */

#define O_RDONLY    PSP_O_RDONLY//0x0001
#define O_WRONLY    PSP_O_WRONLY//0x0002
#define O_RDWR      PSP_O_RDWR//0x0003
#define O_NBLOCK    PSP_O_NBLOCK//0x0010
#define O_APPEND    PSP_O_APPEND//0x0100
#define O_CREAT     PSP_O_CREAT//0x0200
#define O_TRUNC     PSP_O_TRUNC//0x0400
#define O_NOWAIT    PSP_O_NOWAIT//0x8000

//int sceIoOpen(const char* file, int mode, int nazo);
//void sceIoClose(int fd);
//int sceIoRead(int fd, void *data, int size);
//int sceIoWrite(int fd, const void *data, int size);
/*long long sceIoLseek(int fd, long long offset, int whence);
int sceIoRemove(const char *file);
int sceIoMkdir(const char *dir, int mode);
int sceIoRmdir(const char *dir);
int sceIoRename(const char *oldname, const char *newname);*/

struct sce_timeval {
  unsigned int tv_sec;
  unsigned int tv_usec;
};

struct sce_timezone {
  int tz_minuteswest;
  int tz_dsttime;
};

unsigned long sceKernelLibcClock(void);
//unsigned long sceKernelLibcTime(unsigned long *);
//int sceKernelLibcGettimeofday(struct sce_timeval *__p, struct sce_timezone *__z);

int sceKernelAllocPartitionMemory(int, const char *, int, size_t, void *);
void *sceKernelGetBlockHeadAddr(int);
int sceKernelFreePartitionMemory(int);
size_t sceKernelMaxFreeMemSize(void);	// 最大サイズ？
size_t sceKernelTotalFreeMemSize(void); // 合計サイズ？

enum {
    TYPE_DIR=0x10,
    TYPE_FILE=0x20
};

struct dirent_tm {
	UINT_16 year;
	UINT_16 mon;
	UINT_16 mday;
	UINT_16 hour;
	UINT_16 min;
	UINT_16 sec;
	UINT_32 msec;
};

struct dirent {
	int unk0;
	UINT_32 type;
	long long size;
	struct dirent_tm ctime;	//作成日時
	struct dirent_tm atime;	//最終アクセス日時
	struct dirent_tm mtime;	//最終更新日時
	UINT_32 unk[6];		//常にゼロ？
	char name[256];
	int dummy;
	int dummy2;
};

typedef struct dir_t
{
	int handle;
	struct dirent ent;
} DIR;

int sceIoDopen(const char *fn);
//int sceIoDread(int fd, struct dirent *de);
//void sceIoDclose(int fd);


//void sceAudioOutputBlocking(void); //
int  sceAudioOutputPanned(int hdl, int vL, int vR, void *); //
int  sceAudioOutputPannedBlocking(int hdl, int vL, int vR, void *); //
int  sceAudioChReserve(int, int samplecount, int mode); //init buffer? returns handle, minus if error
//void sceAudioChRelease(int handle); //free buffer?
int  sceAudioGetChannelRestLen(int handle); // 未検証
int  sceAudioSetChannelDataLen(int handle, int samplecount); // 未検証
int  sceAudioChangeChannelConfig(int handle, int mode); // 0:streo 0x10:mono
int  sceAudioChangeChannelVolume(int hdl, int vL, int vR); // 未検証


void sceKernelExitGame(void);

#define POWER_CB_POWER		0x80000000
#define POWER_CB_HOLDON		0x40000000
#define POWER_CB_STANDBY	0x00080000
#define POWER_CB_RESCOMP	0x00040000
#define POWER_CB_RESUME		0x00020000
#define POWER_CB_SUSPEND	0x00010000
#define POWER_CB_EXT		0x00001000
#define POWER_CB_BATLOW		0x00000100
#define POWER_CB_BATTERY	0x00000080
#define POWER_CB_BATTPOWER	0x0000007F

typedef int (*pg_threadfunc_t)(int args, void *argp);
//int sceKernelCreateThread(const char *name, pg_threadfunc_t func, unsigned int priority, unsigned int stacksize, unsigned int attrib, void *unk);
//int sceKernelStartThread(int hthread, int arg0, void *arg1);
//void sceKernelExitThread(int ret);
//int sceKernelWaitThreadEnd(int hthread, void *unk);
int sceKernelDeleteThread(int hthread);
int sceKernelDelayThread(UINT_32 usec);

void sceDmacMemcpy(void *dst, const void *src, int size);

char *getCurDir(void);//repuken2
void setCurDir(const char *path);//repuken2


#endif // _SYSCALL_H_INCLUDED
