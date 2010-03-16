#include <stdlib.h>
#include <stdio.h>
#include "polygon.h"

vertex * vertex_new (int x, int y) {
  vertex *v;

  Alloc (v);

  v->x = x;
  v->y = y;
  v->next = NULL;
  v->prev = NULL;
  v->mark = 0;

  return v;
}

vertex * vertex_dup (vertex *v) {
  vertex *w = vertex_new (v->x, v->y);
  w->mark = v->mark;
  return w;
}

void vertex_free (vertex *v) {
  if (v) free (v);
}

void vertex_link (vertex *v1, vertex *v2) {
  v1->next = v2;
  v2->prev = v1;
}

void vertex_unlink (vertex *v1, vertex *v2) {
  v1->next = NULL;
  v2->prev = NULL;
}

void vertex_print (vertex *v) {
  printf ("[%9p] -> (%3d,%3d)[%9p]|%d| -> [%9p]\n",
          (void *) v->prev,
          v->x, v->y,
          (void *) v,
          v->mark,
          (void *) v->next);
}

/* ============ Polygons ============ */

poly * poly_new () {
  poly *p;

  Alloc (p);

  p->size = 0;
  p->hull = NULL;
  p->last = NULL;
  p->lo   = NULL;
  p->hi   = NULL;

  return p;
}

poly * poly_dup (poly *p) {
  poly   *d = poly_new ();
  vertex *v;

  for (v = p->hull; v != p->last; v = v->next)
    poly_add (d, vertex_dup (v));
  poly_add (d, vertex_dup (v));

  return d;
}

void poly_free (poly *p) {
  if (p) {
    vertex *v;

    for (v = p->hull; v != p->last; v = v->next)
      vertex_free (v);
    vertex_free (v);

    free (p);
  }
}

void poly_add (poly *p, vertex *v) {
  if (NULL == p->hull) {
    p->hull = p->last = v;
  } else {
    vertex_link (p->last, v);
    p->last = v;
  }
  vertex_link (p->last, p->hull);

  poly_update_low  (p, v);
  poly_update_high (p, v);

  p->size++;
}

void poly_remove (poly *p, vertex *v) {
  if (p->size == 1) {
    vertex_free (p->hull);
    p->hull = p->last = NULL;
  } else {
    if (p->hull == v) p->hull = v->next;
    if (p->last == v) p->last = v->prev;
    vertex_link (v->prev, v->next);
    vertex_free (v);
  }

  p->size--;
}

void poly_reverse (poly *p) {
  vertex **points;
  vertex  *t = p->hull;
  int      i = 0;

  AllocN(points, p->size);

  for (i = 0; i < p->size; i++, t = t->next)
    points[i] = t;

  for (i = 0; i < p->size; i++) {
    t = points[i]->next;
    points[i]->next = points[i]->prev;
    points[i]->prev = t;
  }

  t       = p->hull;
  p->hull = p->last;
  p->last = t;

  free (points);
}

void poly_print (poly *p) {
  vertex *t = p->hull;

  printf ("[");
  if (p->hull) {
    printf ("(%d, %d)", p->hull->x, p->hull->y);
    for (t = p->hull; t != p->last; t = t->next) {
      printf (", (%d, %d)", t->next->x, t->next->y);
    }
  }
  printf ("]\n");
}

void poly_update (poly *p, vertex *v, int x, int y) {
  v->x = x;
  v->y = y;
  poly_update_low  (p, v);
  poly_update_high (p, v);
}

void poly_update_low (poly *p, vertex *v) {
  p->lo = ((NULL == p->lo) || (v->y < p->lo->y) ||
           (v->y == p->lo->y && v->x < p->lo->x)) ? v : p->lo;
}

void poly_update_high (poly *p, vertex *v) {
  p->hi = ((NULL == p->hi) || (v->y > p->hi->y) ||
           (v->y == p->hi->y && v->x > p->hi->x)) ? v : p->hi;
}

vertex * poly_nearest (poly *p, int x, int y) {
  vertex *n = NULL,
         *t = p->hull;

  if (!t) return n;

  while (t) {
    if (t->x >= x - 3 && t->x <= x + 3 &&
        t->y >= y - 3 && t->y <= y + 3)
      n = t;
    if (t == p->last) break;
    else t = t->next;
  }

  return n;
}

vertex * poly_lowest (poly *p) {
  return p->lo;
}

vertex * poly_highest (poly *p) {
  return p->hi;
}

vertex ** poly_to_array (poly *p) {
  vertex  *t;
  vertex **arr;
  int      i = 0;

  AllocN(arr, p->size);

  for (t = p->hull; t != p->last; t = t->next)
    arr[i++] = vertex_dup (t);
  arr[i] = vertex_dup (t);

  return arr;
}
