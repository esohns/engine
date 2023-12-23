#ifndef GLUT_279_H
#define GLUT_279_H

#include <vector>

#include "common_gl_common.h"

#include "defines_12.h"

// GLUT routines
void engine_glut_279_reshape (int, int);
void engine_glut_279_key (unsigned char, int, int);
void engine_glut_279_key_special (int, int, int);
void engine_glut_279_menu (int);
void engine_glut_279_mouse_button (int, int, int, int);
void engine_glut_279_mouse_move (int, int);
void engine_glut_279_timer (int);
void engine_glut_279_draw (void);
void engine_glut_279_idle (void);
void engine_glut_279_visible (int);

struct Engine_OpenGL_GLUT_279_CBData
{
  //
  std::vector<glm::vec3>  points;
  int                     spongeSize;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

//////////////////////////////////////////

void createSponge (int, int, int, int, struct Engine_OpenGL_GLUT_279_CBData&);

#endif // GLUT_279_H
