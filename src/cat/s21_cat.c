#include "s21_cat.h"

int main(int argc, char **argv) {
  flag flags = cat_read(argc, argv);
  catfunc(argc, argv, flags);
}
int numlen(int n) {
  int kol = 0;
  if (n / 10 == 0)
    kol = 1;
  else {
    while (n > 0) {
      n = n / 10;
      kol += 1;
    }
  }
  return kol;
}
flag cat_read(int argc, char **argv) {
  flag flags = {0};
  struct option longopt[] = {{"number-nonblank", 0, NULL, 'b'},
                             {"number", 0, NULL, 'n'},
                             {"squezze-blank", 0, NULL, 's'},
                             {NULL, 0, NULL, 0}};
  int current = getopt_long(argc, argv, "+bevEnstT", longopt, NULL);
  for (; current != -1;
       current = getopt_long(argc, argv, "+bevEnstT", longopt, NULL)) {
    switch (current) {
      case 'b':
        flags.numberNonblank = 1;
        break;
      case 'e':
        flags.mark_endl = 1;
        flags.t = 1;
        break;
      case 'v':
        flags.t = 1;
        break;
      case 'E':
        flags.mark_endl = 1;
        break;
      case 'n':
        flags.n_all = 1;
        break;
      case 's':
        flags.squeeze = 1;
        break;
      case 't':
        flags.t = 1;
        flags.tab = 1;
        break;
      case 'T':
        flags.tab = 1;
        break;
      default:
        printf("usage: cat [-benstuv] [file ...]\n");
        flags.check = 1;
        break;
    }
  }
  return flags;
}
void flag_b(char ch, int *linecount, int *flag) {
  for (int i = 0; i < 6 - numlen(*linecount); i++) printf(" ");
  if (!(ch >= 0 || ch < 32 || ch != '\n' || ch != '\t' || ch == 127)) {
    printf("%d\t%c", *linecount, ch);
    *flag = 0;
  } else
    printf("%d\t", *linecount);
}

void flag_v(int *flag, char ch) {
  if (ch < 32 && ch != '\n' && ch != '\t') {
    printf("^%c", ch + 64);
    *flag = 0;
  }
  if (ch == 127) {
    printf("^?");
    *flag = 0;
  }
}
void flag_tab(int *flag, char ch) {
  if (ch == '\t') {
    printf("^I");
    *flag = 0;
  }
}
void catfunc(int argc, char **argv, flag flags) {
  int id = optind;
  char prev = '\n', prev2 = ' ';
  int linecount = 1, flag = 1;
  char ch;
  if (argc > 1) {
    if (flags.check == 0) {
      while (id < argc) {
        FILE *f = fopen(argv[id], "r");
        if (f != NULL) {
          ch = fgetc(f);
          while (ch != EOF) {
            if (flags.numberNonblank) flags.n_all = 0;
            if (flags.squeeze && prev2 == '\n' && prev == '\n' && ch == '\n')
              flag = 0;
            if (flags.numberNonblank && prev == '\n' && ch != '\n') {
              flag_b(ch, &linecount, &flag);
              linecount++;
            }
            if (flags.n_all && flag && prev == '\n') {
              for (int i = 0; i < 6 - numlen(linecount); i++) printf(" ");
              printf("%d\t", linecount);
              linecount++;
            }
            if (flags.t) {
              flag_v(&flag, ch);
            }
            if (flags.mark_endl && flag) {
              if (ch == '\n') printf("$");
            }
            if (flags.tab) flag_tab(&flag, ch);
            prev2 = prev;
            prev = ch;
            if (flag) printf("%c", ch);
            ch = getc(f);
            flag = 1;
          }
        } else
          fprintf(stderr, "cat: %s: No such file or directory\n", argv[id]);
        fclose(f);
        id++;
        linecount = 1;
      }
    }
  } else
    fprintf(stderr, "%s too few arguments to function\n", argv[id]);
}
