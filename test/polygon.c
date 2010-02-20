#include <stdlib.h>
#include <stdio.h>
#include "base.h"
#include "../src/polygon.h"

void test_vertex_new    (tstatus *ts);
void test_vertex_link   (tstatus *ts);
void test_vertex_unlink (tstatus *ts);
void test_poly_new      (tstatus *ts);
void test_poly_add      (tstatus *ts);
void test_poly_remove   (tstatus *ts);

int main (int argc, char *argv[]) {
  tstatus ts = {0, 0, true};

  unit ("vertex_new",  &ts, test_vertex_new);
  unit ("vertex_link", &ts, test_vertex_link);
  unit ("vertex_link", &ts, test_vertex_unlink);
  unit ("poly_new",    &ts, test_poly_new);
  unit ("poly_add",    &ts, test_poly_add);
  unit ("poly_remove", &ts, test_poly_remove);

  return summary (ts);
}

void test_vertex_new (tstatus *ts) {
  vertex *v;

  v = vertex_new (0, 0);

  check ("initializes x and y", ts,
          (v->x == 0) && (v->y == 0));

  check ("initializes next and prev to NULL", ts,
          (v->next == NULL) && (v->prev == 0));

  vertex_free (v);
}

void test_vertex_link (tstatus *ts) {
  vertex *v1, *v2;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  vertex_link (v1, v2);

  check ("sets the next vertex", ts,
         v1->next == v2);
  check ("sets the prev vertex", ts,
         v2->prev == v1);

  vertex_free (v1);
  vertex_free (v2);
}

void test_vertex_unlink (tstatus *ts) {
  vertex *v1, *v2;

  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  check ("unsets the next vertex", ts,
         v1->next == NULL);
  check ("unsets the prev vertex", ts,
         v2->prev == NULL);

  vertex_link   (v1, v2);
  vertex_unlink (v1, v2);

  vertex_free (v1);
  vertex_free (v2);
}

void test_poly_new (tstatus *ts) {
  poly *p;

  p = poly_new ();

  check ("initializes the size to 0", ts,
         p->size == 0);
  check ("initializes hull and last to NULL", ts,
         p->hull == NULL && p->last == NULL);

  poly_free (p);
}

void test_poly_add (tstatus *ts) {
  poly *p;
  vertex *v1, *v2;

  p  = poly_new ();
  v1 = vertex_new (0, 0);
  v2 = vertex_new (1, 1);

  poly_add (p, v1);

  check ("the first vertex added is what *hull points to", ts,
         p->hull == v1);
  check ("the last vertex added is available with *last", ts,
         p->last == v1);

  poly_add (p, v2);

  check ("when adding more vertices, the previous property holds", ts,
         p->last == v2);

  check ("polygon size increases when adding vertices", ts,
         p->size == 2);

  poly_free (p);
}

void test_poly_remove (tstatus *ts) {
  poly *p;
  vertex *v1, *v2, *v3;

  p  = poly_new ();
  poly_add (p, (v1 = vertex_new (0, 0)));
  poly_add (p, (v2 = vertex_new (1, 1)));
  poly_add (p, (v3 = vertex_new (2, 2)));

  poly_remove (p, v2);
  check ("links previous and next if removing in the middle", ts,
         v1->next == v3 && v3->prev == v1);
  check ("updates the size", ts, p->size == 2);

  poly_remove (p, v1);
  check ("updates hull if removing the first", ts,
         p->hull == v3 && v3->prev == NULL);

  poly_add    (p, v1);
  poly_remove (p, v1);
  check ("updates last if removing the last", ts,
         p->last == v3 && v1->next == NULL);

  poly_remove (p, v3);
  check ("the poly is now empty", ts,
         p->hull == NULL && p->last == NULL && p->size == 0);

  poly_free   (p);
  vertex_free (v1);
  vertex_free (v2);
  vertex_free (v3);
}
