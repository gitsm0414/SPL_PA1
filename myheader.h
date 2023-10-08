//my header file
#ifndef MYHEADER_H
#define MYHEADER_H

#include<unistd.h>
#include<sys/types.h>
#include<errno.h>

ssize_t readBySentence(int fd, char* usrbuf, size_t n);

int len(char* ptr);

int isblank_(char* buf, int idx);

int isstart(char* buf, int idx);

int eqword(char* buf, int idx, int maxidx, char* word);

int casereturn(char* buf, int maxidx);

void cp(char* buf, int idx, int maxidx, char* word);

void preproc(char* buf, int casenum, int maxidx, char** word, int * wordn);


#endif
