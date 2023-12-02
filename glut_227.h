#ifndef GLUT_227_H
#define GLUT_227_H

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_227_reshape (int, int);
void engine_glut_227_key (unsigned char, int, int);
void engine_glut_227_key_special (int, int, int);
void engine_glut_227_menu (int);
void engine_glut_227_mouse_button (int, int, int, int);
void engine_glut_227_mouse_move (int, int);
void engine_glut_227_timer (int);
void engine_glut_227_draw (void);
void engine_glut_227_idle (void);
void engine_glut_227_visible (int);

struct Engine_OpenGL_GLUT_227_CBData
{
  // 
  float                   angleX;
  float                   angleY;
  float                   angleZ;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_227_H
