#ifndef GLUT_390_H
#define GLUT_390_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_390_reshape (int, int);
void engine_glut_390_key (unsigned char, int, int);
void engine_glut_390_key_special (int, int, int);
void engine_glut_390_menu (int);
void engine_glut_390_mouse_button (int, int, int, int);
void engine_glut_390_mouse_move (int, int);
void engine_glut_390_timer (int);
void engine_glut_390_draw (void);
void engine_glut_390_idle (void);
void engine_glut_390_visible (int);

struct Engine_OpenGL_GLUT_390_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
  GLint                             resolutionLoc;
  GLint                             timeLoc;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_390_H
