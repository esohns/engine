#ifndef GLUT_377_H
#define GLUT_377_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_377_reshape (int, int);
void engine_glut_377_key (unsigned char, int, int);
void engine_glut_377_key_special (int, int, int);
void engine_glut_377_menu (int);
void engine_glut_377_mouse_button (int, int, int, int);
void engine_glut_377_mouse_move (int, int);
void engine_glut_377_timer (int);
void engine_glut_377_draw (void);
void engine_glut_377_idle (void);
void engine_glut_377_visible (int);

struct Engine_OpenGL_GLUT_377_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
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
};

#endif // GLUT_377_H
