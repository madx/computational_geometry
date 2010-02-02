#include <stdlib.h>
#include <stdio.h>
#include "base.h"
#include "../polygon.h"

bool test_vertex_link ();
bool test_vertex_chain_length ();
bool test_poly_add ();

int main (int argc, char *argv[]) {
  tstatus ts = {0, 0, true};

  unit ("vertex_link :: links two vertices",
        test_vertex_link, &ts);

  unit ("vertex_chain_length :: returns the length of a chain",
        test_vertex_link, &ts);

  unit ("poly_add :: adds a vertex to the hull of a polygon",
        test_poly_add, &ts);

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

bool test_poly_add () {
  bool passed = false;
  vertex *v1, *v2;
  poly   *p;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  p = poly_new (v1);
  poly_add (p, v2);

  passed = (p->vertex_c == 2) && (v1->next == v2);

  poly_free (p);

  return passed;
}
