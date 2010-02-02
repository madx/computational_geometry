#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "polygon.h"
#include "ui.h"

void gui_init (gui *g) {
  g->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (g->window, "Polygones");

  g->vbox   = gtk_vbox_new (false, 0);
  g->status = gtk_statusbar_new ();

  g->pixmap = NULL;
  g->canvas = NULL;
  g->gc     = NULL;

  g->draw_zone = gtk_drawing_area_new ();

  gtk_box_pack_start (g->vbox, g->draw_zone, false, false, 5);
  gtk_box_pack_start (g->vbox, g->status,    false, false, 5);

  gtk_container_add (g->window, g->vbox);

  gui_connect_signals (g);
}

void gui_connect_signals (gui *g) {
  g_signal_connect (g->window, "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);

  g_signal_connect (g->draw_zone, "expose_event",
                    G_CALLBACK (gui_draw_zone_expose), g);
}

void gui_draw_zone_expose (GdkDrawable *d, GdkEventExpose *e, gpointer data) {
  gui *g = data;
  if (e->count != 0) return;

  if (!g->canvas) {
    g->canvas = GTK_WIDGET (g->draw_zone)->window;
    g->gc   = gdk_gc_new (g->canvas);
  }

  if (!g->pixmap) {
    g->pixmap = gdk_pixmap_new (g->canvas, 500, 500, -1);
  }

  gui_set_color (g->gc, 0xFFFF, 0xFFFF, 0xFFFF);

  gdk_draw_rectangle (g->pixmap, g->gc, true, 0, 0, -1, -1);
}

void gui_set_color(GdkGC *gc, guint16 red, guint16 green, guint16 blue) {
  GdkColor color;
  color.red   = red;
  color.green = green;
  color.blue  = blue;

  gdk_gc_set_rgb_fg_color (gc, &color);
}
