#ifndef GLUT_277_H
#define GLUT_277_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_277_reshape (int, int);
void engine_glut_277_key (unsigned char, int, int);
void engine_glut_277_key_special (int, int, int);
void engine_glut_277_menu (int);
void engine_glut_277_mouse_button (int, int, int, int);
void engine_glut_277_mouse_move (int, int);
void engine_glut_277_timer (int);
void engine_glut_277_draw (void);
void engine_glut_277_idle (void);
void engine_glut_277_visible (int);

struct Engine_OpenGL_GLUT_277_CBData
{
  //

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   pixelDensityLoc;
  GLint                   canvasLoc;
  GLint                   mouseLoc;
  GLint                   timeLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_277_H
