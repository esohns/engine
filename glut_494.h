#ifndef GLUT_494_H
#define GLUT_494_H

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
void engine_glut_494_reshape (int, int);
void engine_glut_494_key (unsigned char, int, int);
void engine_glut_494_key_up (unsigned char, int, int);
void engine_glut_494_key_special (int, int, int);
void engine_glut_494_key_special_up (int, int, int);
void engine_glut_494_menu (int);
void engine_glut_494_mouse_button (int, int, int, int);
void engine_glut_494_mouse_move (int, int);
void engine_glut_494_timer (int);
void engine_glut_494_draw (void);
void engine_glut_494_idle (void);
void engine_glut_494_visible (int);

struct Engine_OpenGL_GLUT_494_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  Common_GL_Shader                  shader;

  Common_GL_Texture                 texture0;
  Common_GL_Texture                 texture1;

  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             mouseLoc;
  GLint                             texture0Loc;
  GLint                             texture1Loc;

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

#endif // GLUT_494_H
