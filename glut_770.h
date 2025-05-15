#ifndef GLUT_770_H
#define GLUT_770_H

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
void engine_glut_770_reshape (int, int);
void engine_glut_770_key (unsigned char, int, int);
void engine_glut_770_key_up (unsigned char, int, int);
void engine_glut_770_key_special (int, int, int);
void engine_glut_770_key_special_up (int, int, int);
void engine_glut_770_menu (int);
void engine_glut_770_mouse_button (int, int, int, int);
void engine_glut_770_mouse_move (int, int);
void engine_glut_770_timer (int);
void engine_glut_770_draw (void);
void engine_glut_770_idle (void);
void engine_glut_770_visible (int);

struct Engine_OpenGL_GLUT_770_CBData
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

#endif // GLUT_770_H
