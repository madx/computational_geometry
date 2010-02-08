#include <stdlib.h>
#include <stdio.h>
#include "util.h"
#include "polygon.h"
#include "ui.h"

void gui_init (gui *g) {
  int square[] = {50, 50, 75, 75, 50, 100, 25, 75};
  g->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title (g->window, "Polygones");

  g->vbox   = gtk_vbox_new (false, 0);
  g->hbox   = gtk_hbox_new (false, 5);
  g->status = gtk_statusbar_new ();

  g->run_b   = gtk_button_new_with_label ("Hi/Lo");
  g->clear_b = gtk_button_new_with_label ("Clear");
  g->quit_b  = gtk_button_new_with_label ("Quit");

  g->poly   = poly_new() /* _from_list (square, 4)*/;
  g->select = NULL;
  g->hover  = NULL;

  g->pix_w = 0;
  g->pix_h = 0;
  g->gc        = NULL;
  g->canvas    = NULL;
  g->pixmap    = NULL;
  g->draw_zone = gtk_drawing_area_new ();

  gtk_drawing_area_size (g->draw_zone, 200, 200);
  gtk_widget_set_events (g->draw_zone, GDK_EXPOSURE_MASK
                         | GDK_LEAVE_NOTIFY_MASK
                         | GDK_BUTTON_PRESS_MASK
                         | GDK_BUTTON_RELEASE_MASK
                         | GDK_POINTER_MOTION_MASK
                         | GDK_POINTER_MOTION_HINT_MASK);

  gtk_box_pack_start (g->hbox, g->run_b,   false, false, 0);
  gtk_box_pack_start (g->hbox, g->clear_b, false, false, 0);
  gtk_box_pack_end   (g->hbox, g->quit_b,  false, false, 0);

  gtk_box_pack_start (g->vbox, g->hbox,      false, false, 0);
  gtk_box_pack_start (g->vbox, g->draw_zone, true,  true,  0);
  gtk_box_pack_start (g->vbox, g->status,    false, false, 0);

  gtk_container_add (g->window, g->vbox);

  gui_connect_signals (g);
  gui_status_show (g, "(0,0)");
}

void gui_quit (gpointer data) {
  gtk_main_quit ();
}

void gui_connect_signals (gui *g) {
  g_signal_connect (g->window, "destroy", G_CALLBACK (gui_quit), g);

  g_signal_connect (g->run_b,   "clicked", G_CALLBACK (gui_run_clicked),   g);
  g_signal_connect (g->clear_b, "clicked", G_CALLBACK (gui_clear_clicked), g);
  g_signal_connect (g->quit_b,  "clicked", G_CALLBACK (gui_quit), g);

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
  char msg[32];
  vertex *old;

  if (g->select) {
    g->select->x = e->x;
    g->select->y = e->y;
    gui_draw_all (g);
  } else {
    old      = g->hover;
    g->hover = poly_nearest (g->poly, e->x, e->y);

    if (old && old != g->hover)
      gui_draw_point (g, old);

    if (g->hover) {
      gui_draw_point (g, g->hover);
      sprintf (msg, "(%d,%d) [%p]", g->hover->x, g->hover->y, (void *) g->hover);
    } else {
      sprintf (msg, "(%d,%d)", (int) e->x, (int) e->y);
    }
    gui_status_show (g, msg);
  }
}

void gui_draw_zone_button_press (GtkWidget *w, GdkEventButton *e, gpointer data) {
  gui *g = data;

  if (g->hover) {
    g->select = g->hover;
  } else {
    if (e->button == 1) {
      gui_add_point (g, e->x, e->y);
      g->hover = vertex_last (g->poly->hull);
    }
  }

}

void gui_draw_zone_button_release (GtkWidget *w, GdkEventButton *e, gpointer data) {
  gui *g = data;

  if (g->select) {
    g->select = NULL;
    gui_draw_all (g);
  }
}

/*=============*
 *   Buttons   *
 *=============*/

void gui_run_clicked (GtkWidget *w, gpointer data) {
  gui *g = data;
  /* Découpage en chaînes monotones */
  vertex *low, *high;

  low  = poly_lowest  (g->poly);
  high = poly_highest (g->poly);

  gc_set_fg (g->gc, "red");
  gdk_draw_arc (g->pixmap, g->gc, true,  low->x - 3, low->y - 3, 7, 7, 0, 360*64);
  gtk_widget_queue_draw_area (g->draw_zone, low->x - 3, low->y - 3, 7, 7);

  gc_set_fg (g->gc, "blue");
  gdk_draw_arc (g->pixmap, g->gc, true,  high->x - 3, high->y - 3, 7, 7, 0, 360*64);
  gtk_widget_queue_draw_area (g->draw_zone, high->x - 3, high->y - 3, 7, 7);
}

void gui_clear_clicked (GtkWidget *w, gpointer data) {
  gui *g = data;

  poly_free (g->poly);
  g->poly = poly_new ();

  gui_draw_all (g);
}

/*======================*
 *   Polygon handling   *
 *======================*/

void gui_add_point (gui *g, int x, int y) {
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
  if (v == g->hover)
    gc_set_fg (g->gc, "#d6807a");
  else if (v == vertex_last (g->poly->hull))
    gc_set_fg (g->gc, "#4b6983");
  else
    gc_set_fg (g->gc, "#80d67a");
  gdk_draw_arc (g->pixmap, g->gc, true,  v->x - 3, v->y - 3, 7, 7, 0, 360*64);
  gc_set_fg (g->gc, "#7aabd6");
  gdk_draw_arc (g->pixmap, g->gc, false, v->x - 3, v->y - 3, 7, 7, 0, 360*64);

  gtk_widget_queue_draw_area (g->draw_zone, v->x - 3, v->y - 3, 7, 7);
}

void gui_draw_segments (gui *g) {
  vertex *v = g->poly->hull;

  while (v != NULL) {
    gc_set_fg (g->gc, "#565248");
    if (NULL != v->next)
      gdk_draw_line (g->pixmap, g->gc, v->x, v->y, v->next->x, v->next->y);
    v = v->next;
  }
}

/* void gui_show_hovered (gui *g, int x, int y) { */
/*   vertex *n; */
/*  */
/*   if ((n = poly_nearest (g->poly, x, y))) { */
/*     gc_set_fg (g->gc, "#d6807a"); */
/*     gdk_draw_arc (g->pixmap, g->gc, true, n->x - 3, n->y - 3, 7, 7, 0, 360*64); */
/*   } */
/*  */
/*   gtk_widget_queue_draw (g->draw_zone); */
/* } */

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

void gui_status_show (gui *g, char *message) {
  guint cid = gtk_statusbar_get_context_id (g->status, "default");
  gtk_statusbar_pop  (g->status, cid);
  gtk_statusbar_push (g->status, cid, message);
}
