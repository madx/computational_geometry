#ifndef POLYGON_H__
#define POLYGON_H__

#include "util.h"

typedef struct vertex_t vertex;
typedef struct poly_t   poly;

struct vertex_t {
  int     x, y, mark;
  vertex *next, *prev;
};

struct poly_t {
  int     size;
  vertex *hull;
  vertex *last;
  vertex *lo, *hi;
};

/* --------------------- *
 * Functions on vertices *
 * --------------------- */

vertex * vertex_new   (int x, int y);
vertex * vertex_dup   (vertex *v);
vertex * vertex_dup_r (vertex *v);

void vertex_free   (vertex *v);
void vertex_free_r (vertex *v);
void vertex_link   (vertex *v1, vertex *v2);
void vertex_unlink (vertex *v1, vertex *v2);
void vertex_print  (vertex *v);

/* --------------------- *
 * Functions on polygons *
 * --------------------- */

poly * poly_new ();
poly * poly_dup (poly *p);

void poly_free    (poly *p);
void poly_add     (poly *p, vertex *v);
void poly_remove  (poly *p, vertex *v);
void poly_reverse (poly *p);
void poly_print   (poly *p);
void poly_update  (poly *p, vertex *v, int x, int y);
void poly_update_low  (poly *p, vertex *v);
void poly_update_high (poly *p, vertex *v);

vertex * poly_nearest (poly *p, int x, int y);
vertex * poly_lowest  (poly *p);
vertex * poly_highest (poly *p);

vertex **poly_to_array (poly *p);

#endif
