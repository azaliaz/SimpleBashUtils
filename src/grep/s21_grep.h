#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define length 4000
typedef struct flag {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int check;
} flag;

typedef struct index {
  int files[4000];
  int pattern[4000];
  int opt_f[4000];
} ind;

int parser(int argc, char* argv[], flag* flag, ind* ind);
int pattern_find(char pattern[length], char str[length], flag flag,
                 regmatch_t pmatch[1]);
int flag_f(char* argv[], char str[length], flag flag, ind ind,
           regmatch_t pmatch[1]);
int patter(char str[length], char* argv[], flag flag, ind ind,
           regmatch_t pmatch[1]);
void flag_o(char str[length], char* argv[], flag opt, ind ind,
            regmatch_t pmatch[1]);
void grep(int fcount, char* argv[], flag flag, ind ind);