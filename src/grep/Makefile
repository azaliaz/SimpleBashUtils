CC=gcc
CFLAGS=-Wall -Werror -Wextra
QUEST_CFILES= s21_grep.c

all: s21_grep

clean:
	rm -rf *.o
	rm -rf s21_grep

rebuild: clean all
grep.o: s21_grep.c
	$(CC) $(CFLAGS) -c s21_grep.c -o s21_grep.o
s21_grep: grep.o
	$(CC) $(CFLAGS) *.o -o s21_grep