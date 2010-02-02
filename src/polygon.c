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

int vertex_chain_length (vertex *v) {
  int l = 0;
  vertex *tmp = v;

  while (NULL != tmp) {
    l++;
    tmp = tmp->next;
  }

  return l;
}

poly * poly_new (vertex *hull) {
  poly *p;

  Alloc (p);

  p->vertex_c = vertex_chain_length (hull);
  p->hull = hull;

  return p;
}

void poly_free (poly *p) {
  vertex_rfree (p->hull);
  free (p);
}

void poly_add (poly *p, vertex *v) {
  vertex *last = p->hull;

  while (NULL != last->next) last = last->next;
  vertex_link (last, v);
  p->vertex_c++;
}
