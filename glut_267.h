#ifndef GLUT_267_H
#define GLUT_267_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_267_reshape (int, int);
void engine_glut_267_key (unsigned char, int, int);
void engine_glut_267_key_special (int, int, int);
void engine_glut_267_menu (int);
void engine_glut_267_mouse_button (int, int, int, int);
void engine_glut_267_mouse_move (int, int);
void engine_glut_267_timer (int);
void engine_glut_267_draw (void);
void engine_glut_267_idle (void);
void engine_glut_267_visible (int);

struct Engine_OpenGL_GLUT_267_CBData
{
  //
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

#endif // GLUT_267_H
