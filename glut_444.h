#ifndef GLUT_444_H
#define GLUT_444_H

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
void engine_glut_444_reshape (int, int);
void engine_glut_444_key (unsigned char, int, int);
void engine_glut_444_key_special (int, int, int);
void engine_glut_444_menu (int);
void engine_glut_444_mouse_button (int, int, int, int);
void engine_glut_444_mouse_move (int, int);
void engine_glut_444_timer (int);
void engine_glut_444_draw (void);
void engine_glut_444_idle (void);
void engine_glut_444_visible (int);

struct Engine_OpenGL_GLUT_444_CBData
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
};

#endif // GLUT_444_H
