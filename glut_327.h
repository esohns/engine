#ifndef GLUT_327_H
#define GLUT_327_H

#include "noise/noise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_327_reshape (int, int);
void engine_glut_327_key (unsigned char, int, int);
void engine_glut_327_key_special (int, int, int);
void engine_glut_327_menu (int);
void engine_glut_327_mouse_button (int, int, int, int);
void engine_glut_327_mouse_move (int, int);
void engine_glut_327_timer (int);
void engine_glut_327_draw (void);
void engine_glut_327_idle (void);
void engine_glut_327_visible (int);

struct Engine_OpenGL_GLUT_327_CBData
{
  //
  int                     f;
  int                     h;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // noise
  noise::module::Perlin   noise;
};

#endif // GLUT_327_H
