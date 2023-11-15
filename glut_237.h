#ifndef GLUT_237_H
#define GLUT_237_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_237_reshape (int, int);
void engine_glut_237_key (unsigned char, int, int);
void engine_glut_237_key_special (int, int, int);
void engine_glut_237_menu (int);
void engine_glut_237_mouse_button (int, int, int, int);
void engine_glut_237_mouse_move (int, int);
void engine_glut_237_timer (int);
void engine_glut_237_draw (void);
void engine_glut_237_idle (void);
void engine_glut_237_visible (int);

struct Engine_OpenGL_GLUT_237_CBData
{
  //
  float                   offset;
  float                   ring;
  float                   Yangle;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_237_H
