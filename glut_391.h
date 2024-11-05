#ifndef GLUT_391_H
#define GLUT_391_H

#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_391_reshape (int, int);
void engine_glut_391_key (unsigned char, int, int);
void engine_glut_391_key_special (int, int, int);
void engine_glut_391_menu (int);
void engine_glut_391_mouse_button (int, int, int, int);
void engine_glut_391_mouse_move (int, int);
void engine_glut_391_timer (int);
void engine_glut_391_draw (void);
void engine_glut_391_idle (void);
void engine_glut_391_visible (int);


struct ball
{
  float x;
  float y;
  float dx;
  float dy;
  float influence;
};

struct Engine_OpenGL_GLUT_391_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
  GLint                             resolutionLoc;
  GLint                             ballsLoc;
  GLint                             influencesLoc;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;

  // balls
  std::vector<struct ball>          balls;
};

#endif // GLUT_391_H
