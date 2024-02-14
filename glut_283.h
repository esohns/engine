#ifndef GLUT_283_H
#define GLUT_283_H

#include "noise/noise.h"

#include "common_gl_common.h"

#include "defines_13.h"

// GLUT routines
void engine_glut_283_reshape (int, int);
void engine_glut_283_key (unsigned char, int, int);
void engine_glut_283_key_special (int, int, int);
void engine_glut_283_menu (int);
void engine_glut_283_mouse_button (int, int, int, int);
void engine_glut_283_mouse_move (int, int);
void engine_glut_283_timer (int);
void engine_glut_283_draw (void);
void engine_glut_283_idle (void);
void engine_glut_283_visible (int);

struct Engine_OpenGL_GLUT_283_CBData
{
  //

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // noise
  noise::module::Perlin   noise;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

//////////////////////////////////////////

float easeInOutCubic (float);

#endif // GLUT_283_H
