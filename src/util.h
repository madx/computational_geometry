#ifndef UTIL_H__
#define UTIL_H__

#include "assert.h"

typedef enum { false, true } bool;

#define Alloc(p) {p = malloc (sizeof(*p)); assert (NULL != p);}
#define AllocN(p, n) {p = malloc (n * sizeof(*p)); assert (NULL != p);}

#define array_swap(a, i, j) do {void* p = a[i]; a[i] = a[j]; a[j] = p;} while (0)

#endif
