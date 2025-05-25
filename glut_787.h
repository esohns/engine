#ifndef GLUT_787_H
#define GLUT_787_H

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
void engine_glut_787_reshape (int, int);
void engine_glut_787_key (unsigned char, int, int);
void engine_glut_787_key_up (unsigned char, int, int);
void engine_glut_787_key_special (int, int, int);
void engine_glut_787_key_special_up (int, int, int);
void engine_glut_787_menu (int);
void engine_glut_787_mouse_button (int, int, int, int);
void engine_glut_787_mouse_move (int, int);
void engine_glut_787_timer (int);
void engine_glut_787_draw (void);
void engine_glut_787_idle (void);
void engine_glut_787_visible (int);

struct Engine_OpenGL_GLUT_787_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  Common_GL_Shader                  shader;

  GLint                             resolutionLoc;
  GLint                             timeLoc;

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

  // mouse / keyboard
  int                               mouseX;
  int                               mouseY;
  bool                              mouseLMBPressed;
};

#endif // GLUT_787_H
