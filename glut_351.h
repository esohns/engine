#ifndef GLUT_351_H
#define GLUT_351_H

#include <vector>

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"

// GLUT routines
void engine_glut_351_reshape (int, int);
void engine_glut_351_key (unsigned char, int, int);
void engine_glut_351_key_special (int, int, int);
void engine_glut_351_menu (int);
void engine_glut_351_mouse_button (int, int, int, int);
void engine_glut_351_mouse_move (int, int);
void engine_glut_351_timer (int);
void engine_glut_351_draw (void);
void engine_glut_351_idle (void);
void engine_glut_351_visible (int);

//////////////////////////////////////////

class particle
{
 public:
  particle (float x, float yRiseRate, float theta1Rate, float theta2Rate)
   : theta1_ (0.0f), theta2_ (0.0f)
   , theta1Rate_ (theta1Rate), theta2Rate_ (theta2Rate)
   , x_ (x), xRun_ (0.0f), xAmp_ (0.0f), xAmpRate_ (0.7f)
   , yRise_ (static_cast<float> (ENGINE_GLUT_351_DEFAULT_HEIGHT)), yRiseRate_ (yRiseRate)
   , hue_ (0.0f)
   , size_ (0.0f)
  {}

  void position1 () {}
  void position2 ()
  {
    glTranslatef (ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_351_DEFAULT_HEIGHT / 2.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI_4), 0.0f, 0.0f, 1.0f);
    glTranslatef (-ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f, -ENGINE_GLUT_351_DEFAULT_HEIGHT / 2.0f, 0.0f);
  }
  void position3 ()
  {
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI) / 8.0f), 0.0f, 0.0f, 1.0f);
  }

  void repitition ()
  {
    if (yRise_ < 0.0f)
    {
      xAmp_ = 0.0f;
      yRise_ = static_cast<float> (ENGINE_GLUT_351_DEFAULT_HEIGHT);
      hue_ = 0.0f;
    } // end IF

    if (yRise_ < static_cast<float> (ENGINE_GLUT_351_DEFAULT_HEIGHT) / 2.0f)
      xAmpRate_ = -0.5f;
    else
      xAmpRate_ = 0.5f;
  }

  void movement ()
  {
    theta2_ += static_cast<float> (M_PI) / theta2Rate_;
    theta1_ += static_cast<float> (M_PI) / theta1Rate_;
    xAmp_ += xAmpRate_;
    xRun_ = (xAmp_ / 4.0f) * std::cos (theta1_) + (xAmp_ / 2.0f) * std::sin (theta2_);
    yRise_ += yRiseRate_;
    hue_ =
      Common_GL_Tools::map (yRise_, 0.0f, static_cast<float> (ENGINE_GLUT_351_DEFAULT_HEIGHT), 360.0f, 0.0f);
    //size = constrain((xAmp/20)*cos(theta1/2), 1, width);
    size_ = std::abs ((xAmp_ / 20.0f) * std::cos (theta1_ / 2.0f));
  }

  void display (bool wireframe)
  {
    float r, g, b;
    Common_Image_Tools::HSVToRGB (hue_, 75.0f / 100.0f, 1.0f, r, g, b);
    glColor3f (r, g, b);

    float radius_f = (4.0f + size_ / 2.0f) / 2.0f;

    glPushMatrix ();
    glTranslatef (xRun_ + x_, yRise_, 0.0f);
    wireframe ? glutWireSphere  (radius_f, 8, 8)
              : glutSolidSphere (radius_f, 8, 8);
    glPopMatrix ();
  }

  float theta1_, theta2_;
  float theta1Rate_, theta2Rate_;
  float x_, xRun_, xAmp_, xAmpRate_;
  float yRise_, yRiseRate_;
  float hue_;
  float size_;
};

class particles
{
 public:
  particles ()
   : embers1_ (ENGINE_GLUT_351_DEFAULT_NUMBER_OF_PARTICLES, particle (0.0f, 0.0f, 0.0f, 0.0f))
   , embers2_ (ENGINE_GLUT_351_DEFAULT_NUMBER_OF_PARTICLES, particle (0.0f, 0.0f, 0.0f, 0.0f))
   , embers3_ (ENGINE_GLUT_351_DEFAULT_NUMBER_OF_PARTICLES, particle (0.0f, 0.0f, 0.0f, 0.0f))
  {
    for (int n = 0; n < static_cast<int> (embers1_.size ()); n++)
      embers1_[n] = particle (ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f,
                              Common_Tools::getRandomNumber (-3.0f, -0.5f),
                              Common_Tools::getRandomNumber (-60.0f, -50.0f),
                              Common_Tools::getRandomNumber (-110.0f, -90.0f));
    for (int n = 0; n < static_cast<int> (embers2_.size ()); n++)
      embers2_[n] = particle (ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f,
                              Common_Tools::getRandomNumber (-3.0f, -0.5f),
                              Common_Tools::getRandomNumber (-60.0f, -50.0f),
                              Common_Tools::getRandomNumber (-110.0f, -90.0f));
    for (int n = 0; n < static_cast<int> (embers3_.size ()); n++)
      embers3_[n] = particle (ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f,
                              Common_Tools::getRandomNumber (-3.0f, -0.5f),
                              Common_Tools::getRandomNumber (-60.0f, -50.0f),
                              Common_Tools::getRandomNumber (-110.0f, -90.0f));
  }

  void run1 (bool wireframe)
  {
    for (int n = 0; n < static_cast<int> (embers1_.size ()); n++)
    {
      glPushMatrix ();
      embers1_[n].repitition ();
      embers1_[n].movement ();
      embers1_[n].position1 ();
      embers1_[n].display (wireframe);
      glPopMatrix ();
    } // end FOR
  }

  void run2 (bool wireframe)
  {
    glTranslatef (-ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f, -ENGINE_GLUT_351_DEFAULT_HEIGHT / 2.0f, 0.0f);

    for (int n = 0; n < static_cast<int> (embers2_.size ()); n++)
    {
      embers2_[n].repitition ();
      embers2_[n].movement ();
      embers2_[n].position2 ();
      embers2_[n].display (wireframe);
    } // end FOR
  }

  void run3 (bool wireframe)
  {
    glTranslatef (ENGINE_GLUT_351_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_351_DEFAULT_HEIGHT / 2.0f, 0.0f);

    for (int n = 0; n < static_cast<int> (embers3_.size ()); n++)
    {
      embers3_[n].repitition ();
      embers3_[n].movement ();
      embers3_[n].position3 ();
      embers3_[n].display (wireframe);
    } // end FOR
  }

  std::vector<particle> embers1_;
  std::vector<particle> embers2_;
  std::vector<particle> embers3_;
};

struct Engine_OpenGL_GLUT_351_CBData
{
  // context
  particles               particlesystem;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_351_H
