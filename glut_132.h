#ifndef GLUT_132_H
#define GLUT_132_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_132_reshape (int, int);
void engine_glut_132_key (unsigned char, int, int);
void engine_glut_132_key_special (int, int, int);
void engine_glut_132_menu (int);
void engine_glut_132_mouse_button (int, int, int, int);
void engine_glut_132_mouse_move (int, int);
void engine_glut_132_timer (int);
void engine_glut_132_draw (void);
void engine_glut_132_idle (void);
void engine_glut_132_visible (int);

struct Engine_OpenGL_GLUT_132_CBData
{
  // context
  float                   a;
  float                   f;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;

  // noise
  double                  x;
  double                  y;
  double                  z;
  //noise::module::Perlin   noise;
  OpenSimplexNoise        noise;
};

// other
void engine_glut_132_Z (int, int, struct Engine_OpenGL_GLUT_132_CBData&);

#endif // GLUT_132_H
