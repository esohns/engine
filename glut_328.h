#ifndef GLUT_328_H
#define GLUT_328_H

#include "common_gl_common.h"

// GLUT routines
void engine_glut_328_reshape (int, int);
void engine_glut_328_key (unsigned char, int, int);
void engine_glut_328_key_special (int, int, int);
void engine_glut_328_menu (int);
void engine_glut_328_mouse_button (int, int, int, int);
void engine_glut_328_mouse_move (int, int);
void engine_glut_328_timer (int);
void engine_glut_328_draw (void);
void engine_glut_328_idle (void);
void engine_glut_328_visible (int);

struct Engine_OpenGL_GLUT_328_CBData
{
  //
  int                     N;
  float                   hue;
  std::pair<float, float> ofs;
  float                   alt[3];
  float                   t;
  float                   w;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_328_H
