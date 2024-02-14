#ifndef GLUT_318_H
#define GLUT_318_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_318_reshape (int, int);
void engine_glut_318_key (unsigned char, int, int);
void engine_glut_318_key_special (int, int, int);
void engine_glut_318_menu (int);
void engine_glut_318_mouse_button (int, int, int, int);
void engine_glut_318_mouse_move (int, int);
void engine_glut_318_timer (int);
void engine_glut_318_draw (void);
void engine_glut_318_idle (void);
void engine_glut_318_visible (int);

struct Engine_OpenGL_GLUT_318_CBData
{
  //
  int                     f;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_318_H
