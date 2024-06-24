#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "state_sort.h"

#define TASK_INV -1
#define TASK_PRINT 0
#define TASK_SORT 1 
#define TASK_ADD 2

#define LINESIZE 8
/*
typedef struct {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int state;
    int code;
} line;
*/
/*
line init_line(){
    line out;
    out.year = -1;
    out.month = -1;
    out.day = -1;
    out.hour = -1;
    out.minute = -1;
    out.second = -1;
    out.state = -1;
    out.code = -1;
    return out;
}
*/

int main() {
    char* path = NULL;
    path = getpath();
    int nxt = path != NULL;
    if (nxt) {
        int task = get_task();
        if (task == TASK_PRINT) {
            printfile(path);
        }
        if (task == TASK_SORT) {
            sortfile(path);
        }/*
        if (task == TASK_ADD) {
            addline(path);
            sortfile(path);
        }*/
        nxt = (task != TASK_INV);
        free(path);
    }
    if (!nxt) {
        err();
    }
    return 0;
}

void err() {
    printf("n/a");
}
#pragma region Start
char* getpath() {
    char* str = NULL;
    char let = ' ';
    int size = 0;
    while (scanf("%c", &let) != 0 && let != '\n' && let) {
        size++;
        str = realloc(str, (1 + size) * sizeof(char));
        str[size - 1] = let;
    }
    str[size] = '\0';
    return str;
}

int get_task() {
    char taskc = '\n';
    int task = TASK_INV;
    fflush(stdin);
    scanf("%c", &taskc);
    if (taskc == '0') { task = TASK_PRINT; }
    if (taskc == '1') { task = TASK_SORT; }
    if (taskc == '2') { task = TASK_ADD; }
    return task;
}
#pragma endregion
#pragma region Read File

int* readline(FILE* from) {
    int nxt = (from != NULL);
    int* line = NULL;
    if (nxt) {
        line = malloc(sizeof(int)*LINESIZE);
        nxt = line != NULL;
    }
    if (nxt) {
        int fields = fread(line,sizeof(int),LINESIZE,from);
        nxt = (fields == LINESIZE || fields == 0);
        if (fields == 0) {
            free(line);
            line = NULL;
        }
    }
    if (!nxt) {
        err();
        if (line != NULL) {
            free(line);
            line = NULL;
        }
    }
    return line;
}

void printfile(char* path) {
    int nxt = path != NULL;
    FILE* file = NULL;
    if (nxt) {
        file = fopen(path, "rb");
        nxt = file != NULL;
    }
    if (nxt) {
        rewind(file);
        int eof = 0;
        while (!eof) {
            int* line = readline(file);
            if (line != NULL) {
                fflush(stdout);
                for (int i = 0; i < LINESIZE; i++) {
                    printf("%d ", line[i]);
                }
                printf("\n");
                free(line);
                line = NULL;
            } else { eof = 1; }
        }
        rewind(file);
        fclose(file);
        file = NULL;
    }
    if (!nxt) {
        err();
    }
}
#pragma endregion
#pragma region Sort
void sortfile(char* path) {
    int nxt = path != NULL;
    FILE* file = NULL;
    if (nxt) {
        file = fopen(path, "r+b");
        nxt = file != NULL;
    }
    if (nxt) {
        rewind(file);
    }
}

long long unsigned int getval(int* line) {
    const size_t seconds_pos = 6;
    unsigned long long int value = 0;
    for (int i = seconds_pos-1; i >= 0; i--) {
        value+=line[i]*pow(100,i);
    }
}

char* uint_to_string(unsigned int num) {
    char* str = malloc(sizeof(char)*20);
    if (str != NULL) { 
        sprintf(str, "%llu", num);
    }
    return str;
}