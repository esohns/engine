#ifndef GLUT_308_H
#define GLUT_308_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_308_reshape (int, int);
void engine_glut_308_key (unsigned char, int, int);
void engine_glut_308_key_special (int, int, int);
void engine_glut_308_menu (int);
void engine_glut_308_mouse_button (int, int, int, int);
void engine_glut_308_mouse_move (int, int);
void engine_glut_308_timer (int);
void engine_glut_308_draw (void);
void engine_glut_308_idle (void);
void engine_glut_308_visible (int);

struct Engine_OpenGL_GLUT_308_CBData
{
  //

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;
};

#endif // GLUT_308_H
