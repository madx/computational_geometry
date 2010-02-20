#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"
#include "ui.h"

void gui_convex_hull_algorithm (gui *g) {
  direction dir;

  if (g->poly->size == 3) {
    gui_algorithm_info (g, "Graham", "this is a triangle");
    return;
  }

  dir = poly_get_direction (g->poly);

}
