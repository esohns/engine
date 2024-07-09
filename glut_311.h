#ifndef GLUT_311_H
#define GLUT_311_H

#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "glm/glm.hpp"

#include "common_tools.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_311_reshape (int, int);
void engine_glut_311_key (unsigned char, int, int);
void engine_glut_311_key_special (int, int, int);
void engine_glut_311_menu (int);
void engine_glut_311_mouse_button (int, int, int, int);
void engine_glut_311_mouse_move (int, int);
void engine_glut_311_timer (int);
void engine_glut_311_draw (void);
void engine_glut_311_idle (void);
void engine_glut_311_visible (int);

struct ball
{
  glm::vec2 position;
  glm::vec2 velocity;
  float     radius;
};

struct Engine_OpenGL_GLUT_311_CBData
{
  //
  std::vector<struct ball> balls;
  bool                     spacePressed;

  // canvas
  int                      columns;
  int                      rows;
  int                      scaleFactor;

  // shader
  GLint                    programId;
  GLint                    ballsLoc;

  // menu
  bool                     wireframe;

  // camera
  struct Engine_GL_Camera  camera;

  // mouse
  int                      mouseX;
  int                      mouseY;
};

#endif // GLUT_311_H
