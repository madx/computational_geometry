#ifndef ALGORITHMS_H__
#define ALGORITHMS_H__

#include "util.h"
#include "polygon.h"

typedef enum { CCW, CW } direction;

direction poly_get_direction (poly *p);

#endif
