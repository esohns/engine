#ifndef GLUT_439_H
#define GLUT_439_H

#include <vector>

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
void engine_glut_439_reshape (int, int);
void engine_glut_439_key (unsigned char, int, int);
void engine_glut_439_key_special (int, int, int);
void engine_glut_439_menu (int);
void engine_glut_439_mouse_button (int, int, int, int);
void engine_glut_439_mouse_move (int, int);
void engine_glut_439_timer (int);
void engine_glut_439_draw (void);
void engine_glut_439_idle (void);
void engine_glut_439_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_439_CBData
{
  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mousex;
  int                     mousey;
  bool                    mouse_0_ButtonDown;

  // scene
  float                   xm[ENGINE_GLUT_439_DEFAULT_M];
  float                   ym[ENGINE_GLUT_439_DEFAULT_M];
  float                   sz[ENGINE_GLUT_439_DEFAULT_M];
  float                   random[ENGINE_GLUT_439_DEFAULT_M];
  Common_GL_Color_t       color[ENGINE_GLUT_439_DEFAULT_M];
  int                     o[ENGINE_GLUT_439_DEFAULT_M];
  int                     top;
  float                   cf;
};

//////////////////////////////////////////

void quader (float, float, float, float, float);

#endif // GLUT_439_H
