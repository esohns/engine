#ifndef GLUT_371_H
#define GLUT_371_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_371_reshape (int, int);
void engine_glut_371_key (unsigned char, int, int);
void engine_glut_371_key_special (int, int, int);
void engine_glut_371_menu (int);
void engine_glut_371_mouse_button (int, int, int, int);
void engine_glut_371_mouse_move (int, int);
void engine_glut_371_timer (int);
void engine_glut_371_draw (void);
void engine_glut_371_idle (void);
void engine_glut_371_visible (int);

struct Engine_OpenGL_GLUT_371_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
  GLint                             resolutionLoc;
  GLint                             framesLoc;
  GLint                             mouseLoc;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_371_H
