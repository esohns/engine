#ifndef GLUT_348_H
#define GLUT_348_H

#include "common_gl_common.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_348_reshape (int, int);
void engine_glut_348_key (unsigned char, int, int);
void engine_glut_348_key_special (int, int, int);
void engine_glut_348_menu (int);
void engine_glut_348_mouse_button (int, int, int, int);
void engine_glut_348_mouse_move (int, int);
void engine_glut_348_timer (int);
void engine_glut_348_draw (void);
void engine_glut_348_idle (void);
void engine_glut_348_visible (int);

struct Engine_OpenGL_GLUT_348_CBData
{
  // context

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

//////////////////////////////////////////

void ring (float, float, float, float, float, Common_GL_Color_t&, bool);

#endif // GLUT_348_H
