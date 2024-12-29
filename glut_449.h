#ifndef GLUT_449_H
#define GLUT_449_H

#include <vector>

#include "noise/noise.h"

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#include "common_gl_common.h"

#include "engine_common.h"

#include "defines_20.h"

// GLUT routines
void engine_glut_449_reshape (int, int);
void engine_glut_449_key (unsigned char, int, int);
void engine_glut_449_key_special (int, int, int);
void engine_glut_449_menu (int);
void engine_glut_449_mouse_button (int, int, int, int);
void engine_glut_449_mouse_move (int, int);
void engine_glut_449_timer (int);
void engine_glut_449_draw (void);
void engine_glut_449_idle (void);
void engine_glut_449_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_449_CBData
{
  // menu
  bool                             wireframe;

  // camera
  struct Engine_GL_Camera          camera;

  // mouse
  int                              mousex;
  int                              mousey;
  bool                             mouse_0_ButtonDown;

  // scene
  std::vector<std::vector<float> > gradient;

  // noise
  noise::module::Perlin            noise;
};

//////////////////////////////////////////

Common_GL_Color_t grad (float, const std::vector<std::vector<float> >&);
void polygon (float, float, float, int);

#endif // GLUT_449_H
