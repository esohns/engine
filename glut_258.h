#ifndef GLUT_258_H
#define GLUT_258_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_258_reshape (int, int);
void engine_glut_258_key (unsigned char, int, int);
void engine_glut_258_key_special (int, int, int);
void engine_glut_258_menu (int);
void engine_glut_258_mouse_button (int, int, int, int);
void engine_glut_258_mouse_move (int, int);
void engine_glut_258_timer (int);
void engine_glut_258_draw (void);
void engine_glut_258_idle (void);
void engine_glut_258_visible (int);

struct Engine_OpenGL_GLUT_258_CBData
{
  //
  float                   f;
  float                   r;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_258_H
