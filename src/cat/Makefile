CC=gcc
CFLAGS=-Wall -Werror -Wextra
QUEST_CFILES= s21_cat.c

all: s21_cat

clean:
	rm -rf *.o
	rm -rf s21_cat

rebuild: clean all
cat.o: s21_cat.c
	$(CC) $(CFLAGS) -c s21_cat.c -o s21_cat.o
s21_cat: cat.o
	$(CC) $(CFLAGS) *.o -o s21_cat
