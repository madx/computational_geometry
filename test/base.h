#ifndef TEST_H__
#define TEST_H__

#include "../src/util.h"

typedef struct test_status_t {
  int  passed;
  int  failed;
  bool ok;
} tstatus;

#define unit(m,s,f) do { puts(m); f (s); } while (0)

#define check(m,s,e) do {\
  bool p = e;\
  ((p) ? (s)->passed++ : (s)->failed++);\
  ((p)\
    ? puts ("  \033[32m+\033[0m " m)\
    : puts("  \033[31m-\033[0m " m " : \033[31m" __STRING(e) "\033[0m"));\
} while (0)

#define summary(s) do {\
  printf ("\n\033[32m%d\033[0m assertions passed\n", s.passed);\
  printf ("\033[31m%d\033[0m assertions failed\n", s.failed);\
} while(0);

#endif
