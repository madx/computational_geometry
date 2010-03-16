#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "polygon.h"
#include "stack.h"
#include "ui.h"
#include "algorithms.h"

extern struct colors_t Color;

void gui_triangulate_monotone_algorithm (gui *g) {
  vertex *left, *right, *t;
  stack  *s;

  left  = poly_chain_left  (g->poly);
  right = poly_chain_right (g->poly);

  if (vertex_chain_monotone (left) && vertex_chain_monotone (right)) {
    vertex **merged, *cur;
    int      i;

    AllocN(merged, g->poly->size);
    s = stack_new (g->poly->size);

    vertex_merge_lists (merged, left, right);
    /* gui_split_lr_chains_algorithm (g); */
    /* mtr_show_numbers (g, merged); */

    stack_push (s, merged[0]);
    stack_push (s, merged[1]);

    for (i = 2; i < g->poly->size-1; i++) {
      if (merged[i]->mark != ((vertex *) stack_peek (s))->mark) {
        while (!stack_is_empty (s)) {
          cur = ((vertex*) stack_pop (s));
          if (!stack_is_empty (s)) {
            gui_draw_edge (g, merged[i], cur, Color.outline);
          } else break;
        }
        stack_push (s, merged[i-1]);
        stack_push (s, merged[ i ]);

      } else {
        cur = stack_pop (s);

        if (merged[i]->mark == CHAIN_RIGHT)
          while (!stack_is_empty (s) &&
                 vertex_cross (stack_peek (s), cur, merged[i]) < 0) {
            gui_draw_edge (g, merged[i], ((vertex*) stack_peek (s)), Color.outline);
            cur = stack_pop (s);
          }
        else
          while (!stack_is_empty (s) &&
                 vertex_cross (stack_peek (s), cur, merged[i]) > 0) {
            gui_draw_edge (g, merged[i], ((vertex*) stack_peek (s)), Color.outline);
            cur = stack_pop (s);
          }
        stack_push (s, cur);
        stack_push (s, merged[i]);
      }
    }

    stack_pop (s);
    while (s->cur > 1) {
      cur = stack_pop (s);
      gui_draw_edge (g, cur, merged[i], Color.outline);
    }

    free (merged);
    stack_free (s);
  } else {
    gui_status_set (g, "This polygon is not monotone");
  }

  for (t = left; t != NULL; t = t->next)
    vertex_free (t);
  for (t = right; t != NULL; t = t->next)
    vertex_free (t);
}

void vertex_merge_lists (vertex **m, vertex *l, vertex *r) {
  vertex *a = l,
         *b = r;
  int     i = 0;

  while (a && b) {
    if (a->y < b->y) {
      m[i++] = a;
      a = a->next;
    } else {
      m[i++] = b;
      b = b->next;
    }
  }
  for (; a != NULL; a = a->next) m[i++] = a;
  for (; b != NULL; b = b->next) m[i++] = b;
}

void mtr_show_numbers (gui *g, vertex **l) {
  PangoLayout *layout;
  char         buf[5];
  int          i = 0;

  gc_set_fg (g->gc, "black");

  for (i = 0; i < g->poly->size; i++) {
    g_sprintf (buf, "%d", i);
    layout = gtk_widget_create_pango_layout (g->draw_zone, buf);
    gdk_draw_layout (g->pixmap, g->gc, l[i]->x - 8, l[i]->y - 16, layout);
  }

  gtk_widget_queue_draw (g->draw_zone);
}
