#ifndef GLUT_16_H
#define GLUT_16_H

#include "common_gl_common.h"

#include "common_image_common.h"

#include "common_ui_gtk_common.h"

struct Engine_OpenGL_GLUT_16_CBData
{
  bool                      wireframe;
  bool                      color;

  // camera
  struct Common_GL_Camera   camera;

  // mouse
  bool                      spinning;
  float                     xangle, yangle;
  float                     xdeltaAngle, ydeltaAngle;
  int                       xOrigin, yOrigin;

  // shape parameters
  int                       total;
  int                       radius;
  glm::vec3*                globe;
  float                     m;
  float                     n1, n2, n3;
  float                     a, b;
};

// GLUT routines
void engine_glut_16_key_special (int, int, int);
void engine_glut_16_menu (int);
void engine_glut_16_mouse_button (int, int, int, int);
void engine_glut_16_mouse_move (int, int);
void engine_glut_16_timer (int);
void engine_glut_16_draw (void);

//////////////////////////////////////////

struct Engine_UI_GTK_16_CBData
 : Common_UI_GTK_CBData
{
  struct Engine_OpenGL_GLUT_16_CBData* GLUT_CBData;
};

// GTK idle routines
gboolean idle_initialize_UI_16_cb (gpointer);
gboolean idle_finalize_UI_16_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_m_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_n1_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_n2_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_n3_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_16_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // GLUT_16_H
