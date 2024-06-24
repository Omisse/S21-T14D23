#ifndef BINMASTER
#define BINMASTER
#ifndef _INC_STDIO
#include <stdio.h>
#endif
#ifndef V
#define V
#endif
void err();

char *getpath();

int get_task();

int *readline(FILE *from);

void printfile(char *path);
#endif