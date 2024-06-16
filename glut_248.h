#ifndef GLUT_248_H
#define GLUT_248_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_248_reshape (int, int);
void engine_glut_248_key (unsigned char, int, int);
void engine_glut_248_key_special (int, int, int);
void engine_glut_248_menu (int);
void engine_glut_248_mouse_button (int, int, int, int);
void engine_glut_248_mouse_move (int, int);
void engine_glut_248_timer (int);
void engine_glut_248_draw (void);
void engine_glut_248_idle (void);
void engine_glut_248_visible (int);

struct Engine_OpenGL_GLUT_248_CBData
{
  //

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   timeLoc;
  GLint                   mouseLocX;
  GLint                   mouseLocY;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_248_H
