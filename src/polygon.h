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
void     vertex_free  (vertex *v);
void     vertex_rfree (vertex *v);   /* Recursive free */

/* link :: links two vertices */
void   vertex_link    (vertex *v1, vertex *v2);

/* chain_length :: returns the length of a vertex chain */
int vertex_chain_length (vertex *v);


/* --------------------- *
 * Functions on polygons *
 * --------------------- */

poly * poly_new  (vertex *hull);
void   poly_free (poly *p);

/* poly_add :: adds a vertex to the hull of a polygon */
void poly_add (poly *p, vertex *v);

#endif
