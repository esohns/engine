#ifndef GLUT_315_H
#define GLUT_315_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_315_reshape (int, int);
void engine_glut_315_key (unsigned char, int, int);
void engine_glut_315_key_special (int, int, int);
void engine_glut_315_menu (int);
void engine_glut_315_mouse_button (int, int, int, int);
void engine_glut_315_mouse_move (int, int);
void engine_glut_315_timer (int);
void engine_glut_315_draw (void);
void engine_glut_315_idle (void);
void engine_glut_315_visible (int);

struct Engine_OpenGL_GLUT_315_CBData
{
  //

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_315_H
