#ifndef GLUT_417_H
#define GLUT_417_H

#include <utility>
#include <vector>

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#include "noise/noise.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_417_reshape (int, int);
void engine_glut_417_key (unsigned char, int, int);
void engine_glut_417_key_special (int, int, int);
void engine_glut_417_menu (int);
void engine_glut_417_mouse_button (int, int, int, int);
void engine_glut_417_mouse_move (int, int);
void engine_glut_417_timer (int);
void engine_glut_417_draw (void);
void engine_glut_417_idle (void);
void engine_glut_417_visible (int);

//////////////////////////////////////////

float linearDMap (float, float);
float decayingSine (float, float);

std::vector<struct point> getPointDistribution (int, float);
std::pair<float, float> cartesianToSpherical (float, float, float, float);
float sphericalDistance (float, float, float, float, float);

struct point
{
  float     latitude;
  float     longitude;
  glm::vec3 position;
};

struct Engine_OpenGL_GLUT_417_CBData
{
  // context
  struct point              ctrlPoint;
  float                     r;
  std::vector<struct point> points;

  // menu
  bool                      wireframe;

  // camera
  struct Engine_GL_Camera   camera;

  // mouse
  int                       mousex;
  int                       mousey;
  bool                      mouse_0_ButtonDown;

  // noise
  noise::module::Perlin     noise;
  double                    xOffset;
  double                    yOffset;
  double                    zOffset;
};

#endif // GLUT_417_H
