#ifndef GLUT_287_H
#define GLUT_287_H

#include "engine_common.h"

// GLUT routines
void engine_glut_287_reshape (int, int);
void engine_glut_287_key (unsigned char, int, int);
void engine_glut_287_key_special (int, int, int);
void engine_glut_287_menu (int);
void engine_glut_287_mouse_button (int, int, int, int);
void engine_glut_287_mouse_move (int, int);
void engine_glut_287_timer (int);
void engine_glut_287_draw (void);
void engine_glut_287_idle (void);
void engine_glut_287_visible (int);

struct Engine_OpenGL_GLUT_287_CBData
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

#endif // GLUT_287_H
