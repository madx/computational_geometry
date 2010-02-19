#ifndef TEST_H__
#define TEST_H__

#include "../src/util.h"

typedef struct test_status_t {
  int  passed;
  int  failed;
  bool ok;
} tstatus;

void unit    (char *msg, tstatus *ts, void (*test_func)(tstatus *));
void check   (char *msg, tstatus *ts, bool expr);
bool summary (tstatus ts);

#endif
