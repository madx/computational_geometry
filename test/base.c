#include <stdlib.h>
#include <stdio.h>
#include "base.h"

void unit (char *msg, tstatus *ts, void (*test_func)(tstatus *)) {
  printf ("%s ::\n", msg);
  test_func (ts);
}

void check (char *msg, tstatus *ts, bool expr) {
  if (expr) {
    printf ("\033[32m");
    ts->passed++;
  } else {
    printf ("\033[31m");
    ts->failed++;
    ts->ok = false;
  }
  printf ("  %s\033[0m\n", msg);
}

bool summary (tstatus ts) {
  printf ("\n\033[32m%d\033[0m assertions passed\n", ts.passed);
  printf ("\033[31m%d\033[0m assertions failed\n", ts.failed);

  return !ts.ok;
}

