#ifndef GLUT_441_H
#define GLUT_441_H

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
void engine_glut_441_reshape (int, int);
void engine_glut_441_key (unsigned char, int, int);
void engine_glut_441_key_special (int, int, int);
void engine_glut_441_menu (int);
void engine_glut_441_mouse_button (int, int, int, int);
void engine_glut_441_mouse_move (int, int);
void engine_glut_441_timer (int);
void engine_glut_441_draw (void);
void engine_glut_441_idle (void);
void engine_glut_441_visible (int);

struct Engine_OpenGL_GLUT_441_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             frameLoc;
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

#endif // GLUT_441_H
