#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "polygon.h"
#include "ui.h"

int main (int argc, char *argv[]) {
  gui g;

  gtk_init (&argc, &argv);
  gui_init (&g);

  gtk_widget_show_all (g.window);
  gtk_main ();

  exit (EXIT_SUCCESS);
}
