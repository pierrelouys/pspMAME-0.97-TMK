//============================================================
//
//	psp_file.c - PSP low level fileio code
//
//============================================================

#include "driver.h"//#include "psp_main.h"
#include "psp_file.h"
#include <errno.h>


#define CHECKFILE(file)		if (file < 3) return -1



//============================================================
//	PRIVATE VARIABLES
//============================================================

static char curdir[MAX_PATH];
static int  curdir_length;


#if 1
//============================================================
//	setCurDir
//============================================================

void setCurDir(const char *path)
{
	char *p;

	memset(curdir, 0, MAX_PATH);
	strncpy(curdir, path, MAX_PATH - 1);

	p = strrchr(curdir, '/');
	if (p != NULL) *(p + 1) = '\0';

	curdir_length = strlen(curdir);
}
#endif

#if 1
//============================================================
//	getCurDir
//============================================================

char *getCurDir(void)
{
	return curdir;
}
#endif

//============================================================
//	seterrno (for override error code)
//============================================================

INLINE int seterrno(int ret)
{
	if (ret >= 0) return ret;
	errno = ret;
	return -1;
}


#if 1
//============================================================
//	convert_path
//============================================================

static char *convert_path(char *buf, const char *path)
{
	if (strchr(path, ':')) return (char *)path;

	memcpy(buf, curdir, curdir_length);
	strcpy(buf + curdir_length, path);

	return buf;
}
#endif

#if 0
//============================================================
//	psp_open
//============================================================

int psp_open(const char *path, int flags)
{
	char fullpath[MAX_PATH];
	char *p = convert_path(fullpath, path);

	return seterrno(sceIoOpen(p, flags, 0777));
}
#endif

#if 0
//============================================================
//	psp_close
//============================================================

int psp_close(int file)
{
	CHECKFILE(file);
	sceIoClose(file);
	return 0;
}
#endif

#if 0
//============================================================
//	psp_read
//============================================================

int psp_read(int file, void *ptr, int len)
{
	CHECKFILE(file);
	return seterrno(sceIoRead(file, ptr, len));
}
#endif

#if 0
//============================================================
//	psp_write
//============================================================

int psp_write(int file, const void *ptr, int len)
{
	switch (file)
	{
	case 0: /* stdin */
		return -1;
	case 1: /* stdout */
	case 2: /* stderr */
		{
			char buf[2048];
			memset(buf, 0, 2048);
			strncpy(buf, ptr, len);
			/*return*/ psp_printf_bbb(buf);
		}
	default:
		return seterrno(sceIoWrite(file, ptr, len));
	}
}
#endif

#if 0
//============================================================
//	psp_lseek
//============================================================

int psp_lseek(int file, int ptr, int dir)
{
	CHECKFILE(file);
	return seterrno(sceIoLseek(file, ptr, dir));
}
#endif

#if 1
//============================================================
//	psp_fstat
//============================================================

int psp_fstat(int file, struct stat *st)
{
	int size, cur;

	CHECKFILE(file);

	cur = sceIoLseek(file, 0, SEEK_CUR);
	size = sceIoLseek(file, 0, SEEK_END);
	sceIoLseek(file, cur, SEEK_SET);

	memset(st, 0, sizeof(*st));

	st->st_mode = S_IFREG;
	st->st_size = size;

	return 0;
}
#endif

#if 0
//============================================================
//	psp_create
//============================================================

int psp_creat(const char *path, int mode)
{
	return psp_open(path, O_WRONLY|O_TRUNC|O_CREAT);
}
#endif

#if 0
//============================================================
//	psp_rename
//============================================================

int psp_rename(const char *oldpath, const char *newpath)
{
	char fulloldpath[MAX_PATH];
	char fullnewpath[MAX_PATH];

	return seterrno(sceIoRename(convert_path(fulloldpath, oldpath), convert_path(fullnewpath, newpath)));
}
#endif

#if 0
//============================================================
//	psp_unlink
//============================================================

int psp_unlink(const char *path)
{
	char fullpath[MAX_PATH];

	return seterrno(sceIoRemove(convert_path(fullpath, path)));
}
#endif

#if 0
//============================================================
//	psp_chdir
//============================================================

int psp_chdir(const char *path)
{
	if (strchr(path,':'))
		strcpy(curdir,path);
	else
		return -1;

	curdir_length = strlen(curdir);
	curdir[curdir_length++] = '/';
	curdir[curdir_length] = '\0';

	return 0;
}
#endif

#if 0
//============================================================
//	psp_mkdir
//============================================================

int psp_mkdir(const char *path, mode_t mode)
{
	char fullpath[MAX_PATH];

	return seterrno(sceIoMkdir(convert_path(fullpath, path), mode));
}
#endif

#if 0
//============================================================
//	psp_rmdir
//============================================================

int psp_rmdir(const char *path)
{
	char fullpath[MAX_PATH];

	return seterrno(sceIoRmdir(convert_path(fullpath, path)));
}
#endif

#if 1
//============================================================
//	psp_stat
//============================================================

int psp_stat(const char *path, struct stat *sbuf)
{
	char fullpath[MAX_PATH];
	char *p = convert_path(fullpath, path);

	int fhandle =seterrno(sceIoOpen(p, PSP_O_RDONLY, 0777));
	if (fhandle >= 0)
	{
		psp_fstat(fhandle, sbuf);
		sceIoClose(fhandle);
		return 0;
	}

	int handle = seterrno(sceIoDopen(p));
	if (handle >= 0)
	{
		sceIoDclose(handle);
		memset(sbuf, 0, sizeof(*sbuf));
		sbuf->st_mode = _IFDIR;
		return 0;
	}

	return -1;
}
#endif

