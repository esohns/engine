#ifndef GLUT_295_H
#define GLUT_295_H

#include "engine_common.h"

// GLUT routines
void engine_glut_295_reshape (int, int);
void engine_glut_295_key (unsigned char, int, int);
void engine_glut_295_key_special (int, int, int);
void engine_glut_295_menu (int);
void engine_glut_295_mouse_button (int, int, int, int);
void engine_glut_295_mouse_move (int, int);
void engine_glut_295_timer (int);
void engine_glut_295_draw (void);
void engine_glut_295_idle (void);
void engine_glut_295_visible (int);

struct Engine_OpenGL_GLUT_295_CBData
{
  //
  float                   f;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_295_H
