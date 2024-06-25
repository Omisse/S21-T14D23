#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "binlib.h"

#pragma region quest1

#define TASK_INV -1
#define TASK_PRINT 0
#define TASK_SORT 1 
#define TASK_ADD 2

#define LINESIZE 8

int quest_1() {
    char* path = NULL;
    path = getpath();
    int nxt = path != NULL;
    if (nxt) {
        nxt = validate_path(path);
    }
    if (nxt) {
        int task = readtask();
        nxt = task != TASK_INV;
        if (nxt){
            if (task == TASK_PRINT) {
                nxt = printfile(path);
            }
            if (task == TASK_SORT) {
                nxt = sortfile(path);
                if (nxt) {
                    nxt = printfile(path);
                }
            } 
            if (task == TASK_ADD) {
                int* line = readline_std();
                nxt = line != NULL;
                if(nxt) {
                    printf("line+\n");
                    nxt = addline(path,line);
                    free(line);
                }
                if(nxt) {
                    printf("add+\n");
                    nxt = sortfile(path);
                }
                if(nxt) {
                    printf("sort+\n");
                    nxt = printfile(path);
                }
            }
        } 
    }
    if (!nxt) {
        err();
    }
    return 0;
}

void err() {
    fflush(stdout);
    printf("n/a");
}

char* getpath() {
    char* str = NULL;
    char let = ' ';
    int length = 0;
    fflush(stdin);
    while (scanf("%c", &let) != 0 && let != '\n' && let) {
        length++;
        str = realloc(str, sizeof(char*)+(length) * sizeof(char));
        str[length-1] = let;
    }
    if (length <= 0) {
        if (str) {
            free(str);
            str = NULL;
        }
    } else {
        str[length] = '\0';
    }
    return str;
}

int validate_path(char* path) {
    FILE* file = NULL;
    file = fopen(path, "r+b");
    int nxt = file != NULL;
    fclose(file);
    file = NULL;
    return nxt;
}

int readtask() {
    int task = TASK_INV;
    fflush(stdin);
    int res = scanf("%d", &task);
    if (!res) {
        task = TASK_INV;
    }
    if (task < TASK_PRINT && task > TASK_ADD) {
        task = TASK_INV;
    }
    return task;
}

int printfile(char* path) {
    size_t length = fgetlines(path);
    int nxt = 1;
    for (size_t i = 0; i < length && nxt; i++) {
        printf("%lld: ", i);
        int* newline = readline(path, i);
        nxt = newline != NULL;
        if (nxt) {
            printline(newline);
            free(newline);
        }
    }
    return nxt;
}

void printline(int* line) {
    fflush(stdout);
    for (int i = 0; i < LINESIZE; i++) {
        printf("%u",line[i]);
        if (i!=7) {
            printf(" ");
        }
    }
    printf("\n");
}

int* readline(char* path, size_t id) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_SET);
    int offset = id*sizeof(int)*LINESIZE;
    int* newline = malloc(sizeof(int*)+sizeof(int)*8);
    int nxt = newline != NULL && 0 == fseek(file, offset, SEEK_SET);
    if (nxt) {
        fread(newline, sizeof(int), LINESIZE, file);
        rewind(file);
    }
    fclose(file);
    return newline;
}

int writeline(char* path, int* line, size_t id) {
    FILE* file = fopen(path, "r+b");
    int offset = id*sizeof(int)*LINESIZE;
    int nxt = fseek(file, offset, SEEK_SET) == 0;
    if (nxt) {
        fwrite(line, sizeof(int), LINESIZE, file);
        fflush(file);
        rewind(file);
    }
    fclose(file);
    return nxt;
}


size_t fgetbytes(char* path) {
    FILE* file = fopen(path, "rb");
    rewind(file);
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    fclose(file);
    return size;
}

size_t fgetlines(char* path) {
    return fgetbytes(path)/(sizeof(int)*LINESIZE);
}

unsigned long long eval_line(char* path, size_t id) {
    int* line = readline(path, id);
    unsigned long long val = 0;
    int nxt = line != NULL;
    if (nxt) {
        for (int i = 5; i >= 0; i--) {
            unsigned long long number = (unsigned long long) line[i];
            for (int j = 0; j < 5-i; j++) {
                number*=100;
            }
            val = val + number;
        }
        free(line);
    }
    return val;
}

int sortfile(char* path) {
    sort_preshuffle(path, fgetlines(path)-1);
    return qsortf(path, 0, fgetlines(path)-1);
}

int qsortf(char* path, int low, int high) {
    int nxt = 1;
    if (low < high) {
        int pid;
        nxt = qpartf(path, low, high, &pid);
        if(nxt) {
            qsortf(path, low, pid-1);
            qsortf(path, pid+1, high);
        }
    }
    return nxt;
}

int qpartf(char* path, int low, int high, int* k) {
    unsigned long long pval = eval_line(path, high);
    //printf("qpartf.pval: %llu\n", pval);
    *k = low-1;
    int nxt = 1;
    for (int i = low; (i <= high-1) && nxt; i++) {
        unsigned long long eval = eval_line(path, i);
        //printf("qpartf.eval: %lli | line: %i | pivot: %i\n", eval,i,*k);
        if (eval < pval) {
            (*k)++;
            nxt = swap_lines(path, *k, i);
        }
    }
    if (nxt) {
        nxt = swap_lines(path,(*k)+1, high);
        (*k)++;
    }
    return nxt;
}

int swap_lines(char* path, size_t id1, size_t id2) {
    int skip = id1 == id2;
    int nxt = skip;
    if (!skip) {
        int* line1 = readline(path, id1);
        int* line2 = readline(path, id2);
        nxt = line1 != NULL && line2 != NULL;
        if (nxt) {
            writeline(path, line1, id2);
            writeline(path, line2, id1);
            //fflush(stdout);
            //printf("Swapping lines %lld and %lld\n", id1, id2);
        } else {
            if (line1) {
                free(line1);
            }
            if (line2) {
                free(line2);
            }
        }
    }
    return nxt;
}

int* readline_std() {
    int* newline = malloc(sizeof(int*)+sizeof(int)*8);
    int nxt = newline != NULL;
    if (nxt) {
        fflush(stdin);
        int field = 0;
        while (field != 8 && nxt) {
            char separator = '\n';
            int fres = scanf("%d", &(newline[field]));
           // printf("readline_std.newline[field]: %d [%d]\n", newline[field], field);
            int sepres = scanf("%c", &separator);
            nxt = fres*sepres;
            int bool1 = separator == ' ' && field != 7;
            int bool2 = (separator == '\n' || separator == '\0')
                        && field == 7;
            nxt = bool1 || bool2;
            if(nxt) { field++; }
        }
        if (!nxt) {
            free(newline);
            newline = NULL;
        }
    }
    return newline;
}

int addline(char* path, int* line) {
    size_t id = fgetlines(path);
    return writeline(path, line, id);
}

void sort_preshuffle(char* path, int high) {
    srand(time(NULL));
    int nxt = 1;
    for (int i = high; i > 0 && nxt; i--) {
        int j = (rand()%i+1);
        nxt = swap_lines(path, i, j);
    }
}

#undef TASK_INV
#undef TASK_PRINT
#undef TASK_SORT
#undef TASK_ADD


#pragma endregion

#pragma region quest2
#define _strb 256

int quest_2() {
    char* path = NULL;
    path = getpath();
    int nxt = (path != NULL);
    if (nxt) {
        nxt = validate_path(path);
    }
    if (nxt) {
        int* date = getdate();
        int code = -1;
        size_t fields = 3;
        nxt = date != NULL;
        if (nxt) {
            //printf("%d.%d.%d\n", date[0], date[1], date[2]);
            code = ffind_code(path, date, fields);
            nxt = (code != -1);
        }
        if (nxt) {
            nxt = code;
        }
        if (date) { free(date); }
    }
    if(nxt) { printf("%d", nxt); } 
    else { err(); }
    if(path != NULL) {
        free(path);
    }
    return 0;
}

int* getdate() {
    char* buffer = malloc(sizeof(char*)+sizeof(char)*_strb);
    int* date = malloc(sizeof(int*)+3*sizeof(int));
    int nxt = buffer != NULL;
    fflush(stdout);
    if (nxt) {
        fflush(stdin);
        buffer = fgets(buffer, _strb, stdin);
        nxt = (buffer != NULL);
    }
    if(nxt) {
        char* ptr = buffer;
        int dotcount = 0;
        while(*ptr && nxt && *ptr != '\n') {
            nxt = *ptr == '.' || (*ptr >= '0' && *ptr <='9');
            if (*ptr == '.') {
                dotcount++;
            }
            ptr++;
        }
        if (nxt) {
            nxt = dotcount == 2;
        }
    }
    if(nxt) {
        char* str = NULL;
        int field = -1;
        int inc = 0;
        do {
            if (inc == 0) {
                str = strtok(buffer, ".");
                inc++;
            } else {
                str = strtok(NULL, ".");
            }
            if (str != NULL) {
                //printf("getdate_strtok: %s\n", str);
                field++;
            }
            nxt = field < 3;
            if (nxt && str != NULL) {
                date[field] = atoi(str);
            }
        } while(nxt && str != NULL);
        nxt = field == 2;
    }
    if (nxt) {
        int tmpVal = date[0];
        date[0] = date[2];
        date[2] = tmpVal;
        nxt = date[1] <= 12 && date[2] <= 31;
    }
    if (buffer) {
        free(buffer);
    }
    if (!nxt) {
        if (date) {
            free(date);
            date = NULL;
        }
    }
    return date;
}

int ffind_code(char* path, int* what, size_t fields) {
    const int code_id = 7;
    int found = 0;
    int code = -1;
    size_t id = 0;
    size_t fsize = fgetlines(path);
    int nxt = fsize != 0;
    while(!found && nxt && id < fsize) {
        int* line = readline(path, id);
        size_t count = 0;
        nxt = (line != NULL);
        if (nxt) {
            for (size_t i = 0; i < fields; i++) {
                if (what[i] == line[i]) {
                    count++;
                }
            }
            found = count == fields;
        }
        if (!found) { id++; }
        if (found) {code = line[code_id]; }
        if (line != NULL) { free(line); line = NULL; }
    }
    if (!found) {
        code = -1;
    }
    return code;
}

#undef _strb
#pragma endregion