#ifndef GLUT_239_H
#define GLUT_239_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_239_reshape (int, int);
void engine_glut_239_key (unsigned char, int, int);
void engine_glut_239_key_special (int, int, int);
void engine_glut_239_menu (int);
void engine_glut_239_mouse_button (int, int, int, int);
void engine_glut_239_mouse_move (int, int);
void engine_glut_239_timer (int);
void engine_glut_239_draw (void);
void engine_glut_239_idle (void);
void engine_glut_239_visible (int);

struct Engine_OpenGL_GLUT_239_CBData
{
  //
  float                   theta;

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   timeLoc;
  GLint                   mouseLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_228_H
