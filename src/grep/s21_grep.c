#include "s21_grep.h"
int main(int argc, char* argv[]) {
  flag flag = {0};
  ind ind = {0};
  int countfile = 0;
  int pat = 0;
  if (argc > 2) {
    if (flag.check != 1) {
      pat = parser(argc, argv, &flag, &ind);
      if (!flag.e && !flag.f) {
        ind.pattern[pat] = ind.files[countfile];
        int n = 0;
        while (ind.files[n] != 0) {
          ind.files[n] = ind.files[n + 1];
          n++;
        }
      }
      while (ind.files[countfile] != 0) {
        grep(ind.files[countfile++], argv, flag, ind);
      }
    } else
      printf(
          "usage: grep [-abcDEFGHhIiJLlmnOoqRSsUVvwxZ] [-A num] [-B num] "
          "[-C[num]]\n\t[-e pattern] [-f file] [--binary-files=value] "
          "[--color=when]\n\t[--context[=num]] [--directories=action] "
          "[--label] [--line-buffered]\n\t[--null] [pattern] [file ...]");
  } else
    fprintf(stderr, "%s too few arguments to function\n", argv[optind]);
  return 0;
}

int parser(int argc, char* argv[], flag* flag, ind* ind) {
  int tlenght = 0, fcount = 0, f = 0;
  const char* opt = "eivclnhsfo";
  while (optind < argc) {
    int current = getopt(argc, argv, opt);
    switch (current) {
      case 'e':
        flag->e = 1;
        ind->pattern[tlenght++] = optind++;
        break;
      case 'i':
        flag->i = 1;
        break;
      case 'v':
        flag->v = 1;
        break;
      case 'c':
        flag->c = 1;
        break;
      case 'l':
        flag->l = 1;
        break;
      case 'n':
        flag->n = 1;
        break;
      case 'h':
        flag->h = 1;
        break;
      case 's':
        flag->s = 1;
        break;
      case 'f':
        flag->f = 1;
        ind->opt_f[f++] = optind++;
        break;
      case 'o':
        flag->o = 1;
        break;
      case -1:
        ind->files[fcount++] = optind++;
        break;
      default:
        if (!flag->s) flag->check = 1;
        break;
    }
  }
  if (flag->v) flag->o = 0;
  return tlenght;
}
int pattern_find(char pattern[length], char str[length], flag flag,
                 regmatch_t pmatch[1]) {
  int res;
  regex_t preg;
  const size_t nmatch = 1;
  if (regcomp(&preg, pattern, REG_NEWLINE)) {
  } else {
    res = regexec(&preg, str, nmatch, pmatch, 0);
    regfree(&preg);
  }
  if (flag.i) {
    regcomp(&preg, pattern, REG_ICASE);
    res = regexec(&preg, str, nmatch, pmatch, 0);
    regfree(&preg);
  }
  return !res;
}

int flag_f(char* argv[], char str[length], flag flag, ind ind,
           regmatch_t pmatch[1]) {
  int template = 0;
  FILE* f = fopen(argv[ind.opt_f[0]], "r");
  if (f == NULL && !flag.s) {
    fprintf(stderr, "grep: %s: No such file or directory\n",
            argv[ind.opt_f[0]]);

  } else {
    char buf[length];
    while (fgets(buf, length, f)) {
      if (buf[strlen(buf) - 1] == '\n') buf[strlen(buf) - 1] = '\0';
      char pattern[length] = {0};
      strcpy(pattern, buf);
      if ((template = pattern_find(pattern, str, flag, pmatch))) {
        break;
      }
    }
    fclose(f);
    f = NULL;
  }
  return template;
}

int patter(char str[length], char* argv[], flag flag, ind ind,
           regmatch_t pmatch[1]) {
  int pat = 0, pat_f = 0;
  while (ind.pattern[pat] != '\0') {
    char pattern[length];
    strcpy(pattern, argv[ind.pattern[pat]]);
    if ((pat_f = pattern_find(pattern, str, flag, pmatch))) {
      break;
    }
    pat++;
  }
  if ((pat_f == 0) && (flag.f)) {
    pat_f = flag_f(argv, str, flag, ind, pmatch);
  }
  if (flag.v) {
    pat_f = !pat_f;
  }
  return pat_f;
}

void flag_o(char str[length], char* argv[], flag opt, ind ind,
            regmatch_t pmatch[1]) {
  while (patter(str, argv, opt, ind, pmatch)) {
    for (int j = 0; j < pmatch[0].rm_eo; j++) {
      if ((j >= pmatch[0].rm_so)) {
        printf("%c", str[j]);
      }
      str[j] = 128;
    }
    printf("\n");
  }
}

void grep(int fcount, char* argv[], flag flag, ind ind) {
  regmatch_t pmatch[1] = {0};
  FILE* f = fopen(argv[fcount], "r");
  if (f == NULL && !flag.s) {
    fprintf(stderr, "grep: %s: No such file or directory\n", argv[fcount]);
  } else {
    char buffer[length];
    int linecount = 0;
    int flag2 = 0;
    int count_str = 0;
    while (fgets(buffer, length, f)) {
      linecount++;
      if (patter(buffer, argv, flag, ind, pmatch)) {
        count_str++;
        flag2 = 1;
        if (!(flag.c || flag.l)) {
          if ((ind.files[1] != 0) && (!flag.h)) {
            printf("%s:", argv[fcount]);
          }
          if (flag.n) {
            printf("%d:", linecount);
          }
          if (!flag.o) {
            printf("%s", buffer);
          }
        }
      } else {
        flag2 = 0;
      }
      if (flag.o && !flag.c && !flag.l) {
        flag_o(buffer, argv, flag, ind, pmatch);
      }
    }
    if (flag.l) {
      if (count_str) {
        count_str = 1;
      }
    }
    if (flag.c) {
      if ((ind.files[1] != 0) && (!flag.h)) {
        printf("%s:", argv[fcount]);
      }
      printf("%d\n", count_str);
    }
    if (flag.l && count_str) {
      printf("%s\n", argv[fcount]);
    }
    if (((strchr(buffer, '\n') == NULL) && !flag.o && !flag.l && !flag.c &&
         flag2))
      printf("\n");
    fclose(f);
    f = NULL;
  }
}
