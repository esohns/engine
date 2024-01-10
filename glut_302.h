#ifndef GLUT_302_H
#define GLUT_302_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_302_reshape (int, int);
void engine_glut_302_key (unsigned char, int, int);
void engine_glut_302_key_special (int, int, int);
void engine_glut_302_menu (int);
void engine_glut_302_mouse_button (int, int, int, int);
void engine_glut_302_mouse_move (int, int);
void engine_glut_302_timer (int);
void engine_glut_302_draw (void);
void engine_glut_302_idle (void);
void engine_glut_302_visible (int);

struct Engine_OpenGL_GLUT_302_CBData
{
  //

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   mouseLoc;
  GLint                   timeLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_302_H
