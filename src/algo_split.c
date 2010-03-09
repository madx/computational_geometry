#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"
#include "ui.h"

extern struct colors_t Color;

void gui_split_lr_chains_algorithm (gui *g) {
  vertex *left, *right, *t, *lend, *rend;

  left  = poly_chain_left  (g->poly);
  right = poly_chain_right (g->poly);

  for (t = left; t != NULL; t = t->next) {
    if (t->next)
      gui_draw_edge (g, t, t->next, Color.low);
    else
      lend = t;
    gui_draw_vertex (g, t, Color.low, Color.outline);
  }

  for (t = right; t != NULL; t = t->next) {
    if (t->next)
      gui_draw_edge (g, t, t->next, Color.high);
    else
      rend = t;
    gui_draw_vertex (g, t, Color.high, Color.outline);
  }

  gui_draw_edge (g, lend, rend,  Color.low);
  gui_draw_edge (g, left, right, Color.high);
  gui_draw_vertex (g, right, Color.high, Color.outline);
  gui_draw_vertex (g, left,  Color.low,  Color.outline);
  gui_draw_vertex (g, rend,  Color.high, Color.outline);
  gui_draw_vertex (g, lend,  Color.low,  Color.outline);

  for (t = left; t != NULL; t = t->next)
    vertex_free (t);
  for (t = right; t != NULL; t = t->next)
    vertex_free (t);
}
