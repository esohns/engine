#ifndef GLUT_330_H
#define GLUT_330_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_330_reshape (int, int);
void engine_glut_330_key (unsigned char, int, int);
void engine_glut_330_key_special (int, int, int);
void engine_glut_330_menu (int);
void engine_glut_330_mouse_button (int, int, int, int);
void engine_glut_330_mouse_move (int, int);
void engine_glut_330_timer (int);
void engine_glut_330_draw (void);
void engine_glut_330_idle (void);
void engine_glut_330_visible (int);

struct Engine_OpenGL_GLUT_330_CBData
{
  //
  float                   angle;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;
};

//////////////////////////////////////////

void reflectFrac (float, struct Engine_OpenGL_GLUT_330_CBData& cbData, int);
void drawLine (float, float, struct Engine_OpenGL_GLUT_330_CBData& cbData, int);

#endif // GLUT_330_H
