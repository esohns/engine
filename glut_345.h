#ifndef GLUT_345_H
#define GLUT_345_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_345_reshape (int, int);
void engine_glut_345_key (unsigned char, int, int);
void engine_glut_345_key_special (int, int, int);
void engine_glut_345_menu (int);
void engine_glut_345_mouse_button (int, int, int, int);
void engine_glut_345_mouse_move (int, int);
void engine_glut_345_timer (int);
void engine_glut_345_draw (void);
void engine_glut_345_idle (void);
void engine_glut_345_visible (int);

struct Engine_OpenGL_GLUT_345_CBData
{
  // context
  float                   f;
  int32_t                 n;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

//////////////////////////////////////////

void rail (float);

#endif // GLUT_345_H
