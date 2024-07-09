#ifndef GLUT_227_H
#define GLUT_227_H

#include "engine_common.h"

// GLUT routines
void engine_glut_227_reshape (int, int);
void engine_glut_227_key (unsigned char, int, int);
void engine_glut_227_key_special (int, int, int);
void engine_glut_227_menu (int);
void engine_glut_227_mouse_button (int, int, int, int);
void engine_glut_227_mouse_move (int, int);
void engine_glut_227_timer (int);
void engine_glut_227_draw (void);
void engine_glut_227_idle (void);
void engine_glut_227_visible (int);

struct Engine_OpenGL_GLUT_227_CBData
{
  // 
  float                   angleX;
  float                   angleY;
  float                   angleZ;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_227_H
