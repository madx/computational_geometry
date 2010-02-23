#include <stdlib.h>
#include <stdio.h>
#include "base.h"
#include "../src/stack.h"

void test_stack_new  (tstatus *ts);
void test_stack_push (tstatus *ts);
void test_stack_pop  (tstatus *ts);
void test_stack_peek (tstatus *ts);

int main (int argc, char *argv[]) {
  tstatus ts = {0, 0, true};

  unit ("stack_new",  &ts, test_stack_new);
  unit ("stack_push", &ts, test_stack_push);
  unit ("stack_pop",  &ts, test_stack_pop);
  unit ("stack_peek", &ts, test_stack_peek);

  return summary (ts);
}

void test_stack_new (tstatus *ts) {
  stack *s;

  s = stack_new (2);

  check ("sets the size", ts,
         s->size == 2);
  check ("initializes *cur to 0", ts,
         s->cur == 0);

  stack_free (s);
}

void test_stack_push (tstatus *ts) {
  stack *s = stack_new (1);
  char  *strs[2] = {"hello", "world"};

  stack_push (s, strs[0]);
  check ("pushes an item on top of the stack", ts,
         s->items[0] == strs[0]);
  check ("increments *cur", ts,
         s->cur == 1);

  stack_push (s, strs[1]);
  check ("allocates room if needed", ts,
         (s->items[1] == strs[1]) && (s->size == 2));

  stack_free (s);
}

void test_stack_pop (tstatus *ts) {
  stack *s = stack_new (1);
  char   c = 'a';

  check ("returns NULL if the stack is empty", ts,
         stack_pop (s) == NULL);

  stack_push (s, &c);
  check ("pops the item on top of the stack", ts,
         ((char*) stack_pop (s) == &c) && s->cur == 0);

  stack_free (s);
}

void test_stack_peek (tstatus *ts) {
  stack *s = stack_new (1);
  char   c = 'a';

  check ("returns NULL if the stack is empty", ts,
         stack_peek (s) == NULL);

  stack_push (s, &c);
  check ("returns the item on top of the stack", ts,
         (char*) stack_peek (s) == &c);

  stack_free (s);
}
