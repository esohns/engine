#ifndef GLUT_329_H
#define GLUT_329_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_329_reshape (int, int);
void engine_glut_329_key (unsigned char, int, int);
void engine_glut_329_key_special (int, int, int);
void engine_glut_329_menu (int);
void engine_glut_329_mouse_button (int, int, int, int);
void engine_glut_329_mouse_move (int, int);
void engine_glut_329_timer (int);
void engine_glut_329_draw (void);
void engine_glut_329_idle (void);
void engine_glut_329_visible (int);

struct Engine_OpenGL_GLUT_329_CBData
{
  //
  float                   t;
  float                   s;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

//////////////////////////////////////////

void drawCircle (float, float, float, int, bool);
float circleRadius (float);
float spiralRadius (float);
float spiralAngle (float);

#endif // GLUT_329_H
