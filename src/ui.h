#ifndef UI_H_
#define UI_H_

#include <gtk/gtk.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <gdk/gdkkeysyms.h>

#define PIXDIR "/home/madx/dev/L3/geometrie/images/"

typedef struct gui_t {
  gpointer window, vbox, status, draw_zone, box, menu,
           m_split, m_mtr, m_rtr, m_hull, m_quit,
           m_clear, m_draw,
           m_sep1, m_sep2;

  bool ctrl;

  poly   *poly;

  vertex *selected,
         *hovered;

  GdkPixmap *pixmap;
  GdkWindow *canvas;
  GdkGC     *gc;

  int pix_w, pix_h;
  guint cid;
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
void gui_draw_zone_key_press   (GtkWidget *w, GdkEventKey *e, gpointer data);
void gui_draw_zone_key_release (GtkWidget *w, GdkEventKey *e, gpointer data);

void gui_show_menu     (GtkWidget *w, gpointer data);
void gui_menu_item_out (GtkWidget *w, gpointer data);
void gui_menu_item_in  (GtkWidget *w, gpointer data);

void gui_on_split (GtkWidget *w, gpointer data);
void gui_on_mtr   (GtkWidget *w, gpointer data);
void gui_on_rtr   (GtkWidget *w, gpointer data);
void gui_on_hull  (GtkWidget *w, gpointer data);
void gui_on_clear (GtkWidget *w, gpointer data);
void gui_on_draw  (GtkWidget *w, gpointer data);

/* Polygon handling */
void gui_add_vertex (gui *g, int x, int y);

/* Algorithm helpers */
void gui_algorithm_error    (gui *g, char *name, char *msg);
void gui_algorithm_info     (gui *g, char *name, char *msg);
bool gui_algorithm_req_poly (gui *g, char *name);

/* Algorithms */
void gui_split_lr_chains_algorithm      (gui *g);
void gui_triangulate_monotone_algorithm (gui *g);
void gui_triangulate_regular_algorithm  (gui *g);
void gui_graham_convex_hull_algorithm   (gui *g);

void mtr_show_numbers (gui *g, vertex **l);

/* Drawing functions */
void gui_draw_all        (gui *g);
void gui_draw_background (gui *g);
void gui_draw_points     (gui *g);
void gui_draw_point      (gui *g, vertex *v);
#define gui_draw_vertex(g, v, f, o) \
  do {\
    gc_set_fg (g->gc, f);\
    gdk_draw_arc (g->pixmap, g->gc, true, v->x - 3, v->y - 3, 7, 7, 0, 360*64);\
    gc_set_fg (g->gc, o);\
    gdk_draw_arc (g->pixmap, g->gc, false, v->x - 3, v->y - 3, 7, 7, 0, 360*64);\
    gtk_widget_queue_draw_area (g->draw_zone, v->x - 3, v->y - 3, 7, 7);\
  } while (0)
#define gui_draw_edge(g, v, w, c) \
  do {\
    gc_set_fg (g->gc, c);\
    gdk_draw_line (g->pixmap, g->gc, v->x, v->y, w->x, w->y);\
    gtk_widget_queue_draw (g->draw_zone);\
  } while (0)
void gui_draw_edges   (gui *g);

/* GC handling */
void gc_set_fg   (GdkGC *gc, const char *spec);
void gc_set_bg   (GdkGC *gc, const char *spec);

/* UI utils */
void gui_status_push (gui *g, char *message);
void gui_status_pop  (gui *g);
void gui_status_set  (gui *g, char *message);

struct colors_t {
  char *background, *edge, *normal, *hover,
       *first,      *last, *high,   *low,
       *outline;
};

#endif
