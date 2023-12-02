#ifndef GLUT_222_H
#define GLUT_222_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_222_reshape (int, int);
void engine_glut_222_key (unsigned char, int, int);
void engine_glut_222_key_special (int, int, int);
void engine_glut_222_menu (int);
void engine_glut_222_mouse_button (int, int, int, int);
void engine_glut_222_mouse_move (int, int);
void engine_glut_222_timer (int);
void engine_glut_222_draw (void);
void engine_glut_222_idle (void);
void engine_glut_222_visible (int);

struct Engine_OpenGL_GLUT_222_CBData
{
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   timeLoc;

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

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_222_H
