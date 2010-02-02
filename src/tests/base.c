#include <stdlib.h>
#include <stdio.h>
#include "base.h"

void unit (char *msg, bool (*test_func)(), tstatus *ts) {
  bool passed = test_func ();

  if (passed) {
    ts->passed++;
    printf ("\033[32m");
  } else {
    ts->failed++;
    ts->ok = false;
    printf ("\033[31m");
  }
  printf ("%s\033[0m\n", msg);
}

bool summary (tstatus ts) {
  printf ("\n\033[32m%d\033[0m tests passed\n", ts.passed);
  printf ("\033[31m%d\033[0m tests failed\n", ts.failed);

  return !ts.ok;
}

