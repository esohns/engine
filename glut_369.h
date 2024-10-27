#ifndef GLUT_369_H
#define GLUT_369_H

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
void engine_glut_369_reshape (int, int);
void engine_glut_369_key (unsigned char, int, int);
void engine_glut_369_key_special (int, int, int);
void engine_glut_369_menu (int);
void engine_glut_369_mouse_button (int, int, int, int);
void engine_glut_369_mouse_move (int, int);
void engine_glut_369_timer (int);
void engine_glut_369_draw (void);
void engine_glut_369_idle (void);
void engine_glut_369_visible (int);

class particle
{
 public:
  particle (float x, float y, float vx, float vy, int ci)
   : position_ (x, y)
   , velocity_ (vx, vy)
   , mass_ (Common_Tools::getRandomNumber (1, 20))
   , airDrag_ (Common_Tools::getRandomNumber (0.92f, 0.98f))
   , colorIndex_ (ci)
  {
    velocity_ *= Common_Tools::getRandomNumber (0.0f, 11.0f);
    velocity_ = glm::rotate (velocity_, static_cast<float> (M_PI) / 180.0f * Common_Tools::getRandomNumber (-25.0f, 25.0f));
  }

  void move ()
  {
    velocity_ *= airDrag_;
    position_ += velocity_;
  }

 public:
  glm::vec2 position_;
  glm::vec2 velocity_;
  int       mass_;
  float     airDrag_;
  int       colorIndex_;
};

struct Engine_OpenGL_GLUT_369_CBData
{
  // particles
  std::vector<particle>             particles;

  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
  GLint                             resolutionLoc;
  GLint                             trailCountLoc;
  GLint                             trailLoc;
  GLint                             particleCountLoc;
  GLint                             particlesLoc;
  GLint                             colorsLoc;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
  int                               pmouseX;
  int                               pmouseY;
  bool                              mouseCenterPressed;
  std::vector<std::pair<int, int> > trail;
};

#endif // GLUT_369_H
