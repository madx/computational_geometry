#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "stack.h"
#include "algorithms.h"
#include "ui.h"

/* Trouver le pivot P;
   Trier les points par angle (les points d'angle égal seront triés par rapport à leur abscisse);

   # Points[0] est le pivot, Points[longueur] aussi (fin du circuit)
   Pile.empiler(Points[0]);
   Pile.empiler(Points[1]);
   POUR i = 2 A Points.longueur
           TANT QUE (Pile.hauteur >= 2) ET (Produit_vectoriel(Pile.second, Pile.haut, Points[i]) <= 0)
                   Pile.dépiler;
           FIN TANT QUE
           Pile.empiler(Points[i]);
   FIN POUR
*/

extern struct colors_t Color;

vertex *pivot;

void graham_show_names (gui *g);

void gui_graham_convex_hull_algorithm (gui *g) {
  vertex **points;
  vertex  *t;
  stack   *s;
  int      i, pindex;
  direction dir = poly_get_direction (g->poly);

  graham_show_names (g);

  points = poly_to_array (g->poly);
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
  t = points[0];
  points[0] = pivot;
  points[pindex] = t;
  points[g->poly->size] = pivot;

  puts("");
  for (i = 0; i < g->poly->size; i++)
    vertex_print (points[i]);

  /* Tri des points par angle avec le pivot */
  qsort (points+1, g->poly->size - 1, sizeof(*points), graham_sort_points);

  puts("");
  for (i = 0; i < g->poly->size; i++)
    vertex_print (points[i]);

  /* Déroulement de l'algo */
  s = stack_new (g->poly->size + 1);
  stack_push (s, points[0]);
  stack_push (s, points[1]);

  for (i = 0; i < g->poly->size + 1; i++) {
    while ((dir == CCW) ? (s->cur >= 2) &&
           vertex_cross ((vertex*) stack_peekn (s, 2),
                         (vertex*) stack_peek  (s),
                         points[i]) > 0
                        : (s->cur >= 2) &&
           vertex_cross ((vertex*) stack_peekn (s, 2),
                         (vertex*) stack_peek  (s),
                         points[i]) < 0
           ) {
      stack_pop (s);
    }
    stack_push (s, points[i]);
  }

  stack_print (s);

  stack_free (s);
  free (points);
}

int graham_sort_points (const void *p1, const void *p2) {
  double a1, a2, diff;
  vertex *v1 = *(vertex* const*) p1;
  vertex *v2 = *(vertex* const*) p2;

  a1 = (v1->x - pivot->x) / (double) (v1->y - pivot->y);
  a2 = (v2->x - pivot->x) / (double) (v2->y - pivot->y);

  diff = a1 - a2;

  return (diff < 0) ? -1 : (diff == 0) ? 0 : 1;
}

void graham_show_names (gui *g) {
  PangoLayout *layout;
  char buf[10];
  vertex *t;

  gc_set_fg (g->gc, "black");

  for (t = g->poly->hull; t != g->poly->last; t = t->next) {
    g_sprintf (buf, "%p", (void*) t);
    layout = gtk_widget_create_pango_layout (g->draw_zone, buf);
    gdk_draw_layout (g->pixmap, g->gc, t->x - 8, t->y - 16, layout);
  }
  g_sprintf (buf, "%p", (void*) t);
  layout = gtk_widget_create_pango_layout (g->draw_zone, buf);
  gdk_draw_layout (g->pixmap, g->gc, t->x - 8, t->y - 16, layout);

  gtk_widget_queue_draw (g->draw_zone);
}
