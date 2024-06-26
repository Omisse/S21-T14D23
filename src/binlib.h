#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define prefix "../"

#define TASK_INV -1
#define TASK_PRINT 0
#define TASK_SORT 1 
#define TASK_ADD 2
#define LINESIZE 8

#define _strb 256

#pragma region Quest1
int do_job(int type);

int quest_1();

int quest_2();

int quest_3();

void err();

char *getpath();

int validate_path(const char* path);

int readtask();

int printfile(const char* path);

void printline(int *line);

int* readline(const char* path, size_t id);

size_t fgetbytes(const char* path);

size_t fgetlines(const char* path);

unsigned long long eval_line(const char* path, size_t id);

int sortfile(const char* path);

int qsortf(const char* path, int low, int high);

int qpartf(const char* path, int low, int high, int *k);

int swap_lines(const char* path, size_t id1, size_t id2);

int* readline_std();

int addline(const char* path, const int *line);

void sort_preshuffle(const char* path, int high);
#pragma endregion
#pragma region Quest_2
int quest_2();

int* get_date();

int ffind_code(const char* path, const int *what, size_t fields);
#pragma endregion
#pragma region Quest_3
int quest3();

long long int* ffind_range(const char * path, const int * low3, const int * high3);

int rewrite_exclude(const char * path, const long long int * id_exclude);

int is_in_ids(long long int id, const long long int * ids);

void cleari();

unsigned long long eval_date(const int * date);
#pragma endregion
