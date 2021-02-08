#ifndef DEBUGPSP_H_INCLUDED
#define DEBUGPSP_H_INCLUDED

int openLog(char* nomLog);
void logWrite(char* s1,char* s2,char* s3);
void logWriteX(char* s1,char* s2,char* s3,int threadId);
void logWriteY(char* s1,char* s2,char* s3,int threadId,int valor1,int valor2);
void closeLog(void);


#endif // DEBUGPSP_H_INCLUDED
