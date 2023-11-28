#ifndef GLUT_253_H
#define GLUT_253_H

#include "common_gl_common.h"

#include "defines_11.h"

// GLUT routines
void engine_glut_253_reshape (int, int);
void engine_glut_253_key (unsigned char, int, int);
void engine_glut_253_key_special (int, int, int);
void engine_glut_253_menu (int);
void engine_glut_253_mouse_button (int, int, int, int);
void engine_glut_253_mouse_move (int, int);
void engine_glut_253_timer (int);
void engine_glut_253_draw (void);
void engine_glut_253_idle (void);
void engine_glut_253_visible (int);

class vox
{
 public:
  vox ()
   : position ()
   , free (true)
   , slave (false)
   , dimension ()
  {}

  vox (glm::i32vec3 pos)
   : position (pos)
   , free (true)
   , slave (false)
   , dimension (1, 1, 1)
  {}

  glm::i32vec3 position;
  bool         free;
  bool         slave;
  glm::i32vec3 dimension;
};

struct Engine_OpenGL_GLUT_253_CBData
{
  //
  float                   a0;
  bool                    flip;
  vox                     voxGrid[ENGINE_GLUT_253_DEFAULT_DIMENSION_X * ENGINE_GLUT_253_DEFAULT_DIMENSION_Y * ENGINE_GLUT_253_DEFAULT_DIMENSION_Z];

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

int index (int, int, int);
int maxIndex ();
bool validIndex (int, int, int);

float easeInOutQuint (float);

void createVoxWorld (struct Engine_OpenGL_GLUT_253_CBData&);

void drawFaceBox (int, int, int, Common_GL_Color_t&, Common_GL_Color_t&, Common_GL_Color_t&);
void drawVox (int, int, int, float, struct Engine_OpenGL_GLUT_253_CBData&);

bool areAllFree (glm::i32vec3, glm::i32vec3, struct Engine_OpenGL_GLUT_253_CBData&);
void claim (glm::i32vec3, glm::i32vec3, struct Engine_OpenGL_GLUT_253_CBData&);

void irregularSubDivide (struct Engine_OpenGL_GLUT_253_CBData&);

#endif // GLUT_253_H
