#include <stdlib.h>
#include <stdio.h>
#include "polygon.h"

vertex * vertex_new (int x, int y) {
  vertex *v;

  Alloc (v);

  v->x = x;
  v->y = y;
  v->next = NULL;

  return v;
}

vertex * vertex_dup (vertex *v) {
  return vertex_new (v->x, v->y);
}

vertex * vertex_rdup (vertex *v) {
  vertex *d;

  if (NULL == v) return NULL;
  d = vertex_dup (v);
  d->next = vertex_rdup (v->next);

  return d;
}

void vertex_free (vertex *v) {
  free (v);
  v = NULL;
}

void vertex_rfree (vertex *v) {
  if (NULL != v->next)
    vertex_rfree (v->next);
  vertex_free (v);
}

void vertex_link (vertex *v1, vertex *v2) {
  v1->next = v2;
}

vertex * vertex_last (vertex *v) {
  vertex *t = v;

  while (t->next != NULL) t = t->next;

  return t;
}

int vertex_chain_length (vertex *v) {
  if (v == NULL) return 0;
  return 1 + vertex_chain_length (v->next);
}

/* ============ Polygons ============ */

poly * poly_new () {
  poly *p;

  Alloc (p);

  p->vertex_c = 0;
  p->hull     = NULL;

  return p;
}

poly * poly_dup (poly *p) {
  poly *d;

  Alloc (d);

  d->vertex_c = p->vertex_c;
  d->hull     = vertex_rdup (p->hull);

  return d;
}

poly * poly_from_list (int *l, int n) {
  poly   *p;
  int     i = 0;

  p = poly_new ();

  for (i = 0; i < 2*n; i += 2)
    poly_add (p, vertex_new (l[i], l[i+1]));

  return p;
}

void poly_free (poly *p) {
  vertex_rfree (p->hull);
  free (p);
}

void poly_add (poly *p, vertex *v) {
  if (NULL == p->hull)
    p->hull = v;
  else
    vertex_link (vertex_last (p->hull), v);

  p->vertex_c++;
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

vertex * poly_lowest (poly *p) {
  vertex *v, *l = p->hull;

  for (v = p->hull; v != NULL; v = v->next)
    if (v->y < l->y) l = v;

  return l;
}

vertex * poly_highest (poly *p) {
  vertex *v, *h = p->hull;

  for (v = p->hull; v != NULL; v = v->next)
    if (v->y > h->y) h = v;

  return h;
}
