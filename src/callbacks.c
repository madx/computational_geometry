#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "polygon.h"
#include "ui.h"

/*============*
 *   Events   *
 *============*/

void gui_draw_zone_expose (GtkWidget *w, GdkEventExpose *e, gpointer data) {
  gui *g = data;
  if (e->count != 0) return;

  gdk_draw_pixmap (w->window, g->gc, g->pixmap, e->area.x, e->area.y,
                   e->area.x, e->area.y, e->area.width, e->area.height);
}

void gui_draw_zone_configure (GtkWidget *w, GdkEventConfigure *e, gpointer data) {
  gui *g = data;

  if (g->pixmap)
    gdk_pixmap_unref (g->pixmap);

  if (!g->canvas)
    g->canvas = w->window;

  g->pix_w  = w->allocation.width;
  g->pix_h  = w->allocation.height;
  g->gc     = gdk_gc_new (w->window);
  g->pixmap = gdk_pixmap_new (g->canvas, g->pix_w, g->pix_h, -1);

  gui_draw_all (g);
}

void gui_draw_zone_motion_notify (GtkWidget *w, GdkEventMotion *e, gpointer data) {
  gui *g = data;
  vertex *old;

  if (g->selected) {
    poly_update (g->poly, g->selected, e->x, e->y);
    gui_draw_all (g);
  } else {
    old      = g->hovered;
    g->hovered = poly_nearest (g->poly, e->x, e->y);

    if (old && old != g->hovered)
      gui_draw_point (g, old);

    if (g->hovered) {
      gui_draw_point (g, g->hovered);
    }
  }
}

void gui_draw_zone_button_press (GtkWidget *w, GdkEventButton *e, gpointer data) {
  gui *g = data;

  switch (e->button) {
  case 1:
    if (g->hovered) {
      if (g->ctrl) {
        poly_remove (g->poly, g->hovered);
        g->hovered = NULL;
        gui_draw_all (g);
      } else g->selected = g->hovered;
    } else gui_add_vertex (g, e->x, e->y);
    break;

  case 3:
    gtk_menu_popup (g->menu, NULL, NULL, NULL, NULL, 1,
                    gtk_get_current_event_time ());
  default:
    break;
  }
}

void gui_draw_zone_button_release (GtkWidget *w, GdkEventButton *e, gpointer data) {
  gui *g = data;

  g->selected = NULL;
}

void gui_draw_zone_key_press (GtkWidget *w, GdkEventKey *e, gpointer data) {
  gui *g = data;

  if (e->keyval == GDK_Control_L) {
    gui_status_push (g, "Ctrl: click on a vertex to remove it");
    g->ctrl = true;
  }
}

void gui_draw_zone_key_release (GtkWidget *w, GdkEventKey *e, gpointer data) {
  gui *g = data;

  gui_status_pop (g);
  g->ctrl = false;
}

void gui_show_menu (GtkWidget *w, gpointer data) {
  gui *g = data;

  gtk_menu_popup (g->menu, NULL, NULL, NULL, NULL, 1,
                  gtk_get_current_event_time ());
}

void gui_menu_item_out (GtkWidget *w, gpointer data) {
  gui *g = data;

  gui_status_pop (g);
}

void gui_menu_item_in (GtkWidget *w, gpointer data) {
  gui *g = data;

  gui_status_set (g,
      (g->m_split == w) ? "Split left and right chains"             :
      (g->m_mtr   == w) ? "Triangulate a monotone polygon"          :
      (g->m_rtr   == w) ? "Triangulate a simple polygon"            :
      (g->m_hull  == w) ? "Compute convex hull using Graham's Scan" :
      (g->m_clear == w) ? "Clear the drawing area"                  :
      (g->m_draw  == w) ? "Redraw the polygon"                      :
      (g->m_quit  == w) ? "Quit polytool"                           : "");
}

void gui_on_split (GtkWidget *w, gpointer data) {
  gui *g = data;

  if (gui_algorithm_req_poly (g, "Split")) return;

  gui_split_lr_chains_algorithm (g);
}

void gui_on_mtr   (GtkWidget *w, gpointer data) {
  gui *g = data;

  if (gui_algorithm_req_poly (g, "MTR")) return;

  gui_triangulate_monotone_algorithm (g);
}

void gui_on_rtr   (GtkWidget *w, gpointer data) {
  gui *g = data;

  if (gui_algorithm_req_poly (g, "RTR")) return;
  gui_triangulate_regular_algorithm (g);
}

void gui_on_hull  (GtkWidget *w, gpointer data) {
  gui *g = data;

  if (gui_algorithm_req_poly (g, "Graham")) return;

  gui_graham_convex_hull_algorithm (g);
}

void gui_on_clear (GtkWidget *w, gpointer data) {
  gui *g = data;

  poly_free (g->poly);
  g->poly = poly_new ();
  gui_draw_all (g);
}

void gui_on_draw (GtkWidget *w, gpointer data) {
  gui *g = data;

  gui_draw_all (g);
}

