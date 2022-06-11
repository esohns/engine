#ifndef GLUT_3_H
#define GLUT_3_H

#include "noise/noise.h"

#include "gtk/gtk.h"

#include "common_gl_common.h"

#include "common_image_common.h"

#include "common_ui_gtk_common.h"

enum Engine_GLUTMode
{
  ENGINE_GLUT_MODE_WIREFRAME = 0,
  ENGINE_GLUT_MODE_COLOR,
  //////////////////////
  ENGINE_GLUT_MODE_INVALID,
  ENGINE_GLUT_MODE_MAX,
};

// GLUT routines
void engine_glut_3_reshape (int, int);
void engine_glut_3_key (unsigned char, int, int);
void engine_glut_3_key_special (int, int, int);
void engine_glut_3_menu (int);
void engine_glut_3_mouse_button (int, int, int, int);
void engine_glut_3_mouse_move (int, int);
void engine_glut_3_timer (int);
void engine_glut_3_draw (void);
void engine_glut_3_idle (void);
void engine_glut_3_visible (int);

// GTK idle routines
gboolean idle_initialize_UI_cb (gpointer);
gboolean idle_finalize_UI_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_frequency_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_octaves_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_persistence_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_lacunarity_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_step_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_level_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_speed_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

struct Engine_OpenGL_GLUT_3_CBData
{
  int                       columns;
  int                       rows;
  int                       scaleFactor;
  Common_Image_Resolution_t size;
  bool                      wireframe;
  bool                      color;

  // camera
  struct Common_GL_Camera   camera;

  // mouse
  float                     angle;
  float                     deltaAngle;
  int                       xOrigin;

  // noise
  double                    x;
  double                    y;
  double                    z;
  double                    step;
  noise::module::Perlin     module;

  // terrain
  double                    level;
  float*                    terrain;
  double                    yOffset; // 'moving' along y
};

struct Engine_UI_GTK_CBData
 : Common_UI_GTK_CBData
{
  struct Engine_OpenGL_GLUT_3_CBData* GLUT_CBData;
};

#endif // GLUT_3_H
