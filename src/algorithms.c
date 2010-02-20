#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"

direction poly_get_direction (poly *p) {
  direction dir;
  vertex   *hi = poly_highest (p);

  if (hi->next)
    dir = (hi->next->x > hi->x) ? CW : CCW;
  else
    dir = (hi->prev->x < hi->x) ? CW : CCW;

  return dir;
}

