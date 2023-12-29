#ifndef GLUT_298_H
#define GLUT_298_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_298_reshape (int, int);
void engine_glut_298_key (unsigned char, int, int);
void engine_glut_298_key_special (int, int, int);
void engine_glut_298_menu (int);
void engine_glut_298_mouse_button (int, int, int, int);
void engine_glut_298_mouse_move (int, int);
void engine_glut_298_timer (int);
void engine_glut_298_draw (void);
void engine_glut_298_idle (void);
void engine_glut_298_visible (int);

struct Engine_OpenGL_GLUT_298_CBData
{
  //
  float                   i;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_298_H
