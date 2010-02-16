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

void gui_init (gui *g) {
  g->poly     = poly_new ();
  g->selected = NULL;
  g->hovered  = NULL;
  g->pix_w    = 0;
  g->pix_h    = 0;
  g->gc       = NULL;
  g->canvas   = NULL;
  g->pixmap   = NULL;

  /* Main window */
  g->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (g->window, "[:Polytool:]");

  /* Widgets creation */
  g->vbox      = gtk_vbox_new         (false, 0);
  g->box       = gtk_hbox_new         (false, 4);
  g->status    = gtk_statusbar_new    ();
  g->draw_zone = gtk_drawing_area_new ();

  /* Context menu */
  g->menu = gtk_menu_new ();
  g->m_split = gtk_menu_item_new_with_label ("Split monotone chains");
  g->m_mtr   = gtk_menu_item_new_with_label ("Triangulate (monotone)");
  g->m_rtr   = gtk_menu_item_new_with_label ("Triangulate (generic)");
  g->m_hull  = gtk_menu_item_new_with_label ("Convex hull (Graham)");
  g->m_sep1  = gtk_separator_menu_item_new  ();
  g->m_clear = gtk_menu_item_new_with_label ("Clear");
  g->m_fit   = gtk_menu_item_new_with_label ("Auto-fit");
  g->m_sep2  = gtk_separator_menu_item_new  ();
  g->m_quit  = gtk_menu_item_new_with_label ("Quit");
  gtk_menu_append (g->menu, g->m_split);
  gtk_menu_append (g->menu, g->m_mtr);
  gtk_menu_append (g->menu, g->m_rtr);
  gtk_menu_append (g->menu, g->m_hull);
  gtk_menu_append (g->menu, g->m_sep1);
  gtk_menu_append (g->menu, g->m_clear);
  gtk_menu_append (g->menu, g->m_fit);
  gtk_menu_append (g->menu, g->m_sep2);
  gtk_menu_append (g->menu, g->m_quit);
  gtk_widget_show_all (g->menu);


  gtk_drawing_area_size (g->draw_zone, 200, 200);
  gtk_widget_set_events (g->draw_zone, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_BUTTON_RELEASE_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK);

  /* Packing */
  gtk_box_pack_start (g->box, g->draw_zone, true,  true,  0);

  gtk_container_set_border_width (g->box, 4);
  gtk_box_pack_start (g->vbox, g->box, true, true, 0);
  gtk_box_pack_start (g->vbox, g->status, false, false, 0);

  g->cid = gtk_statusbar_get_context_id (g->status, "default");

  gtk_container_add (g->window, g->vbox);

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
  g_signal_connect (g->m_fit,   "select", G_CALLBACK (gui_menu_item_in), g);
  g_signal_connect (g->m_quit,  "select", G_CALLBACK (gui_menu_item_in), g);

  g_signal_connect (g->m_split, "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_mtr,   "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_rtr,   "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_hull,  "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_clear, "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_fit,   "deselect", G_CALLBACK (gui_menu_item_out), g);
  g_signal_connect (g->m_quit,  "deselect", G_CALLBACK (gui_menu_item_out), g);

  g_signal_connect (g->m_quit,  "activate", G_CALLBACK (gui_quit),     g);
  g_signal_connect (g->m_split, "activate", G_CALLBACK (gui_on_split), g);
  g_signal_connect (g->m_mtr,   "activate", G_CALLBACK (gui_on_mtr),   g);
  g_signal_connect (g->m_rtr,   "activate", G_CALLBACK (gui_on_rtr),   g);
  g_signal_connect (g->m_hull,  "activate", G_CALLBACK (gui_on_hull),  g);
  g_signal_connect (g->m_clear, "activate", G_CALLBACK (gui_on_clear), g);
  g_signal_connect (g->m_fit,   "activate", G_CALLBACK (gui_on_fit),   g);

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

}

/*======================*
 *   Polygon handling   *
 *======================*/

void gui_add_vertex (gui *g, int x, int y) {
  poly_add (g->poly, vertex_new (x, y));
  gui_draw_all (g);
}


/*=======================*
 *   Drawing functions   *
 *=======================*/

void gui_draw_all (gui *g) {
  gui_draw_background (g);
  gui_draw_segments   (g);
  gui_draw_points     (g);

  gtk_widget_queue_draw (g->draw_zone);
}

void gui_draw_background (gui *g) {
  gc_set_fg (g->gc, "#eae8e3");
  gdk_draw_rectangle (g->pixmap, g->gc, true, 0, 0, -1, -1);
}

void gui_draw_points (gui *g) {
  vertex *v = g->poly->hull;

  for (v = g->poly->hull; v != NULL; v = v->next)
    gui_draw_point (g, v);
}

void gui_draw_point (gui *g, vertex *v) {
  char *color = ((v == g->hovered)                  ? "#d6807a" :
                 (v == vertex_last (g->poly->hull)) ? "#4b6983" :
                                                      "#80d67a");

  gui_draw_vertex (g, v, color,     true);
  gui_draw_vertex (g, v, "#7aabd6", false);

  gtk_widget_queue_draw_area (g->draw_zone, v->x - 3, v->y - 3, 7, 7);
}

void gui_draw_segments (gui *g) {
  vertex *v = g->poly->hull;

  while (v != NULL) {
    gc_set_fg (g->gc, "#565248");
    if (NULL != v->next)
      gdk_draw_line (g->pixmap, g->gc, v->x, v->y, v->next->x, v->next->y);
    else
      gdk_draw_line (g->pixmap, g->gc, v->x, v->y, g->poly->hull->x, g->poly->hull->y);
    v = v->next;
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
