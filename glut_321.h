#ifndef GLUT_321_H
#define GLUT_321_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "glm/glm.hpp"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_321_reshape (int, int);
void engine_glut_321_key (unsigned char, int, int);
void engine_glut_321_key_special (int, int, int);
void engine_glut_321_menu (int);
void engine_glut_321_mouse_button (int, int, int, int);
void engine_glut_321_mouse_move (int, int);
void engine_glut_321_timer (int);
void engine_glut_321_draw (void);
void engine_glut_321_idle (void);
void engine_glut_321_visible (int);

struct Engine_OpenGL_GLUT_321_CBData
{
  //

  // canvas
  int                        columns;
  int                        rows;
  int                        scaleFactor;

  // shader
  GLint                      programId;
  GLint                      resolutionLoc;
  GLint                      timeLoc;
  GLint                      mouseLoc;

  // menu
  bool                       wireframe;

  // camera
  struct Common_GL_Camera    camera;

  // mouse
  int                        mouseX;
  int                        mouseY;
};

#endif // GLUT_321_H
