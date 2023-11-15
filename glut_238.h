#ifndef GLUT_238_H
#define GLUT_238_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_238_reshape (int, int);
void engine_glut_238_key (unsigned char, int, int);
void engine_glut_238_key_special (int, int, int);
void engine_glut_238_menu (int);
void engine_glut_238_mouse_button (int, int, int, int);
void engine_glut_238_mouse_move (int, int);
void engine_glut_238_timer (int);
void engine_glut_238_draw (void);
void engine_glut_238_idle (void);
void engine_glut_238_visible (int);

struct Engine_OpenGL_GLUT_238_CBData
{
  //
  int                     f;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_238_H
