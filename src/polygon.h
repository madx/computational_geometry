#ifndef POLYGON_H__
#define POLYGON_H__

#include "util.h"

typedef struct vertex_t vertex;
typedef struct poly_t   poly;

struct vertex_t {
  int     x, y;
  vertex *next;
};

struct poly_t {
  int     vertex_c;
  vertex *hull;
};

/* --------------------- *
 * Functions on vertices *
 * --------------------- */

vertex * vertex_new   (int x, int y);
vertex * vertex_dup   (vertex *v);
vertex * vertex_rdup  (vertex *v);
vertex * vertex_last  (vertex *v);

void vertex_free  (vertex *v);
void vertex_rfree (vertex *v);   /* Recursive free */
void vertex_link  (vertex *v1, vertex *v2);

vertex * vertex_chain_reverse (vertex *v);
int vertex_chain_length (vertex *v);

/* --------------------- *
 * Functions on polygons *
 * --------------------- */

poly * poly_new  ();
poly * poly_dup  (poly *p);
poly * poly_from_list (int *list, int n);

void poly_free   (poly *p);
void poly_add    (poly *p, vertex *v);
void poly_remove (poly *p, vertex *v);

vertex * poly_nearest (poly *p, int x, int y);
vertex * poly_before  (poly *p, vertex *v);
vertex * poly_after   (poly *p, vertex *v);
vertex * poly_lowest  (poly *p);
vertex * poly_highest (poly *p);

#endif
