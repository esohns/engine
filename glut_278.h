#ifndef GLUT_278_H
#define GLUT_278_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "common_gl_common.h"

#include "defines_12.h"

// GLUT routines
void engine_glut_278_reshape (int, int);
void engine_glut_278_key (unsigned char, int, int);
void engine_glut_278_key_special (int, int, int);
void engine_glut_278_menu (int);
void engine_glut_278_mouse_button (int, int, int, int);
void engine_glut_278_mouse_move (int, int);
void engine_glut_278_timer (int);
void engine_glut_278_draw (void);
void engine_glut_278_idle (void);
void engine_glut_278_visible (int);

struct Engine_OpenGL_GLUT_278_CBData
{
  //
  float                   f;

  // noise
  //noise::module::Perlin   noise;
  OpenSimplexNoise        noise;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_278_H
