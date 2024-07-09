#ifndef GLUT_297_H
#define GLUT_297_H

#include "engine_common.h"

// GLUT routines
void engine_glut_297_reshape (int, int);
void engine_glut_297_key (unsigned char, int, int);
void engine_glut_297_key_special (int, int, int);
void engine_glut_297_menu (int);
void engine_glut_297_mouse_button (int, int, int, int);
void engine_glut_297_mouse_move (int, int);
void engine_glut_297_timer (int);
void engine_glut_297_draw (void);
void engine_glut_297_idle (void);
void engine_glut_297_visible (int);

struct Engine_OpenGL_GLUT_297_CBData
{
  //
  int                     f;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_297_H
