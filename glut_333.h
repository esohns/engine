#ifndef GLUT_333_H
#define GLUT_333_H

#include "noise/noise.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_333_reshape (int, int);
void engine_glut_333_key (unsigned char, int, int);
void engine_glut_333_key_special (int, int, int);
void engine_glut_333_menu (int);
void engine_glut_333_mouse_button (int, int, int, int);
void engine_glut_333_mouse_move (int, int);
void engine_glut_333_timer (int);
void engine_glut_333_draw (void);
void engine_glut_333_idle (void);
void engine_glut_333_visible (int);

struct Engine_OpenGL_GLUT_333_CBData
{
  //
  float                   radiusNoise;
  float                   angleStep;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;

  // noise
  noise::module::Perlin   noise;
};

//////////////////////////////////////////

void drawCircle_2 (float, float, float, int, bool);

#endif // GLUT_333_H
