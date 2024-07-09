#ifndef GLUT_264_H
#define GLUT_264_H

#include <vector>

#include "engine_common.h"

// GLUT routines
void engine_glut_264_reshape (int, int);
void engine_glut_264_key (unsigned char, int, int);
void engine_glut_264_key_special (int, int, int);
void engine_glut_264_menu (int);
void engine_glut_264_mouse_button (int, int, int, int);
void engine_glut_264_mouse_move (int, int);
void engine_glut_264_timer (int);
void engine_glut_264_draw (void);
void engine_glut_264_idle (void);
void engine_glut_264_visible (int);

struct Engine_OpenGL_GLUT_264_CBData
{
  //
  float                   time;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

glm::vec3 figureEight (float, int);
glm::vec3 rotatePoint (float, float, float, float, float);
std::vector<glm::vec3> createCapVerts (glm::vec3&, glm::vec3&, int);
std::vector<glm::vec3> createVertLoop (float, float, int, int, int, int);

#endif // GLUT_264_H
