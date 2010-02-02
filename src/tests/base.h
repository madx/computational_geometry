#ifndef TEST_H__
#define TEST_H__

#include "../util.h"

typedef struct test_status_t {
  int  passed;
  int  failed;
  bool ok;
} tstatus;

void unit (char *msg, bool (*test_func)(), tstatus *ts);
bool summary (tstatus ts);

#endif
