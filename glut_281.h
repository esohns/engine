#ifndef GLUT_281_H
#define GLUT_281_H

#include "common_gl_common.h"

#include "defines_13.h"

// GLUT routines
void engine_glut_281_reshape (int, int);
void engine_glut_281_key (unsigned char, int, int);
void engine_glut_281_key_special (int, int, int);
void engine_glut_281_menu (int);
void engine_glut_281_mouse_button (int, int, int, int);
void engine_glut_281_mouse_move (int, int);
void engine_glut_281_timer (int);
void engine_glut_281_draw (void);
void engine_glut_281_idle (void);
void engine_glut_281_visible (int);

struct Engine_OpenGL_GLUT_281_CBData
{
  //
  int                     q;
  float                   f;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_281_H
