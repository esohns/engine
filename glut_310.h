#ifndef GLUT_310_H
#define GLUT_310_H

//#include "noise/noise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_310_reshape (int, int);
void engine_glut_310_key (unsigned char, int, int);
void engine_glut_310_key_special (int, int, int);
void engine_glut_310_menu (int);
void engine_glut_310_mouse_button (int, int, int, int);
void engine_glut_310_mouse_move (int, int);
void engine_glut_310_timer (int);
void engine_glut_310_draw (void);
void engine_glut_310_idle (void);
void engine_glut_310_visible (int);

struct Engine_OpenGL_GLUT_310_CBData
{
  //
  float                   f;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // noise
  //noise::module::Perlin   noise;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_310_H
