#ifndef GLUT_370_H
#define GLUT_370_H

#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "glm/glm.hpp"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_17.h"

// GLUT routines
void engine_glut_370_reshape (int, int);
void engine_glut_370_key (unsigned char, int, int);
void engine_glut_370_key_special (int, int, int);
void engine_glut_370_menu (int);
void engine_glut_370_mouse_button (int, int, int, int);
void engine_glut_370_mouse_move (int, int);
void engine_glut_370_timer (int);
void engine_glut_370_draw (void);
void engine_glut_370_idle (void);
void engine_glut_370_visible (int);

class particle_2
{
 public:
  particle_2 (float x, float y, const Common_GL_Color_t& c)
   : position_ (x, y)
   , velocity_ (Common_Tools::getRandomNumber (0.0f, 1.0f), Common_Tools::getRandomNumber (0.0f, 1.0f))
   , mass_ (Common_Tools::getRandomNumber (1, 30))
   , lifeCount_ (Common_Tools::getRandomNumber (-100, 100))
   , color_ (c)
  {
    velocity_ = glm::normalize (velocity_);
    velocity_ *= Common_Tools::getRandomNumber (1.0f, 20.0f);
    velocity_ = glm::rotate (velocity_, static_cast<float> (M_PI) / 180.0f * Common_Tools::getRandomNumber (-25.0f, 25.0f));
  }

  void move ()
  {
    float attitudeX = std::sin (lifeCount_ / 20.0f);
    float attitudeY = std::cos (lifeCount_ / 120.0f);
    int massX = Common_GL_Tools::map (mass_, 0, 30, ENGINE_GLUT_370_DEFAULT_WIDTH / 2, 130);
    int massY = Common_GL_Tools::map (mass_, 0, 30, ENGINE_GLUT_370_DEFAULT_HEIGHT / 2, 130);
    float x = attitudeX * (std::sin (lifeCount_ / 116.0f) * massX);
    float y = attitudeY * (std::sin (lifeCount_ / 16.0f)  * massY);

    position_ = glm::vec2 (ENGINE_GLUT_370_DEFAULT_WIDTH / 2 + x, ENGINE_GLUT_370_DEFAULT_HEIGHT / 2 + y);
    // position_ = new p5.Vector(mouseX + x, mouseY + y);
    // position_.add(x, y);
  }

 public:
  glm::vec2         position_;
  glm::vec2         velocity_;
  int               mass_;
  int               lifeCount_;
  Common_GL_Color_t color_;
};

class particleGroup
{
 public:
  particleGroup ()
  {}

  void initialize (std::vector<particle_2>& particles)
  {
    particles_ = &particles;
  }

  void move ()
  {
    for (int i = 0; i < particles_->size (); i++)
    {
      particle_2& p = particles_->operator[] (i);
      p.move ();
      p.lifeCount_++;
    } // end FOR
  }

 public:
  std::vector<particle_2>* particles_;
};

struct Engine_OpenGL_GLUT_370_CBData
{
  // particles
  std::vector<particle_2>           particles;
  particleGroup                     particleGroup;

  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             programId;
  GLint                             resolutionLoc;
  GLint                             particlesLoc;
  GLint                             particleCountLoc;
  GLint                             particleColorsLoc;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_370_H
