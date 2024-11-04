#ifndef GLUT_389_H
#define GLUT_389_H

#include <chrono>
#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_389_reshape (int, int);
void engine_glut_389_key (unsigned char, int, int);
void engine_glut_389_key_special (int, int, int);
void engine_glut_389_menu (int);
void engine_glut_389_mouse_button (int, int, int, int);
void engine_glut_389_mouse_move (int, int);
void engine_glut_389_timer (int);
void engine_glut_389_draw (void);
void engine_glut_389_idle (void);
void engine_glut_389_visible (int);

struct whirl
{
  int       id;
  glm::vec2 p;
  float     r;
  float     distortForce;
};

struct Engine_OpenGL_GLUT_389_CBData
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
  GLint                             whirlsLoc;

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

  std::vector<struct whirl>         whirls;
};

#endif // GLUT_389_H
