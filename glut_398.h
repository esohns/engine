#ifndef GLUT_398_H
#define GLUT_398_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_398_reshape (int, int);
void engine_glut_398_key (unsigned char, int, int);
void engine_glut_398_key_special (int, int, int);
void engine_glut_398_menu (int);
void engine_glut_398_mouse_button (int, int, int, int);
void engine_glut_398_mouse_move (int, int);
void engine_glut_398_timer (int);
void engine_glut_398_draw (void);
void engine_glut_398_idle (void);
void engine_glut_398_visible (int);

struct Engine_OpenGL_GLUT_398_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             mouseLoc;

  Common_GL_Shader                  shader;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_392_H
