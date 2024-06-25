CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

LC=clang-format
LFLAGS=-style=file:$(CLNGPATH) -i

CPPC=cppcheck
CPPCFLAGS=--enable=all --std=c11 --suppress=missingIncludeSystem --check-config

BPATH=build
SRCPATH=src
CLNGPATH:=materials/linters/.clang-format

NAMES_1=binlib.c state_sort.c 
FILE_1=$(addprefix $(SRCPATH)/,$(NAMES_1))
OBJ_1=$(FILE_1:.c=.o)
OUT_1=$(BPATH)/Quest_1.exe

NAMES_2=binlib.c state_search.c
FILE_2=$(addprefix $(SRCPATH)/,$(NAMES_2))
OBJ_2=$(FILE_2:.c=.o)
OUT_2=$(BPATH)/Quest_2.exe

.o: .c
	$(CC) -c $(CFLAGS) $^ -o $@

all: state_sort state_search clean

dbg: ss_debug ss_rgdb

ssort_run: state_sort $(OUT_1)
	./$(OUT_1)

ssort_rgdb: ssort_debug $(OUT_1)
	gdb $(OUT_1)

ssearch_run: state_search $(OUT_2)
	./$(OUT_2)

ssearch_rgdb: ssearch_debug $(OUT_2)
	gdb $(OUT_2)

state_sort: $(OBJ_1)
#	$(LC) $(LFLAGS) $(FILE_1)
#	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) $(CFLAGS) $(OBJ_1) -o $(OUT_1)

ssort_debug:
#	$(LC) $(LFLAGS) $(FILE_1)
#	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) $(CFLAGS) -g $(FILE_1) -o $(OUT_1)

state_search: $(OBJ_2)
#	$(LC) $(LFLAGS) $(FILE_2)
#	$(CPPC) $(CPPCFLAGS) $(FILE_2)
	$(CC) $(CFLAGS) $(OBJ_2) -o $(OUT_2)

ssearch_debug:
#	$(LC) $(LFLAGS) $(FILE_2)
#	$(CPPC) $(CPPCFLAGS) $(FILE_2)
	$(CC) $(CFLAGS) -g $(FILE_2) -o $(OUT_2)

clean:
	rm src/*.o