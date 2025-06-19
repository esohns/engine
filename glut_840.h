#ifndef GLUT_840_H
#define GLUT_840_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"
#include "common_gl_texture.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_840_reshape (int, int);
void engine_glut_840_key (unsigned char, int, int);
void engine_glut_840_key_up (unsigned char, int, int);
void engine_glut_840_key_special (int, int, int);
void engine_glut_840_key_special_up (int, int, int);
void engine_glut_840_menu (int);
void engine_glut_840_mouse_button (int, int, int, int);
void engine_glut_840_mouse_move (int, int);
void engine_glut_840_timer (int);
void engine_glut_840_draw (void);
void engine_glut_840_idle (void);
void engine_glut_840_visible (int);

struct Engine_OpenGL_GLUT_840_CBData
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
  GLint                             channel0Loc;

  Common_GL_Texture                 texture0;

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

#endif // GLUT_840_H
