#ifndef UI_H_
#define UI_H_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>

typedef struct gui_t {
  gpointer window, vbox, hbox, status, draw_zone;
  gpointer run_b, clear_b, quit_b;

  poly   *poly;

  vertex *select,
         *hover;

  GdkPixmap *pixmap;
  GdkWindow *canvas;
  GdkGC     *gc;

  int pix_w, pix_h;
} gui;

void gui_init (gui *g);
void gui_quit (gpointer data);

void gui_connect_signals (gui *g);

/* Events */
void gui_draw_zone_expose    (GtkWidget *w, GdkEventExpose *e,    gpointer data);
void gui_draw_zone_configure (GtkWidget *w, GdkEventConfigure *e, gpointer data);
void gui_draw_zone_motion_notify  (GtkWidget *w, GdkEventMotion *e, gpointer data);
void gui_draw_zone_button_press   (GtkWidget *w, GdkEventButton *e, gpointer data);
void gui_draw_zone_button_release (GtkWidget *w, GdkEventButton *e, gpointer data);

void gui_run_clicked   (GtkWidget *w, gpointer data);
void gui_clear_clicked (GtkWidget *w, gpointer data);

/* Polygon handling */
void gui_add_point     (gui *g, int x, int y);

/* Drawing functions */
void gui_draw_all        (gui *g);
void gui_draw_background (gui *g);
void gui_draw_points     (gui *g);
void gui_draw_point      (gui *g, vertex *v);
void gui_draw_segments   (gui *g);

/* GC handling */
void gc_set_fg   (GdkGC *gc, const char *spec);
void gc_set_bg   (GdkGC *gc, const char *spec);

/* UI utils */
void gui_status_show (gui *g, char *message);

#endif
