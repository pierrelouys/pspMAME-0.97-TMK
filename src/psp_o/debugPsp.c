#include "debugPsp.h"
#include "fileDefs.h"
#include "osdepend.h"
//stdio version:
/*static FILE* f;

void openLog(char* nomLog)
{
f = open(nomLog,"w+");

if(f == NULL);//error
fputs("isaaaaaaaaaaaaaaaaa!\n",f);
}

void logWrite(char* s1,char* s2)
{
fputs(s1,f);
fputs(s2,f);
fputs("\n",f);
}


void closeLog(void)
{
close(log);
}*/

//psp sce version:
#define HANDLE int;

static int fHandle;
static char internalNameCache[512];
static long logCounter;

int openLog(char* nomLog)
{
//fHandle = open(nomLog,"w+");
int thisTID = sceKernelGetThreadId();//unknown uid 100f
char sTID[20];

int fHandle_test;
osd_file *osTest;
int fdOsd;
/*int 	sceKernelGetUserLevel (void)
 	Get the user level of the current thread.
*/

logCounter = 0;

sprintf(sTID,"TID==%d",thisTID);

fHandle = sceIoOpen(nomLog, PSP_O_WRONLY|PSP_O_CREAT, 0777);
strcpy(internalNameCache,nomLog);

if(!fHandle)
 return 0;//error
else {

fHandle_test = sceIoOpen("roms/gberet.zip", PSP_O_RDONLY, 0777);//ms0:/PSP/GAME/mameGBeret/
logWriteY("...TEST...","abriendo gberet.zip , ","val01=fd ",666,fHandle_test,0);
close(fHandle_test);

//osTest = osd_fopen(1, 0, "roms/gberet.zip", "r");
fdOsd = 0;//osTest->handle;

//if(osTest->handler < 0)
  //logWriteY("...TEST OSD...","abriendo gberet.zip pos OSD, -OK-","val01=fd ",667,0,0);
/*else
  logWriteY("...TEST OSD...","abriendo gberet.zip pos OSD, -CAGO- ","val01=fd ",667,fdOsd,0);*/

//osd_fclose(osTest);

   closeLog();
   sceIoRemove(internalNameCache);
return 1;
}


fHandle = sceIoOpen(nomLog, PSP_O_WRONLY|PSP_O_CREAT, 0777);
logWrite(sTID,"","");
logWrite("isaaaaaaaaaaaaaaaaa!","","");
closeLog();
return 1;
}

void logWrite(char* s1,char* s2,char* s3)
{
char linea[1024];
sprintf(linea,"%d>> %s%s%s\r\n",logCounter,s1,s2,s3);

fHandle = sceIoOpen(internalNameCache, PSP_O_WRONLY|PSP_O_APPEND, 0777);
sceIoWrite(fHandle,linea, strlen(linea) );
closeLog();

logCounter++;
}

void logWriteX(char* s1,char* s2,char* s3,int threadId)
{
char linea[1024];
sprintf(linea,"%d>> [TID:'%d']%s%s%s\r\n",logCounter,threadId,s1,s2,s3);

fHandle = sceIoOpen(internalNameCache, PSP_O_WRONLY|PSP_O_APPEND, 0777);
sceIoWrite(fHandle,linea, strlen(linea) );
closeLog();
logCounter++;
}


void logWriteY(char* s1,char* s2,char* s3,int threadId,int valor1,int valor2)
{
char linea[1024];
sprintf(linea,"%d>> [TID:'%d']%s%s%s,valor1=='%d',valor2=='%d'\r\n",logCounter,threadId,s1,s2,s3,valor1,valor2);

fHandle = sceIoOpen(internalNameCache, PSP_O_WRONLY|PSP_O_APPEND, 0777);
sceIoWrite(fHandle,linea, strlen(linea) );
closeLog();
logCounter++;
}

#define SHOW_FD_CLOSE 0
void closeLog(void)
{
//sceIoClose(fHandle);
char dbg[512];

#if (SHOW_FD_CLOSE==1)
if(fHandle >= 0){
  sprintf(dbg,"cerrando log, fd fHandle SceUid=='%d'\r\n",fHandle);
  sceIoWrite(fHandle,dbg, strlen(dbg) );
}
#endif
close(fHandle);
}

