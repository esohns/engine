#ifndef GLUT_25_H
#define GLUT_25_H

#include <vector>

#include "glm/glm.hpp"

#include "common_gl_common.h"

#include "camera.h"

typedef std::vector<glm::fvec3> mandelbulb_t;

struct Engine_OpenGL_GLUT_25_CBData
{
  // camera
  Camera       camera;

  // mandelbulb
  int          dimension;
  mandelbulb_t mandelbulb;
};

void engine_glut_25_generate (struct Engine_OpenGL_GLUT_25_CBData&);

// GLUT routines
void engine_glut_25_key_special (int, int, int);
void engine_glut_25_key_down (unsigned char, int, int);
void engine_glut_25_key_up (unsigned char, int, int);
void engine_glut_25_mouse_move (int, int);
void engine_glut_25_reshape (int, int);
void engine_glut_25_draw (void);

#endif // GLUT_4_H
