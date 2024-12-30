#ifndef GLUT_450_H
#define GLUT_450_H

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

#include "defines_21.h"

// GLUT routines
void engine_glut_450_reshape (int, int);
void engine_glut_450_key (unsigned char, int, int);
void engine_glut_450_key_special (int, int, int);
void engine_glut_450_menu (int);
void engine_glut_450_mouse_button (int, int, int, int);
void engine_glut_450_mouse_move (int, int);
void engine_glut_450_timer (int);
void engine_glut_450_draw (void);
void engine_glut_450_idle (void);
void engine_glut_450_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_450_CBData
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
  float                            f;
};

#endif // GLUT_450_H
