#ifndef GLUT_240_H
#define GLUT_240_H

#include "engine_common.h"

// GLUT routines
void engine_glut_240_reshape (int, int);
void engine_glut_240_key (unsigned char, int, int);
void engine_glut_240_key_special (int, int, int);
void engine_glut_240_menu (int);
void engine_glut_240_mouse_button (int, int, int, int);
void engine_glut_240_mouse_move (int, int);
void engine_glut_240_timer (int);
void engine_glut_240_draw (void);
void engine_glut_240_idle (void);
void engine_glut_240_visible (int);

struct Engine_OpenGL_GLUT_240_CBData
{
  //
  float                   f;
  float                   p;
  float                   P;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_240_H
