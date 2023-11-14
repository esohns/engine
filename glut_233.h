#ifndef GLUT_233_H
#define GLUT_233_H

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_233_reshape (int, int);
void engine_glut_233_key (unsigned char, int, int);
void engine_glut_233_key_special (int, int, int);
void engine_glut_233_menu (int);
void engine_glut_233_mouse_button (int, int, int, int);
void engine_glut_233_mouse_move (int, int);
void engine_glut_233_timer (int);
void engine_glut_233_draw (void);
void engine_glut_233_idle (void);
void engine_glut_233_visible (int);

struct Engine_OpenGL_GLUT_233_CBData
{
  //
  float                   f;
  int                     n;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

//////////////////////////////////////////

void rail (float);

#endif // GLUT_232_H
