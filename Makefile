CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra

LC=clang-format
LFLAGS=-style=file:$(CLNGPATH) -i

CPPC=cppcheck
CPPCFLAGS=--enable=all --std=c11 --suppress=missingIncludeSystem --check-config

BPATH=build
SRCPATH=src
CLNGPATH:=materials/linters/.clang-format

NAMES_1=state_sort.c
FILE_1=$(addprefix $(SRCPATH)/,$(NAMES_1))
OBJ_1=$(FILE_1:.c=.o)
OUT_1=$(BPATH)/Quest_1.exe

.o:.c
	$(CC) $(CFLAGS) -c $^ -o $@

all: state_sort clean

dbg: ss_debug clean gdb ss_rgdb

ss_run: state_sort $(OUT_1)
	./$(OUT_1)

ss_rgdb: ss_debug $(OUT_1)
	gdb ./$(OUT_1)

state_sort: $(OBJ_1)
#	$(LC) $(LFLAGS) $(FILE_1)
#	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) $(CFLAGS) $(OBJ_1) -o $(OUT_1)

ss_debug: $(OBJ_1)
#	$(LC) $(LFLAGS) $(FILE_1)
#	$(CPPC) $(CPPCFLAGS) $(FILE_1)
	$(CC) -g $(CFLAGS) $(OBJ_1) -o $(OUT_1)

clean:
	rm src/*.o