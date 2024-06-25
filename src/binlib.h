#include <stdio.h>

#pragma region Quest1
int quest_1();

void err();

char *getpath();

int validate_path(char *path);

int readtask();

int printfile(char *path);

void printline(int *line);

int* readline(char *path, size_t id);

size_t fgetbytes(char *path);

size_t fgetlines(char *path);

unsigned long long eval_line(char *path, size_t id);

int sortfile(char *path);

int qsortf(char *path, int low, int high);

int qpartf(char *path, int low, int high, int *k);

int swap_lines(char *path, size_t id1, size_t id2);

int* readline_std();

int addline(char *path, int *line);

void sort_preshuffle(char *path, int high);
#pragma endregion
#pragma region Quest_2
int quest_2();

int *getdate();
int ffind_code(char *path, int *what, size_t fields);
#pragma endregion
