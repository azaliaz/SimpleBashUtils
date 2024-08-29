#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  bool numberNonblank;
  bool mark_endl;
  bool n_all;
  bool squeeze;
  bool tab;
  bool t;
  bool check;
} flag;

void catfunc(int argc, char **argv, flag flags);
void flag_tab(int *flag, char ch);
int numlen(int n);
flag cat_read(int argc, char **argv);
void flag_b(char ch, int *linecount, int *flag);
void flag_v(int *flag, char ch);