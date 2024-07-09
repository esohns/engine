#ifndef GLUT_294_H
#define GLUT_294_H

#include "engine_common.h"

// GLUT routines
void engine_glut_294_reshape (int, int);
void engine_glut_294_key (unsigned char, int, int);
void engine_glut_294_key_special (int, int, int);
void engine_glut_294_menu (int);
void engine_glut_294_mouse_button (int, int, int, int);
void engine_glut_294_mouse_move (int, int);
void engine_glut_294_timer (int);
void engine_glut_294_draw (void);
void engine_glut_294_idle (void);
void engine_glut_294_visible (int);

struct Engine_OpenGL_GLUT_294_CBData
{
  //
  float                   dim;
  float                   f;
  bool                    amped;
  float                   amp;
  float                   amptarget;
  float                   freq;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_294_H
