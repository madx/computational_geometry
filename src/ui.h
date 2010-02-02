#ifndef UI_H_
#define UI_H_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

typedef struct gui_t {
  gpointer window, vbox, status, draw_zone;

  GdkPixmap *pixmap;
  GdkWindow *canvas;
  GdkGC     *gc;
} gui;

void gui_init (gui *g);
void gui_connect_signals (gui *g);

/* Events */
void gui_draw_zone_expose (GdkDrawable *d, GdkEventExpose *e, gpointer data);

void gui_set_color(GdkGC *gc, guint16 red, guint16 green, guint16 blue);

#endif
