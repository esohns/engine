#ifndef GLUT_285_H
#define GLUT_285_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_285_reshape (int, int);
void engine_glut_285_key (unsigned char, int, int);
void engine_glut_285_key_special (int, int, int);
void engine_glut_285_menu (int);
void engine_glut_285_mouse_button (int, int, int, int);
void engine_glut_285_mouse_move (int, int);
void engine_glut_285_timer (int);
void engine_glut_285_draw (void);
void engine_glut_285_idle (void);
void engine_glut_285_visible (int);

struct Engine_OpenGL_GLUT_285_CBData
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

#endif // GLUT_285_H
