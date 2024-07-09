#ifndef GLUT_236_H
#define GLUT_236_H

#include "engine_common.h"

// GLUT routines
void engine_glut_236_reshape (int, int);
void engine_glut_236_key (unsigned char, int, int);
void engine_glut_236_key_special (int, int, int);
void engine_glut_236_menu (int);
void engine_glut_236_mouse_button (int, int, int, int);
void engine_glut_236_mouse_move (int, int);
void engine_glut_236_timer (int);
void engine_glut_236_draw (void);
void engine_glut_236_idle (void);
void engine_glut_236_visible (int);

struct Engine_OpenGL_GLUT_236_CBData
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

#endif // GLUT_236_H
