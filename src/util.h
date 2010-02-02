#ifndef UTIL_H__
#define UTIL_H__

#include "assert.h"

typedef enum { false, true } bool;

#define Alloc(p) {p = malloc (sizeof(*p)); assert (NULL != p);}
#define AllocN(p, n) {p = malloc (n * sizeof(*p)); assert (NULL != p);}

#endif
