#ifndef GLUT_312_H
#define GLUT_312_H

#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "glm/glm.hpp"

#include "common_tools.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_312_reshape (int, int);
void engine_glut_312_key (unsigned char, int, int);
void engine_glut_312_key_special (int, int, int);
void engine_glut_312_menu (int);
void engine_glut_312_mouse_button (int, int, int, int);
void engine_glut_312_mouse_move (int, int);
void engine_glut_312_timer (int);
void engine_glut_312_draw (void);
void engine_glut_312_idle (void);
void engine_glut_312_visible (int);

struct ball_2
{
  glm::vec2 position;
  glm::vec2 velocity;
  float     radius;
};

struct Engine_OpenGL_GLUT_312_CBData
{
  //
  std::vector<struct ball_2> balls;

  // canvas
  int                        columns;
  int                        rows;
  int                        scaleFactor;

  // shader
  GLint                      programId;
  GLint                      ballsLoc;

  // menu
  bool                       wireframe;

  // camera
  struct Common_GL_Camera    camera;

  // mouse
  int                        mouseX;
  int                        mouseY;
};

#endif // GLUT_311_H
