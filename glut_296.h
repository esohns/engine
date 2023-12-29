#ifndef GLUT_296_H
#define GLUT_296_H

#include "noise/noise.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_296_reshape (int, int);
void engine_glut_296_key (unsigned char, int, int);
void engine_glut_296_key_special (int, int, int);
void engine_glut_296_menu (int);
void engine_glut_296_mouse_button (int, int, int, int);
void engine_glut_296_mouse_move (int, int);
void engine_glut_296_timer (int);
void engine_glut_296_draw (void);
void engine_glut_296_idle (void);
void engine_glut_296_visible (int);

struct Engine_OpenGL_GLUT_296_CBData
{
  //
  float                   f;

  // noise
  noise::module::Perlin   noise;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_296_H
