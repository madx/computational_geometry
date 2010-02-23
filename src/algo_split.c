#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"
#include "ui.h"

extern struct colors_t Color;

void gui_split_monotone_chains_algorithm (gui *g) {
  vertex *lo, *hi, *t;
  direction dir;

  lo = poly_lowest  (g->poly);
  hi = poly_highest (g->poly);

  dir = poly_get_direction (g->poly);

  gui_status_push (g, dir == CCW ? "Split: polygon drawn counter clock-wise":
                                   "Split: polygon drawn clock-wise");

  vertex_link (g->poly->last, g->poly->hull);

  /* TODO: refactor: reverse the poly and use only two loops */
  if (dir == CCW) {
    for (t = lo; t != hi; t = t->next) {
      gui_draw_edge (g, t, t->next, Color.low);
      if (t != lo && t != hi) gui_draw_vertex (g, t, Color.normal, Color.outline);
    }
    for (t = lo; t != hi; t = t->prev) {
      gui_draw_edge (g, t, t->prev, Color.high);
      if (t != lo && t != hi) gui_draw_vertex (g, t, Color.normal, Color.outline);
    }

  } else {
    for (t = lo; t != hi; t = t->next) {
      gui_draw_edge (g, t, t->next, Color.high);
      if (t != lo && t != hi) gui_draw_vertex (g, t, Color.normal, Color.outline);
    }
    for (t = lo; t != hi; t = t->prev) {
      gui_draw_edge (g, t, t->prev, Color.low);
      if (t != lo && t != hi) gui_draw_vertex (g, t, Color.normal, Color.outline);
    }
  }

  vertex_unlink (g->poly->last, g->poly->hull);

  gui_draw_vertex (g, lo, Color.low,  Color.outline);
  gui_draw_vertex (g, hi, Color.high, Color.outline);
}
