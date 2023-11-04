#ifndef GLUT_220_H
#define GLUT_220_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_220_reshape (int, int);
void engine_glut_220_key (unsigned char, int, int);
void engine_glut_220_key_special (int, int, int);
void engine_glut_220_menu (int);
void engine_glut_220_mouse_button (int, int, int, int);
void engine_glut_220_mouse_move (int, int);
void engine_glut_220_timer (int);
void engine_glut_220_draw (void);
void engine_glut_220_idle (void);
void engine_glut_220_visible (int);

struct Engine_OpenGL_GLUT_220_CBData
{
  // context
  float                   i;
  float                   f;
  int32_t                 m;
  int32_t                 n;
  int32_t                 mod;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_220_H
