#ifndef UTILS_H
#define UTILS_H

#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "ctype.h"
#include "unistd.h"

enum PipeEnd {
    READ_END,
    WRITE_END
};

int IsPrime(int number);

void TrimString(char *str);

int CreateProcess();

int OpenFile(char *filename);

void CreatePipe(int pipeFd[2]);

char *ReadString(FILE *stream);

#endif
