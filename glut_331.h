#ifndef GLUT_331_H
#define GLUT_331_H

#include "noise/noise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_331_reshape (int, int);
void engine_glut_331_key (unsigned char, int, int);
void engine_glut_331_key_special (int, int, int);
void engine_glut_331_menu (int);
void engine_glut_331_mouse_button (int, int, int, int);
void engine_glut_331_mouse_move (int, int);
void engine_glut_331_timer (int);
void engine_glut_331_draw (void);
void engine_glut_331_idle (void);
void engine_glut_331_visible (int);

struct Engine_OpenGL_GLUT_331_CBData
{
  //

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;

  // noise
  noise::module::Perlin   noise;
};

#endif // GLUT_331_H
