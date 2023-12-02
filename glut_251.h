#ifndef GLUT_251_H
#define GLUT_251_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "glm/glm.hpp"

#include "common_tools.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_251_reshape (int, int);
void engine_glut_251_key (unsigned char, int, int);
void engine_glut_251_key_special (int, int, int);
void engine_glut_251_menu (int);
void engine_glut_251_mouse_button (int, int, int, int);
void engine_glut_251_mouse_move (int, int);
void engine_glut_251_timer (int);
void engine_glut_251_draw (void);
void engine_glut_251_idle (void);
void engine_glut_251_visible (int);

class metaball
{
 public:
  metaball (int width_in, int height_in)
   : position_ (width_in / 2.0f, height_in / 2.0f)
   , velocity_ ()
   , radius_ (0.0f)
  {
    float size = static_cast<float> (std::pow (Common_Tools::getRandomNumber (0.0f, 1.0f), 2));
    velocity_.x = Common_Tools::getRandomNumber (0.0f, 1.0f);
    velocity_.y = Common_Tools::getRandomNumber (0.0f, 1.0f);
    velocity_ *= (1.0f - size) + 2.0f;
    radius_ = 80.0f * size + 20.0f;
  }

  void update (int width_in, int height_in)
  {
    position_ += velocity_;

    if (position_.x < radius_ || position_.x > width_in - radius_) velocity_.x *= -1.0f;
    if (position_.y < radius_ || position_.y > height_in - radius_) velocity_.y *= -1.0f;
  }

  glm::vec2 position_;
  glm::vec2 velocity_;
  float     radius_;
};

struct Engine_OpenGL_GLUT_251_CBData
{
  //
  std::vector<metaball>   balls;

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   ballsLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_248_H
