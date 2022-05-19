#ifndef GLUT_3_H
#define GLUT_3_H

#include "noise/noise.h"

#include "common_gl_common.h"

#include "common_image_common.h"

void engine_glut_3_reshape (int, int);
void engine_glut_3_key (unsigned char, int, int);
void engine_glut_3_mouse_button (int, int, int, int);
void engine_glut_3_mouse_move (int, int);
void engine_glut_3_timer (int);
void engine_glut_3_draw (void);
void engine_glut_3_idle (void);
void engine_glut_3_visible (int);

struct Engine_OpenGL_GLUT_3_CBData
{
  int                       columns;
  int                       rows;
  int                       scaleFactor;
  Common_Image_Resolution_t size;

  // camera
  struct Common_GL_Camera   camera;

  // mouse
  float                     angle;
  float                     deltaAngle;
  int                       xOrigin;

  // noise
  double                    x;
  double                    y;
  double                    z;
  double                    step;
  noise::module::Perlin     module;

  // terrain
  float*                    terrain;
  double                    xOffset;
};

#endif // GLUT_3_H
