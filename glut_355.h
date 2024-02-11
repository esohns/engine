#ifndef GLUT_355_H
#define GLUT_355_H

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

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

// GLUT routines
void engine_glut_355_reshape (int, int);
void engine_glut_355_key (unsigned char, int, int);
void engine_glut_355_key_special (int, int, int);
void engine_glut_355_menu (int);
void engine_glut_355_mouse_button (int, int, int, int);
void engine_glut_355_mouse_move (int, int);
void engine_glut_355_timer (int);
void engine_glut_355_draw (void);
void engine_glut_355_idle (void);
void engine_glut_355_visible (int);

struct Engine_OpenGL_GLUT_355_CBData
{
  // context

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_355_H
