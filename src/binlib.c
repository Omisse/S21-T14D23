#include "binlib.h"

#pragma region pseudomain

int do_job(int type) {
    int retval = -1;
    if (type == 1) {
        retval = quest_1();
    }
    if (type == 2) {
        retval = quest_2();
    }
    if (type == 3) {
        retval = quest_3();
    }
    return retval;
}

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
        if (nxt) {
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
                if (nxt) {
                    // printf("line+\n");
                    nxt = addline(path, line);
                    free(line);
                }
                if (nxt) {
                    // printf("add+\n");
                    nxt = sortfile(path);
                }
                if (nxt) {
                    // printf("sort+\n");
                    nxt = printfile(path);
                }
            }
        }
    }
    if (!nxt) {
        err();
    }
    if (path) {
        free(path);
    }
    return 0;
}

int quest_2() {
    char* path = NULL;
    path = getpath();
    int nxt = (path != NULL);
    if (nxt) {
        nxt = validate_path(path);
    }
    if (nxt) {
        int* date = get_date();
        int code = -1;
        nxt = date != NULL;
        if (nxt) {
            // printf("%d.%d.%d\n", date[0], date[1], date[2]);
            size_t fields = 3;
            code = ffind_code(path, date, fields);
            nxt = (code != -1);
        }
        if (nxt) {
            nxt = code;
        }
        if (date) {
            free(date);
        }
    }
    if (nxt) {
        printf("%d", nxt);
    } else {
        err();
    }
    if (path != NULL) {
        free(path);
    }
    return 0;
}

int quest_3() {
    char* path = getpath();
    int nxt = path != NULL;
    int* date1 = NULL;
    int* date2 = NULL;
    long long int* found_ids = NULL;
    if (nxt) {
        nxt = validate_path(path);
    }
    if (nxt) {
        date1 = get_date();
        nxt = date1 != NULL;
    }
    if (nxt) {
        date2 = get_date();
        nxt = date2 != NULL;
    }
    if (nxt) {
        found_ids = ffind_range(path, date1, date2);
        /*
int i = 0;
fflush(stdout);
while (found_ids[i] != -1) {
    printf("found: %llu| line: %llu\n", found_ids[i], eval_line(path, found_ids[i]));
    i++;
}
        */
        nxt = found_ids != NULL;
    }
    if (nxt) {
        nxt = rewrite_exclude(path, found_ids);
    }
    if (nxt) {
        printfile(path);
    }
    if (!nxt) {
        err();
    }
    if (path) {
        free(path);
    }
    if (date1) {
        free(date1);
    }
    if (date2) {
        free(date2);
    }
    if (found_ids) {
        free(found_ids);
    }
    return 0;
}

#pragma endregion

#pragma region quest1

void err() {
    fflush(stdout);
    printf("n/a");
}

char* getpath() {
    char* str = NULL;
    char let = ' ';
    int length = 0;
    cleari();
    while (scanf("%c", &let) != 0 && let != '\n' && let) {
        length++;
        char* prerealloc = str;
        str = realloc(str, sizeof(char*) + (length) * sizeof(char));
        if (str == NULL) {
            length = 0;
            str = prerealloc;
            let = '\0';
        } else {
            str[length - 1] = let;
        }
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

int validate_path(const char* path) {
    FILE* file = NULL;
    file = fopen(path, "r+b");
    int nxt = file != NULL;
    fclose(file);
    file = NULL;
    return nxt;
}

int readtask() {
    int task = TASK_INV;
    cleari();
    int res = scanf("%d", &task);
    if (!res) {
        task = TASK_INV;
    }
    if (task < TASK_PRINT || task > TASK_ADD) {
        task = TASK_INV;
    }
    return task;
}

int printfile(const char* path) {
    size_t length = fgetlines(path);
    int nxt = 1;
    for (size_t i = 0; i < length && nxt; i++) {
        // printf("%lld: ", i);
        int* newline = readline(path, i);
        nxt = newline != NULL;
        if (nxt) {
            printline(newline);
            if (i != length - 1) {
                printf("\n");
            }
            free(newline);
        }
    }
    return nxt;
}

void printline(int* line) {
    fflush(stdout);
    for (int i = 0; i < LINESIZE; i++) {
        printf("%d", line[i]);
        if (i != 7) {
            printf(" ");
        }
    }
}

int* readline(const char* path, size_t id) {
    FILE* file = fopen(path, "rb");
    fseek(file, 0, SEEK_SET);
    int offset = id * sizeof(int) * LINESIZE;
    int* newline = malloc(sizeof(int) * 8 + sizeof(int*));
    int nxt = newline != NULL && 0 == fseek(file, offset, SEEK_SET);
    if (nxt) {
        fread(newline, sizeof(int), LINESIZE, file);
        rewind(file);
    }
    fclose(file);
    return newline;
}

int writeline(const char* path, const int* line, size_t id) {
    FILE* file = fopen(path, "r+b");
    int offset = id * sizeof(int) * LINESIZE;
    int nxt = fseek(file, offset, SEEK_SET) == 0;
    if (nxt) {
        fwrite(line, sizeof(int), LINESIZE, file);
        fflush(file);
        rewind(file);
    }
    fclose(file);
    return nxt;
}

size_t fgetbytes(const char* path) {
    FILE* file = fopen(path, "rb");
    rewind(file);
    size_t size = 0;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    fclose(file);
    return size;
}

size_t fgetlines(const char* path) { return fgetbytes(path) / (sizeof(int) * LINESIZE); }

unsigned long long eval_line(const char* path, size_t id) {
    int* line = readline(path, id);
    unsigned long long val = 0;
    int nxt = line != NULL;
    if (nxt) {
        for (int i = 5; i >= 0; i--) {
            unsigned long long number = (unsigned long long)line[i];
            for (int j = 0; j < 5 - i; j++) {
                number *= 100;
            }
            val = val + number;
        }
        free(line);
    }
    return val;
}

int sortfile(const char* path) {
    sort_preshuffle(path, fgetlines(path) - 1);
    return qsortf(path, 0, fgetlines(path) - 1);
}

int qsortf(const char* path, int low, int high) {
    int nxt = 1;
    if (low < high) {
        int pid;
        nxt = qpartf(path, low, high, &pid);
        if (nxt) {
            qsortf(path, low, pid - 1);
            qsortf(path, pid + 1, high);
        }
    }
    return nxt;
}

int qpartf(const char* path, int low, int high, int* k) {
    unsigned long long pval = eval_line(path, high);
    // printf("qpartf.pval: %llu\n", pval);
    *k = low - 1;
    int nxt = 1;
    for (int i = low; (i <= high - 1) && nxt; i++) {
        unsigned long long eval = eval_line(path, i);
        // printf("qpartf.eval: %lli | line: %i | pivot: %i\n", eval,i,*k);
        if (eval < pval) {
            (*k)++;
            nxt = swap_lines(path, *k, i);
        }
    }
    if (nxt) {
        nxt = swap_lines(path, (*k) + 1, high);
        (*k)++;
    }
    return nxt;
}

int swap_lines(const char* path, size_t id1, size_t id2) {
    int skip = id1 == id2;
    int nxt = skip;
    if (!skip) {
        int* line1 = readline(path, id1);
        int* line2 = readline(path, id2);
        nxt = line1 != NULL && line2 != NULL;
        if (nxt) {
            writeline(path, line1, id2);
            writeline(path, line2, id1);
            // fflush(stdout);
            // printf("Swapping lines %lld and %lld\n", id1, id2);
        }
        if (line1) {
            free(line1);
        }
        if (line2) {
            free(line2);
        }
    }
    return nxt;
}

int* readline_std() {
    int* newline = malloc(sizeof(int) * 8 + sizeof(int*));
    int nxt = newline != NULL;
    if (nxt) {
        int field = 0;
        cleari();
        while (field != 8 && nxt) {
            char separator = '\n';
            int fres = scanf("%d", &(newline[field]));
            int sepres = scanf("%c", &separator);
            nxt = fres * sepres;
            int bool1 = separator == ' ' && field != 7;
            int bool2 = (separator == '\n' || separator == '\0') && field == 7;
            nxt = nxt && (bool1 || bool2);
            if (nxt) {
                if (field == 0) {
                    nxt = newline[field] >= 0;
                }
                if (field == 1) {
                    nxt = newline[field] >= 0 && newline[field] <= 12;
                }
                if (field == 2) {
                    nxt = newline[field] >= 0 && newline[field] <= 31;
                }
                if (field == 3) {
                    nxt = newline[field] >= 0 && newline[field] <= 23;
                }
                if (field == 4 || field == 5) {
                    nxt = newline[field] >= 0 && newline[field] <= 59;
                }
                if (field == 6) {
                    nxt = newline[field] == 0 || newline[field] == 1;
                }
                if (field == 7) {
                    nxt = newline[field] >= 0;
                }
                field++;
            }
        }
        if (!nxt) {
            free(newline);
            newline = NULL;
        }
    }
    return newline;
}

int addline(const char* path, const int* line) {
    size_t id = fgetlines(path);
    return writeline(path, line, id);
}

void sort_preshuffle(const char* path, int high) {
    srand(time(NULL));
    int nxt = 1;
    for (int i = high; i > 0 && nxt; i--) {
        int j = (rand() % i + 1);
        nxt = swap_lines(path, i, j);
    }
}

#pragma endregion

#pragma region quest2

int* get_date() {
    char* buffer = malloc(sizeof(char) * _strb + sizeof(char*));
    int* date = malloc(3 * sizeof(int) + sizeof(int*));
    int nxt = buffer != NULL;
    fflush(stdout);
    if (nxt) {
        cleari();
        buffer = fgets(buffer, _strb, stdin);
        nxt = (buffer != NULL);
    }
    if (nxt) {
        char* ptr = buffer;
        int dotcount = 0;
        int numcount = 0;
        while (*ptr && nxt && *ptr != '\n') {
            nxt = *ptr == '.' || (*ptr >= '0' && *ptr <= '9');
            if (*ptr == '.') {
                dotcount++;
                nxt = numcount <= 2;
                if (nxt) {
                    numcount = 0;
                }
            }
            if (*ptr >= '0' && *ptr <= '9') {
                numcount++;
            }
            ptr++;
        }
        if (nxt) {
            nxt = dotcount == 2;
        }
    }
    if (nxt) {
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
                field++;
            }
            nxt = field < 3;
            if (nxt && str != NULL) {
                date[field] = atoi(str);
            }
        } while (nxt && str != NULL);
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

int ffind_code(const char* path, const int* what, size_t fields) {
    const int code_id = 7;
    int found = 0;
    int code = -1;
    size_t id = 0;
    size_t fsize = fgetlines(path);
    int nxt = fsize != 0;
    while (!found && nxt && id < fsize) {
        int* line = readline(path, id);
        size_t count = (size_t)0;
        nxt = (line != NULL);
        if (nxt) {
            for (size_t i = 0; i < fields; i++) {
                if (what[i] == line[i]) {
                    count++;
                }
            }
            found = count == fields;
        }
        if (!found) {
            id++;
        }
        if (found) {
            code = line[code_id];
        }
        if (line != NULL) {
            free(line);
            line = NULL;
        }
    }
    if (!found) {
        code = -1;
    }
    return code;
}

#pragma endregion
#pragma region quest3

// returns long long int* array of ids, terminated with -1; if found nothing, returns -1 in array[0];
long long int* ffind_range(const char* path, const int* low3, const int* high3) {
    size_t fsize = fgetlines(path);
    int nxt = fsize != 0;
    long long int* ids = NULL;
    size_t top_id = 0;
    unsigned long long low_val = eval_date(low3);
    unsigned long long high_val = eval_date(high3);
    if (nxt) {
        ids = malloc((1 + fsize) * sizeof(size_t) + sizeof(long long int*));
        nxt = ids != NULL;
    }
    for (size_t i = 0; (i < fsize) && nxt; i++) {
        int* line = readline(path, i);
        nxt = line != NULL;
        if (nxt) {
            unsigned long long line_val = eval_date(line);
            free(line);
            int found = low_val <= line_val && line_val <= high_val;
            if (found) {
                ids[top_id] = i;
                top_id++;
            }
        }
    }
    if (nxt) {
        ids[top_id] = -1;
    } else {
        if (ids) {
            free(ids);
            ids = NULL;
        }
    }
    return ids;
}

int rewrite_exclude(const char* path, const long long int* id_exclude) {
    char tmppath[30];
    srand(time(NULL));
    sprintf(tmppath, "%d", rand());
    FILE* newfile = fopen(tmppath, "wb");
    int nxt = newfile != NULL;
    fclose(newfile);
    size_t lines = fgetlines(path);
    size_t newpos = 0;
    for (size_t i = 0; i < lines && nxt; i++) {
        if (!is_in_ids(i, id_exclude)) {
            int* line = readline(path, i);
            nxt = line != NULL;
            if (nxt) {
                writeline(tmppath, line, newpos);
                free(line);
                newpos++;
            }
        }
    }
    if (nxt) {
        nxt = !remove(path);
        fflush(stdout);
        printf("remove fault: %d\n", nxt);
    }
    if (nxt) {
        nxt = !rename(tmppath, path);
        fflush(stdout);
        printf("rename fault: %d\n", nxt);
    }
    return nxt;
}

int is_in_ids(long long int id, const long long int* ids) {
    int found = 0;
    int i = 0;
    while (ids[i] != -1 && !found) {
        found = ids[i] == id;
        i++;
    }
    return found;
}

void cleari() { fseek(stdin, 0, SEEK_END); }

unsigned long long eval_date(const int* date) {
    unsigned long long val = 0;
    int nxt = date != NULL;
    if (nxt) {
        for (int i = 2; i >= 0; i--) {
            unsigned long long number = (unsigned long long)date[i];
            for (int j = 0; j < 2 - i; j++) {
                number *= 100;
            }
            val = val + number;
        }
    }
    return val;
}