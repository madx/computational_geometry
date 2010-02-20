#ifndef POLYGON_H__
#define POLYGON_H__

#include "util.h"

typedef struct vertex_t vertex;
typedef struct poly_t   poly;

struct vertex_t {
  int     x, y;
  vertex *next, *prev;
};

struct poly_t {
  int     size;
  vertex *hull;
  vertex *last;
};

/* --------------------- *
 * Functions on vertices *
 * --------------------- */

vertex * vertex_new (int x, int y);

void vertex_free   (vertex *v);
void vertex_free_r (vertex *v);
void vertex_link   (vertex *v1, vertex *v2);
void vertex_unlink (vertex *v1, vertex *v2);
void vertex_print  (vertex *v);

/* --------------------- *
 * Functions on polygons *
 * --------------------- */

poly * poly_new ();

void poly_free    (poly *p);

void poly_add    (poly *p, vertex *v);
void poly_remove (poly *p, vertex *v);
void poly_print  (poly *p);

vertex * poly_nearest (poly *p, int x, int y);
vertex * poly_lowest  (poly *p);
vertex * poly_highest (poly *p);

#endif
