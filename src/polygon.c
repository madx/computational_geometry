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

  return v;
}

void vertex_free (vertex *v) {
  if (v) free (v);
}

void vertex_free_r (vertex *v) {
  if (v) {
    vertex_free_r (v->next);
    vertex_free   (v);
  }
}

void vertex_link (vertex *v1, vertex *v2) {
  v1->next = v2;
  v2->prev = v1;
}

void vertex_print (vertex *v) {
  printf ("[%9p] -> (% 2d,% 2d)[%9p] -> [%9p]\n",
          (void *) v->prev,
          v->x, v->y,
          (void *) v,
          (void *) v->next);
}

/* ============ Polygons ============ */

poly * poly_new () {
  poly *p;

  Alloc (p);

  p->size = 0;
  p->hull = NULL;
  p->last = NULL;

  return p;
}

void poly_free (poly *p) {
  if (p) {
    vertex_free_r (p->hull);
    free (p);
    p = NULL;
  }
}

void poly_add (poly *p, vertex *v) {
  if (NULL == p->hull) {
    p->hull = p->last = v;
  } else {
    vertex_link (p->last, v);
    p->last = v;
  }

  p->size++;
}

void poly_remove (poly *p, vertex *v) {
  if (v->prev == NULL && v->next == NULL) {
    p->hull = p->last = NULL;

  } else if (v->prev == NULL) {
    p->hull = v->next;
    v->next->prev = NULL;

  } else if (v->next == NULL) {
    p->last = v->prev;
    v->prev->next = NULL;

  } else vertex_link (v->prev, v->next);

  v->prev = v->next = NULL;

  p->size--;
}

vertex * poly_nearest (poly *p, int x, int y) {
  vertex *n = NULL,
         *t = p->hull;

  if (!p->hull) return n;

  while (t) {
    if (t->x >= x - 3 && t->x <= x + 3 &&
        t->y >= y - 3 && t->y <= y + 3)
      n = t;
    t = t->next;
  }

  return n;
}

/* vertex * poly_lowest (poly *p) { */
/*   vertex *v, *l = p->hull; */
/*  */
/*   for (v = p->hull; v != NULL; v = v->next) */
/*     if (v->y < l->y) l = v; */
/*  */
/*   return l; */
/* } */
/*  */
/* vertex * poly_highest (poly *p) { */
/*   vertex *v, *h = p->hull; */
/*  */
/*   for (v = p->hull; v != NULL; v = v->next) */
/*     if (v->y > h->y) h = v; */
/*  */
/*   return h; */
/* } */
