#ifndef GLUT_304_H
#define GLUT_304_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_304_reshape (int, int);
void engine_glut_304_key (unsigned char, int, int);
void engine_glut_304_key_special (int, int, int);
void engine_glut_304_menu (int);
void engine_glut_304_mouse_button (int, int, int, int);
void engine_glut_304_mouse_move (int, int);
void engine_glut_304_timer (int);
void engine_glut_304_draw (void);
void engine_glut_304_idle (void);
void engine_glut_304_visible (int);

struct Engine_OpenGL_GLUT_304_CBData
{
  //
  float                   time;

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   timeLoc;
  GLint                   powerLoc;
  GLint                   angleLoc;
  GLint                   mouseLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_304_H
