//============================================================
//
//	psp_file.c - PSP low level fileio code
//
//============================================================

#ifndef PSP_FILE_H
#define PSP_FILE_H

#include <sys/stat.h>

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


void setCurDir(const char *path);
char *getCurDir(void);

int psp_open(const char *path, int flags);
int psp_close(int file);
int psp_read(int file, void *ptr, int len);
int psp_write(int file, const void *ptr, int len);
int psp_lseek(int file, int ptr, int dir);
int psp_fstat(int file, struct stat *st);
int psp_creat(const char *path, int mode);
int psp_rename(const char *oldpath, const char *newpath);
int psp_unlink(const char *path);

int psp_chdir(const char *path);
int psp_mkdir(const char *path, mode_t mode);
int psp_rmdir(const char *path);

int psp_stat(const char *path, struct stat *sbuf);

#endif /* PSP_FILE_H */
