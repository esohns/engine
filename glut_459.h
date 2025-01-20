#ifndef GLUT_459_H
#define GLUT_459_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#include "common_gl_shader.h"

#include "common_ui_gtk_common.h"

#include "engine_common.h"

struct Engine_OpenGL_GLUT_459_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             phaseLoc;
  GLint                             scaleLoc;
  GLint                             octavesLoc;
  GLint                             frequencyLoc;
  GLint                             noiseFactorLoc;
  GLint                             sharpnessLoc;
  GLint                             timeLoc;
  GLint                             patternLoc;

  Common_GL_Shader                  shader;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
  bool                              mouseLMBPressed;

  // noise
#if defined (GLM_SUPPORT)
  glm::vec2                         phase;
#endif // GLM_SUPPORT
  float                             scale;
  int                               octaves;
  float                             frequency;
  float                             noiseFactor;
  float                             sharpness;
  int                               pattern;
};

// GLUT routines
void engine_glut_459_reshape (int, int);
void engine_glut_459_key (unsigned char, int, int);
void engine_glut_459_key_special (int, int, int);
void engine_glut_459_menu (int);
void engine_glut_459_mouse_button (int, int, int, int);
void engine_glut_459_mouse_move (int, int);
void engine_glut_459_timer (int);
void engine_glut_459_draw (void);
void engine_glut_459_idle (void);
void engine_glut_459_visible (int);

//////////////////////////////////////////

struct Engine_UI_GTK_459_CBData
 : Common_UI_GTK_CBData
{
  struct Engine_OpenGL_GLUT_459_CBData* GLUT_CBData;
};

// GTK idle routines
gboolean idle_initialize_UI_459_cb (gpointer);
gboolean idle_finalize_UI_459_cb (gpointer);

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
G_MODULE_EXPORT void scale_noise_scale_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_noise_octaves_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_noise_factor_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_frequency_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_sharpness_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void scale_pattern_value_changed_cb (GtkRange*, gpointer);
G_MODULE_EXPORT void button_reset_459_clicked_cb (GtkButton*, gpointer);
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif // GLUT_459_H
