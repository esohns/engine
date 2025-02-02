#ifndef GLUT_469_H
#define GLUT_469_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_469_reshape (int, int);
void engine_glut_469_key (unsigned char, int, int);
void engine_glut_469_key_special (int, int, int);
void engine_glut_469_menu (int);
void engine_glut_469_mouse_button (int, int, int, int);
void engine_glut_469_mouse_move (int, int);
void engine_glut_469_timer (int);
void engine_glut_469_draw (void);
void engine_glut_469_idle (void);
void engine_glut_469_visible (int);

struct Engine_OpenGL_GLUT_469_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  Common_GL_Shader                  shader;
  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             mouseLoc;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
  bool                              mouseLMBPressed;
};

#endif // GLUT_469_H
