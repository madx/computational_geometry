#include <stdlib.h>
#include <stdio.h>
#include "base.h"
#include "../polygon.h"

bool test_vertex_link         ();
bool test_vertex_chain_length ();
bool test_vertex_last         ();
bool test_poly_add            ();
bool test_poly_from_list      ();
bool test_poly_bounds         ();

int main (int argc, char *argv[]) {
  tstatus ts = {0, 0, true};

  unit ("vertex_link :: links two vertices",
        test_vertex_link, &ts);

  unit ("vertex_chain_length :: returns the length of a chain",
        test_vertex_chain_length, &ts);

  unit ("vertex_last :: returns the last vertex of a chain",
        test_vertex_last, &ts);

  unit ("poly_add :: adds a vertex to the hull of a polygon",
        test_poly_add, &ts);

  unit ("poly_from_list :: builds a polygon from a list",
        test_poly_from_list, &ts);

  unit ("poly_(lowest|highest) :: returns the right points",
        test_poly_bounds, &ts);

  return summary (ts);
}

bool test_vertex_link () {
  bool passed = false;
  vertex *v1, *v2;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (0, 0);

  vertex_link (v1, v2);

  passed = (v1->next == v2);

  vertex_rfree (v1);

  return passed;
}

bool test_vertex_chain_length () {
  bool passed = false;
  vertex *v1, *v2;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  vertex_link (v1, v2);

  passed = (vertex_chain_length (v1) == 2);

  vertex_rfree (v1);

  return passed;
}

bool test_vertex_last () {
  bool passed = false;
  vertex *v1, *v2;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  vertex_link (v1, v2);

  passed = (vertex_last (v1) == v2);

  vertex_rfree (v1);

  return passed;
}

bool test_poly_add () {
  bool passed = false;
  vertex *v1, *v2;
  poly   *p;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  p = poly_new ();
  poly_add (p, v1);

  passed = (p->vertex_c == 1) && (p->hull == v1);

  poly_add (p, v2);

  passed &= (p->vertex_c == 2) && (v1->next == v2);

  poly_free (p);

  return passed;
}

bool test_poly_from_list () {
  bool  passed;
  poly *p;
  int  square[] = {0, 0, 1, 0, 1, 1, 0, 1};

  p = poly_from_list (square, 4);

  passed  = vertex_chain_length (p->hull) == 4;
  passed &= p->hull->x == 0 && p->hull->y == 0;
  passed &= vertex_last (p->hull)->y == 1;

  poly_free (p);

  return passed;
}

bool test_poly_bounds () {
  bool  passed;
  int   square[] = {50, 50, 75, 75, 50, 100, 25, 75};
  poly *p;

  p = poly_from_list (square, 4);

  passed  = poly_lowest  (p) == p->hull;
  passed &= poly_highest (p) == p->hull->next->next;

  return passed;
}
