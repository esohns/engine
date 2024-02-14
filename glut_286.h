#ifndef GLUT_286_H
#define GLUT_286_H

#include "noise/noise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_286_reshape (int, int);
void engine_glut_286_key (unsigned char, int, int);
void engine_glut_286_key_special (int, int, int);
void engine_glut_286_menu (int);
void engine_glut_286_mouse_button (int, int, int, int);
void engine_glut_286_mouse_move (int, int);
void engine_glut_286_timer (int);
void engine_glut_286_draw (void);
void engine_glut_286_idle (void);
void engine_glut_286_visible (int);

struct Engine_OpenGL_GLUT_286_CBData
{
  //
  int                     f;
  int                     i;

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

#endif // GLUT_286_H
