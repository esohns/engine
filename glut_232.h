#ifndef GLUT_232_H
#define GLUT_232_H

#include <vector>

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_tools.h"

#include "common_gl_common.h"

//////////////////////////////////////////

class object
{
 public:
  object ()
   : dist_ (Common_Tools::getRandomNumber (150.0f, 300.0f))
   , x_ (Common_Tools::getRandomNumber (0.0f, dist_))
   , y_ (0.0f)
   , a_ (0.0f)
   , range_ (Common_Tools::getRandomNumber (-20.0f, 50.0f))
   , c_ (Common_Tools::getRandomNumber (0.5f, 15.0f))
   , e_ (Common_Tools::getRandomNumber (0.0f, 255.0f))
   , xSpeed_ (Common_Tools::getRandomNumber (0.1f, 0.4f))
   , ySpeed_ (Common_Tools::getRandomNumber (0.02f, 0.04f))
   , rot_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
   , r_ (static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)))
   , g_ (static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)))
   , b_ (static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)))
   , s_ (1.0f)
   , r2_ (0.0f)
  {}

  void move ()
  {
    x_ += xSpeed_;
    y_ = range_ * std::sin (x_ * ySpeed_);
    s_ += 0.002f;
  }

  void show ()
  {
    glPushMatrix ();
    glRotatef (rot_ * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
    for (int i2 = 0; i2 < 50; i2 += 1)
    {
      glColor4ub (r_, g_, 255, 100);

      glBegin (GL_TRIANGLE_FAN);
      float x1 = x_ + i2 * xSpeed_;
      float y1 = range_ * std::sin ((x_ + i2 * xSpeed_) * ySpeed_);
      float r = r2_ / 50.0f * i2 * s_;
      glVertex2f (x1, y1);
      for (float angle = 0.0f; angle < 2.0f * static_cast<float> (M_PI) + 0.2f; angle += 0.2f)
      {
        float x2 = x1 + std::sin (angle) * r;
        float y2 = y1 + std::cos (angle) * r;
        glVertex2f (x2, y2);
      } // end FOR
      glEnd ();

      if (x_ + i2 * xSpeed_ > dist_)
      {
        x_ = -50.0f * xSpeed_;
        dist_ = Common_Tools::getRandomNumber (200.0f, 300.0f);
        r_ = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
        g_ = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
        b_ = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
        s_ = 1.0f;
      } // end IF
      if (x_ + i2 * xSpeed_ < 0.0f)
        r2_ = 0.0f;
      else
        r2_ = c_ * std::sin (i2 / 50.0f * static_cast<float> (M_PI)) + 1.0f;
    } // end FOR
    glPopMatrix ();
  }

  float dist_;
  float x_;
  float y_;
  float a_;
  float range_;
  float c_;
  float e_;
  float xSpeed_;
  float ySpeed_;
  float rot_;
  uint8_t r_;
  uint8_t g_;
  uint8_t b_;
  float s_;
  float r2_;
};

//////////////////////////////////////////

// GLUT routines
void engine_glut_232_reshape (int, int);
void engine_glut_232_key (unsigned char, int, int);
void engine_glut_232_key_special (int, int, int);
void engine_glut_232_menu (int);
void engine_glut_232_mouse_button (int, int, int, int);
void engine_glut_232_mouse_move (int, int);
void engine_glut_232_timer (int);
void engine_glut_232_draw (void);
void engine_glut_232_idle (void);
void engine_glut_232_visible (int);

struct Engine_OpenGL_GLUT_232_CBData
{
  //
  std::vector<object>     objects;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;
};

#endif // GLUT_232_H
