#ifndef ALGORITHMS_H__
#define ALGORITHMS_H__

#include "util.h"
#include "polygon.h"

typedef enum { CCW, CW } direction;
typedef enum { CHAIN_RIGHT, CHAIN_LEFT } chain_side;

direction poly_get_direction (poly *p);
double    poly_signed_area   (poly *p);
bool      poly_is_convex     (poly *p);

vertex * poly_chain_left (poly *p);
vertex * poly_chain_right (poly *p);

void vertex_merge_lists    (vertex **m, vertex *l, vertex *r);
bool vertex_chain_monotone (vertex *v1);
int  vertex_cross          (vertex *v1, vertex *v2, vertex *v3);

int graham_sort_points (const void *p1, const void *p2);

#endif
