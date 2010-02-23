#include <stdlib.h>
#include <stdio.h>
#include "stack.h"

stack * stack_new (int size) {
  stack *s;

  Alloc (s);
  AllocN (s->items, size);

  s->size = size;
  s->cur  = 0;

  return s;
}

void stack_free (stack *s) {
  if (s) {
    free (s->items);
    free (s);
  }
}

void stack_push (stack *s, void *item) {
  if (s->cur == s->size) {
    int new_size = 2 * s->size;
    s->items = realloc (s->items, new_size * sizeof(*s->items));
    assert (NULL != s->items);
    s->size = new_size;
  }

  s->items[s->cur++] = item;
}

void * stack_peek (stack *s) {
  if (stack_is_empty (s))
    return NULL;
  else
    return s->items[s->cur - 1];
}

void * stack_pop (stack *s) {
  void * out = stack_peek (s);
  if (!stack_is_empty (s)) s->cur--;

  return out;
}

bool stack_is_empty (stack *s) {
  return (s->cur == 0);
}
