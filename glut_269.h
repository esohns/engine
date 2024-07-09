#ifndef GLUT_269_H
#define GLUT_269_H

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
void engine_glut_269_reshape (int, int);
void engine_glut_269_key (unsigned char, int, int);
void engine_glut_269_key_special (int, int, int);
void engine_glut_269_menu (int);
void engine_glut_269_mouse_button (int, int, int, int);
void engine_glut_269_mouse_move (int, int);
void engine_glut_269_timer (int);
void engine_glut_269_draw (void);
void engine_glut_269_idle (void);
void engine_glut_269_visible (int);

class whirl
{
 public:
  whirl (glm::vec2& position)
   : id_ (Common_Tools::getRandomNumber (0, 100000))
   , position_ (position)
   , phase_ (Common_Tools::getRandomNumber (0.0f, 1.0f))
   , r_ (Common_Tools::getRandomNumber (0.1f, 0.5f))
   , distortForce_ (Common_Tools::getRandomNumber (0.0f, 1.0f) * Common_Tools::getRandomNumber (0.0f, 0.2f))
  {}

  int       id_;
  glm::vec2 position_;
  float     phase_;
  float     r_;
  float     distortForce_;
};

struct Engine_OpenGL_GLUT_269_CBData
{
  //
  std::vector<whirl>      whirls;

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   timeLoc;
  GLint                   mouseLoc;
  GLint                   whirlsLoc;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;
};

#endif // GLUT_269_H
