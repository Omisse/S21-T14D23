CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

LC=clang-format
LFLAGS=-style=file:$(CLNGPATH) -i

CPPC=cppcheck
CPPCFLAGS=--enable=all --suppress=missingIncludeSystem

BPATH=../build
CLNGPATH:=../materials/linters/.clang-format

NAMES_1=binlib.c state_sort.c 
FILE_1=$(NAMES_1)
OBJ_1=$(FILE_1:.c=.o)
OUT_1=$(BPATH)/Quest_1

NAMES_2=binlib.c state_search.c
FILE_2=$(NAMES_2)
OBJ_2=$(FILE_2:.c=.o)
OUT_2=$(BPATH)/Quest_2

NAMES_3=binlib.c clear_state.c
FILE_3=$(NAMES_3)
OBJ_3=$(FILE_3:.c=.o)
OUT_3=$(BPATH)/Quest_3

.o: .c
	$(CC) -c $(CFLAGS) $^ -o $@

all: state_sort state_search clear_state clean

ssort_run: state_sort $(OUT_1)
	./$(OUT_1)

ssort_leaks: state_sort $(OUT_1)
	leaks -atExit -- ./$(OUT_1)

ssearch_run: state_search $(OUT_2)
	./$(OUT_2)

ssearch_leaks: state_search $(OUT_2)
	leaks -atExit -- ./$(OUT_2)

sclear_run: clear_state $(OUT_3)
	./$(OUT_3)

sclear_leaks: clear_state $(OUT_3)
	leaks -atExit -- ./$(OUT_3)

state_sort: $(OBJ_1)
	$(LC) $(LFLAGS) $(FILE_1)
	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) $(CFLAGS) $(OBJ_1) -o $(OUT_1)

ssort_debug:
	$(LC) $(LFLAGS) $(FILE_1)
	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) $(CFLAGS) -g $(FILE_1) -o $(OUT_1)

state_search: $(OBJ_2)
	$(LC) $(LFLAGS) $(FILE_2)
	$(CPPC) $(CPPCFLAGS) $(FILE_2)
	$(CC) $(CFLAGS) $(OBJ_2) -o $(OUT_2)

ssearch_debug:
	$(LC) $(LFLAGS) $(FILE_2)
	$(CPPC) $(CPPCFLAGS) $(FILE_2)
	$(CC) $(CFLAGS) -g $(FILE_2) -o $(OUT_2)

clear_state: $(OBJ_3)
	$(LC) $(LFLAGS) $(FILE_3)
	$(CPPC) $(CPPCFLAGS) $(FILE_3)
	$(CC) $(CFLAGS) $(OBJ_3) -o $(OUT_3)

clear_debug:
	$(LC) $(LFLAGS) $(FILE_3)
	$(CPPC) $(CPPCFLAGS) $(FILE_3)
	$(CC) $(CFLAGS) -g $(FILE_3) -o $(OUT_3)

#clear_state

clean:
	rm -rf *.o