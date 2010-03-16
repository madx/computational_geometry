#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "stack.h"
#include "algorithms.h"
#include "ui.h"

extern struct colors_t Color;

vertex *pivot;

/* void graham_show_names (gui *g, vertex **points, int n); */

void gui_graham_convex_hull_algorithm (gui *g) {
  vertex **points;
  vertex  *t;
  stack   *s;
  int      i, pindex;

  points = poly_to_array (g->poly);
  /* graham_show_names (g, points, g->poly->size); */

  points = realloc (points, (g->poly->size+1) * sizeof(*points));
  assert (NULL != points);

  /* Recherche du pivot et arrangements */
  pindex = 0;
  pivot  = points[pindex];
  for (i = 1; i < g->poly->size; i++) {
    if ((points[i]->y < pivot->y) ||
        (points[i]->y == pivot->y && points[i]->x < pivot->x)) {
      pindex = i;
      pivot = points[pindex];
    }
  }
  array_swap (points, 0, pindex);
  points[g->poly->size] = pivot;

  /* Tri des points par angle avec le pivot */
  qsort (points+1, g->poly->size - 1, sizeof(*points), graham_sort_points);

  /* Déroulement de l'algo */
  s = stack_new (g->poly->size + 1);
  stack_push (s, points[0]);
  stack_push (s, points[1]);

  for (i = 2; i < g->poly->size + 1; i++) {
    while ((s->cur >= 2) &&
           vertex_cross ((vertex*) stack_peekn (s, 2),
                         (vertex*) stack_peek  (s),
                         points[i]) > 0
          ) {
      stack_pop (s);
    }
    stack_push (s, points[i]);
  }

  /* Dessin */
  while (s->cur > 1) {
    t = stack_pop (s);
    gui_draw_edge (g, t, ((vertex*) stack_peek (s)), Color.outline);
    gui_draw_vertex (g, t, Color.low, Color.outline);
  }
  gui_draw_vertex (g, ((vertex*) stack_peek (s)), Color.low, Color.outline);

  stack_free (s);
  free (points);
}

int graham_sort_points (const void *p1, const void *p2) {
  vertex *v1 = *(vertex* const*) p1;
  vertex *v2 = *(vertex* const*) p2;

  return vertex_cross (pivot, v1, v2);
}

void graham_show_names (gui *g, vertex **points, int n) {
  PangoLayout *layout;
  char buf[10];
  int i;

  gc_set_fg (g->gc, "black");

  for (i = 0; i < n; i++) {
    g_sprintf (buf, "%p", (void*) points[i]);
    layout = gtk_widget_create_pango_layout (g->draw_zone, buf);
    gdk_draw_layout (g->pixmap, g->gc, points[i]->x - 8, points[i]->y - 16, layout);
  }

  gtk_widget_queue_draw (g->draw_zone);
}
