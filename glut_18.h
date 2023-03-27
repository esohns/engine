#ifndef GLUT_18_H
#define GLUT_18_H

#include <string>
#include <utility>
#include <vector>

#include "common_gl_common.h"

typedef std::pair<char, std::string> rule_t;
typedef std::vector<rule_t> rules_t;

struct Engine_OpenGL_GLUT_18_CBData
{
  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   xangle;
  float                   xDeltaAngle;
  int                     xOrigin;

  // L-system
  rules_t                 rules;
  std::string             sentence;
  float                   angle;
  float                   length;
};

// GLUT routines
void engine_glut_18_mouse_button (int, int, int, int);
void engine_glut_18_reshape (int, int);
void engine_glut_18_draw (void);

#endif // GLUT_4_H
