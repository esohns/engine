#ifndef GLUT_317_H
#define GLUT_317_H

#include "engine_common.h"

// GLUT routines
void engine_glut_317_reshape (int, int);
void engine_glut_317_key (unsigned char, int, int);
void engine_glut_317_key_special (int, int, int);
void engine_glut_317_menu (int);
void engine_glut_317_mouse_button (int, int, int, int);
void engine_glut_317_mouse_move (int, int);
void engine_glut_317_timer (int);
void engine_glut_317_draw (void);
void engine_glut_317_idle (void);
void engine_glut_317_visible (int);

struct Engine_OpenGL_GLUT_317_CBData
{
  //
  int                     w;
  float                   r;
  float                   zStart;
  int                     loopTime;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  //int                     mouseX;
  //int                     mouseY;
};

float e (float);
void drawCircle (float, float, float, int);

#endif // GLUT_317_H
