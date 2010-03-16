#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "polygon.h"
#include "algorithms.h"
#include "ui.h"

struct colors_t Color = {
  "#eae8e3", "#565248", "#80d67a", "#d6807a",
  "#83694b", "#4b6983", "cyan",    "magenta",
  "#7aabd6", "#88847a"
};

int main (int argc, char *argv[]) {
  gui g;

  gtk_init (&argc, &argv);
  gui_init (&g);

  gtk_widget_show_all (g.window);
  gtk_main ();

  exit (EXIT_SUCCESS);
}

void gui_init (gui *g) {
  g->poly     = poly_new ();
  g->selected = NULL;
  g->hovered  = NULL;
  g->gc       = NULL;
  g->canvas   = NULL;
  g->pixmap   = NULL;
  g->ctrl     = false;
  g->pix_w    = 0;
  g->pix_h    = 0;

  /* Main window */
  g->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (g->window, "[:Polytool:]");

  /* Widget creation */
  g->vbox      = gtk_vbox_new         (false, 0);
  g->box       = gtk_hbox_new         (false, 4);
  g->status    = gtk_statusbar_new    ();
  g->draw_zone = gtk_drawing_area_new ();


  /* Context menu */
  g->menu = gtk_menu_new ();

  g->m_split = gtk_menu_item_new_with_label ("Split left/right chains");
  g->m_mtr   = gtk_menu_item_new_with_label ("Triangulate (monotone)");
  g->m_rtr   = gtk_menu_item_new_with_label ("Triangulate (generic)");
  g->m_hull  = gtk_menu_item_new_with_label ("Convex hull (Graham)");
  g->m_sep1  = gtk_separator_menu_item_new  ();
  g->m_clear = gtk_menu_item_new_with_label ("Clear");
  g->m_draw  = gtk_menu_item_new_with_label ("Redraw");
  g->m_sep2  = gtk_separator_menu_item_new  ();
  g->m_quit  = gtk_menu_item_new_with_label ("Quit");

  gtk_menu_append (g->menu, g->m_split);
  gtk_menu_append (g->menu, g->m_mtr);
  gtk_menu_append (g->menu, g->m_rtr);
  gtk_menu_append (g->menu, g->m_hull);
  gtk_menu_append (g->menu, g->m_sep1);
  gtk_menu_append (g->menu, g->m_clear);
  gtk_menu_append (g->menu, g->m_draw);
  gtk_menu_append (g->menu, g->m_sep2);
  gtk_menu_append (g->menu, g->m_quit);

  gtk_widget_show_all (g->menu);


  /* Drawing area */
  gtk_drawing_area_size (g->draw_zone, 200, 200);
  gtk_widget_set_events (g->draw_zone, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_BUTTON_RELEASE_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK);

  /* Packing */
  gtk_container_set_border_width (g->box, 4);

  gtk_box_pack_start (g->box,    g->draw_zone, true,  true,  0);
  gtk_box_pack_start (g->vbox,   g->box,       true,  true,  0);
  gtk_box_pack_start (g->vbox,   g->status,    false, false, 0);
  gtk_container_add  (g->window, g->vbox);

  g->cid = gtk_statusbar_get_context_id (g->status, "default");

  gui_connect_signals (g);
}

void gui_quit (gpointer data) {
  gtk_main_quit ();
}

void gui_connect_signals (gui *g) {
  g_signal_connect (g->window, "destroy", G_CALLBACK (gui_quit), g);

  g_signal_connect (g->m_split, "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_mtr,   "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_rtr,   "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_hull,  "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_clear, "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_draw,   "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_quit,  "select", G_CALLBACK (gui_menu_item_in), g);

  g_signal_connect (g->m_split, "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_mtr,   "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_rtr,   "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_hull,  "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_clear, "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_draw,  "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_quit,  "deselect", G_CALLBACK (gui_menu_item_out), g);

  g_signal_connect (g->m_quit,  "activate", G_CALLBACK (gui_quit),     g);
  g_signal_connect (g->m_split, "activate", G_CALLBACK (gui_on_split), g);
  g_signal_connect (g->m_mtr,   "activate", G_CALLBACK (gui_on_mtr),   g);
  g_signal_connect (g->m_rtr,   "activate", G_CALLBACK (gui_on_rtr),   g);
  g_signal_connect (g->m_hull,  "activate", G_CALLBACK (gui_on_hull),  g);
  g_signal_connect (g->m_clear, "activate", G_CALLBACK (gui_on_clear), g);
  g_signal_connect (g->m_draw,  "activate", G_CALLBACK (gui_on_draw),   g);

  g_signal_connect (g->draw_zone, "configure_event",
                    G_CALLBACK (gui_draw_zone_configure), g);
  g_signal_connect (g->draw_zone, "expose_event",
                    G_CALLBACK (gui_draw_zone_expose), g);
  g_signal_connect (g->draw_zone, "motion_notify_event",
                    G_CALLBACK (gui_draw_zone_motion_notify), g);
  g_signal_connect (g->draw_zone, "button_press_event",
                    G_CALLBACK (gui_draw_zone_button_press), g);
  g_signal_connect (g->draw_zone, "button_release_event",
                    G_CALLBACK (gui_draw_zone_button_release), g);

  g_signal_connect (g->window, "key_press_event",
                    G_CALLBACK (gui_draw_zone_key_press), g);
  g_signal_connect (g->window, "key_release_event",
                    G_CALLBACK (gui_draw_zone_key_release), g);
}

/*======================*
 *   Polygon handling   *
 *======================*/

void gui_add_vertex (gui *g, int x, int y) {
  poly_add (g->poly, vertex_new (x, y));
  gui_draw_all (g);
}

/*=======================*
 *   Algorithm helpers   *
 *=======================*/

void gui_algorithm_error (gui *g, char *name, char *msg) {
  GString *s = g_string_new (name);

  g_string_append_printf (s, ": [error] %s", msg);
  gui_status_push        (g, s->str);
  g_string_free          (s, false);
}

void gui_algorithm_info (gui *g, char *name, char *msg) {
  GString *s = g_string_new (name);

  g_string_append_printf (s, ": %s", msg);
  gui_status_push        (g, s->str);
  g_string_free          (s, false);
}

bool gui_algorithm_req_poly (gui *g, char *name) {
  if (g->poly->size < 3) {
    gui_algorithm_error (g, name,
                         "requires a valid polygon (at least 3 vertices)");
    return true;
  }

  return false;
}

/*=======================*
 *   Drawing functions   *
 *=======================*/

void gui_draw_all (gui *g) {
  gui_draw_background (g);
  gui_draw_edges      (g);
  gui_draw_points     (g);

  gtk_widget_queue_draw (g->draw_zone);
}

void gui_draw_background (gui *g) {
  gc_set_fg (g->gc, Color.background);
  gdk_draw_rectangle (g->pixmap, g->gc, true, 0, 0, -1, -1);
}

void gui_draw_points (gui *g) {
  vertex *v = g->poly->hull;

  if (v) {
    for (v = g->poly->hull; v != g->poly->last; v = v->next)
      gui_draw_point (g, v);
    gui_draw_point (g, v);
  }
}

void gui_draw_point (gui *g, vertex *v) {
  char *color = (v == g->hovered)    ? Color.hover :
                (v == g->poly->hull) ? Color.first :
                (v == g->poly->last) ? Color.last  :
                                       Color.normal;

  gui_draw_vertex (g, v, color, Color.outline);
}

void gui_draw_edges (gui *g) {
  vertex *v;

  if (g->poly->hull) {
    for (v = g->poly->hull; v != g->poly->last; v = v->next)
      gui_draw_edge (g, v, v->next, Color.edge);
    gui_draw_edge (g, v, v->next, Color.edge);
  }
}

/*=================*
 *   GC handling   *
 *=================*/

void gc_set_fg (GdkGC *gc, const char *spec) {
  GdkColor color;

  gdk_color_parse (spec, &color);
  gdk_gc_set_rgb_fg_color (gc, &color);
}

void gc_set_bg (GdkGC *gc, const char *spec) {
  GdkColor color;

  gdk_color_parse (spec, &color);
  gdk_gc_set_rgb_bg_color (gc, &color);
}

/*==============*
 *   UI utils   *
 *==============*/

void gui_status_push (gui *g, char *message) {
  gtk_statusbar_push (g->status, g->cid, message);
}

void gui_status_pop (gui *g) {
  gtk_statusbar_pop  (g->status, g->cid);
}

void gui_status_set (gui *g, char *message) {
  gui_status_pop  (g);
  gui_status_push (g, message);
}
