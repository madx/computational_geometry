#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"

direction poly_get_direction (poly *p) {
  return (poly_signed_area (p) < 0) ? CCW : CW;
}

double poly_signed_area (poly *p) {
  vertex *t;
  double  a = 0;

  for (t = p->hull; t != p->last; t = t->next)
    a += (t->x * t->next->y) - (t->next->x * t->y);

  return a/2;
}

bool poly_is_convex (poly *p) {
  return false;
}

vertex * poly_chain_left (poly *p) {
  direction dir = poly_get_direction (p);
  vertex *origin, *t, *prev;

  prev = origin = vertex_dup (p->lo);

  if (dir == CW) {
    for (t = p->lo->next; t != p->hi; t = t->next) {
      vertex_link (prev, vertex_dup (t));
      prev = prev->next;
    }
  } else {
    for (t = p->lo->prev; t != p->hi; t = t->prev) {
      vertex_link (prev, vertex_dup (t));
      prev = prev->next;
    }
  }

  for (t = origin; t != NULL; t = t->next) t->mark = CHAIN_LEFT;

  return origin;
}

vertex * poly_chain_right (poly *p) {
  direction dir = poly_get_direction (p);
  vertex *origin, *t, *prev;

  prev = origin = vertex_dup (p->lo);

  if (dir == CW) {
    for (t = p->lo->prev; t->next != p->hi; t = t->prev) {
      vertex_link (prev, vertex_dup (t));
      prev = prev->next;
    }
  } else {
    for (t = p->lo->next; t->prev != p->hi; t = t->next) {
      vertex_link (prev, vertex_dup (t));
      prev = prev->next;
    }
  }

  t = origin->next;
  vertex_free (origin);
  origin = t;

  for (t = origin; t != prev->next; t = t->next) t->mark = CHAIN_RIGHT;

  return origin;
}

bool vertex_chain_monotone (vertex *l) {
  vertex *t;

  for (t = l; t->next != NULL; t = t->next)
    if (t->next->y < t->y) return false;

  return true;
}

int vertex_cross (vertex *v1, vertex *v2, vertex *v3) {
  return (v2->x - v1->x)*(v3->y - v1->y) - (v2->y - v1->y)*(v3->x - v1->x);
}
