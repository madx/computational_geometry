#ifndef STACK_H__
#define STACK_H__

#include "util.h"

typedef struct stack_t {
  void **items;
  int    size;
  int    cur;
} stack;

stack * stack_new (int size);

void stack_free (stack *s);
void stack_push (stack *s, void *item);

void * stack_peek  (stack *s);
void * stack_peekn (stack *s, int n);
void * stack_pop   (stack *s);

void stack_print (stack *s);

bool stack_is_empty (stack *s);

#endif
