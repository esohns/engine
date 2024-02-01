#ifndef GLUT_229_H
#define GLUT_229_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "common_gl_common.h"

// other functions
float easeInOutExpo (float);

// GLUT routines
void engine_glut_229_reshape (int, int);
void engine_glut_229_key (unsigned char, int, int);
void engine_glut_229_key_special (int, int, int);
void engine_glut_229_menu (int);
void engine_glut_229_mouse_button (int, int, int, int);
void engine_glut_229_mouse_move (int, int);
void engine_glut_229_timer (int);
void engine_glut_229_draw (void);
void engine_glut_229_idle (void);
void engine_glut_229_visible (int);

struct Engine_OpenGL_GLUT_229_CBData
{
  //
  noise::module::Perlin   noise;
  //OpenSimplexNoise        noise;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // fps
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point                                        lastTimeStamp;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> lastTimeStamp;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
};

#endif // GLUT_229_H
