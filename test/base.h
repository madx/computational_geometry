#ifndef TEST_H__
#define TEST_H__

#include "../src/util.h"

typedef struct test_status_t {
  int  passed;
  int  failed;
  bool ok;
} tstatus;

#define unit(_m_,_s_,_f_) do { puts(_m_); _f_ (_s_); } while (0)

#define check(_m_,_s_,_e_) do {\
  bool _p_ = _e_;\
  ((_p_) ? (_s_)->passed++ : (_s_)->failed++);\
  ((_p_)\
    ? puts ("  \033[32m+\033[0m " _m_)\
    : puts("  \033[31m-\033[0m " _m_ " : \033[31m" __STRING(_e_) "\033[0m"));\
} while (0)

#define summary(_s_) do {\
  printf ("\n\033[32m%d\033[0m assertions passed\n", _s_.passed);\
  printf ("\033[31m%d\033[0m assertions failed\n", _s_.failed);\
} while(0);

#endif
