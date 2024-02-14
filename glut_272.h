#ifndef GLUT_272_H
#define GLUT_272_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_272_reshape (int, int);
void engine_glut_272_key (unsigned char, int, int);
void engine_glut_272_key_special (int, int, int);
void engine_glut_272_menu (int);
void engine_glut_272_mouse_button (int, int, int, int);
void engine_glut_272_mouse_move (int, int);
void engine_glut_272_timer (int);
void engine_glut_272_draw (void);
void engine_glut_272_idle (void);
void engine_glut_272_visible (int);

struct Engine_OpenGL_GLUT_272_CBData
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

//////////////////////////////////////////

void Z (int x, int n, float y, float s);

#endif // GLUT_272_H
