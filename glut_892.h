#ifndef GLUT_892_H
#define GLUT_892_H

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
void engine_glut_892_reshape (int, int);
void engine_glut_892_key (unsigned char, int, int);
void engine_glut_892_key_up (unsigned char, int, int);
void engine_glut_892_key_special (int, int, int);
void engine_glut_892_key_special_up (int, int, int);
void engine_glut_892_menu (int);
void engine_glut_892_mouse_button (int, int, int, int);
void engine_glut_892_mouse_move (int, int);
void engine_glut_892_timer (int);
void engine_glut_892_draw (void);
void engine_glut_892_idle (void);
void engine_glut_892_visible (int);

struct Engine_OpenGL_GLUT_892_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  Common_GL_Shader                  shader;

  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             frameLoc;
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

#endif // GLUT_892_H
