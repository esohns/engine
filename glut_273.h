#ifndef GLUT_273_H
#define GLUT_273_H

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

#include "glm/glm.hpp"

#include "common_tools.h"

#include "common_math_tools.h"

#include "common_gl_common.h"

// GLUT routines
void engine_glut_273_reshape (int, int);
void engine_glut_273_key (unsigned char, int, int);
void engine_glut_273_key_special (int, int, int);
void engine_glut_273_menu (int);
void engine_glut_273_mouse_button (int, int, int, int);
void engine_glut_273_mouse_move (int, int);
void engine_glut_273_timer (int);
void engine_glut_273_draw (void);
void engine_glut_273_idle (void);
void engine_glut_273_visible (int);

//////////////////////////////////////////

class shape
{
 public:
  shape (glm::vec3& pos, glm::vec3& ang, float s)
   : position_ (pos)
   , angle_ (ang)
   , tang_ (ang)
   , otang_ (ang)
   , s_ (0.0f)
   , ts_ (s)
   , ots_ (s)
   , tpos_ (pos)
   , spikes_ ()
  {
    for (int i = 0; i < 3; i++)
    {
      glm::vec3 vec (Common_Tools::getRandomNumber (0.0f, 1.0f),
                     Common_Tools::getRandomNumber (0.0f, 1.0f),
                     Common_Tools::getRandomNumber (0.0f, 1.0f));
      spikes_.push_back (vec);
    } // end FOR
  }

  void animate ()
  {
    s_ = Common_Math_Tools::lerp (s_, ts_, 0.1f);

    position_.x = Common_Math_Tools::lerp (position_.x, tpos_.x, 0.1f);
    position_.y = Common_Math_Tools::lerp (position_.y, tpos_.y, 0.1f);
    position_.z = Common_Math_Tools::lerp (position_.z, tpos_.z, 0.1f);

    angle_.x = Common_Math_Tools::lerp (angle_.x, tang_.x, 0.1f);
    angle_.y = Common_Math_Tools::lerp (angle_.y, tang_.y, 0.1f);
    angle_.z = Common_Math_Tools::lerp (angle_.z, tang_.z, 0.1f);
  }

  void show (int style,
             bool wireFrame)
  {
    glPushMatrix ();

    glRotatef (angle_.x * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);
    glTranslatef (position_.y, position_.x, 0.0f);
    glRotatef (angle_.y * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef (angle_.z * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);

    switch (style)
    {
      case 0:
      {
        glScalef (s_, s_, s_);

        glBegin (GL_QUADS);
        // Top face (y = 0.5f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glVertex3f ( 0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f,  0.5f);
        glVertex3f ( 0.5f, 0.5f,  0.5f);

        // Bottom face (y = -0.5f)
        glVertex3f ( 0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f ( 0.5f, -0.5f, -0.5f);

        // Front face  (z = 0.5f)
        glVertex3f ( 0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f, -0.5f, 0.5f);
        glVertex3f ( 0.5f, -0.5f, 0.5f);

        // Back face (z = -0.5f)
        glVertex3f ( 0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f ( 0.5f,  0.5f, -0.5f);

        // Left face (x = -0.5f)
        glVertex3f (-0.5f,  0.5f,  0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);

        // Right face (x = 0.5f)
        glVertex3f (0.5f,  0.5f, -0.5f);
        glVertex3f (0.5f,  0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f, -0.5f);
        glEnd ();

        break;
      }
      case 1:
      {
        glTranslatef (0.0f, 0.0f, -s_ / 2.0f);

        wireFrame ? glutWireCylinder (s_ / 2.0, s_, 16, 16)
                  : glutSolidCylinder (s_ / 2.0, s_, 16, 16);

        break;
      }
      case 2:
      {
        wireFrame ? glutWireTorus (s_ / 4.0, s_ / 2.0, 16, 16)
                  : glutSolidTorus (s_ / 4.0, s_ / 2.0, 16, 16);
        break;
      }
      case 3:
      {
        glPushMatrix ();

        glTranslatef (0.0f, 0.0f, s_ / 1.5f);

        glScalef (s_ / 1.5f, s_ / 1.5f, s_ / 1.5f);

        glBegin (GL_QUADS);
        // Top face (y = 0.5f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glVertex3f ( 0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f,  0.5f);
        glVertex3f ( 0.5f, 0.5f,  0.5f);

        // Bottom face (y = -0.5f)
        glVertex3f ( 0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f ( 0.5f, -0.5f, -0.5f);

        // Front face  (z = 0.5f)
        glVertex3f ( 0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f, -0.5f, 0.5f);
        glVertex3f ( 0.5f, -0.5f, 0.5f);

        // Back face (z = -0.5f)
        glVertex3f ( 0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f ( 0.5f,  0.5f, -0.5f);

        // Left face (x = -0.5f)
        glVertex3f (-0.5f,  0.5f,  0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);

        // Right face (x = 0.5f)
        glVertex3f (0.5f,  0.5f, -0.5f);
        glVertex3f (0.5f,  0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f, -0.5f);
        glEnd ();

        glPopMatrix ();

        glPushMatrix ();

        glTranslatef (0.0f, 0.0f, -s_ / 1.5f);

        glScalef (s_ / 1.5f, s_ / 1.5f, s_ / 1.5f);

        glBegin (GL_QUADS);
        // Top face (y = 0.5f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glVertex3f ( 0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f, -0.5f);
        glVertex3f (-0.5f, 0.5f,  0.5f);
        glVertex3f ( 0.5f, 0.5f,  0.5f);

        // Bottom face (y = -0.5f)
        glVertex3f ( 0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f ( 0.5f, -0.5f, -0.5f);

        // Front face  (z = 0.5f)
        glVertex3f ( 0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f,  0.5f, 0.5f);
        glVertex3f (-0.5f, -0.5f, 0.5f);
        glVertex3f ( 0.5f, -0.5f, 0.5f);

        // Back face (z = -0.5f)
        glVertex3f ( 0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f ( 0.5f,  0.5f, -0.5f);

        // Left face (x = -0.5f)
        glVertex3f (-0.5f,  0.5f,  0.5f);
        glVertex3f (-0.5f,  0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f, -0.5f);
        glVertex3f (-0.5f, -0.5f,  0.5f);

        // Right face (x = 0.5f)
        glVertex3f (0.5f,  0.5f, -0.5f);
        glVertex3f (0.5f,  0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f,  0.5f);
        glVertex3f (0.5f, -0.5f, -0.5f);
        glEnd ();

        glPopMatrix ();

        break;
      }
      default:
        ACE_ASSERT (false);
        break;
    } // end SWITCH

    glPopMatrix ();
  }

  glm::vec3              position_;
  glm::vec3              angle_;
  glm::vec3              tang_;
  glm::vec3              otang_;
  float                  s_;
  float                  ts_;
  float                  ots_;
  glm::vec3              tpos_;
  std::vector<glm::vec3> spikes_;
};

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_273_CBData
{
  //
  int                     style;
  std::vector<shape>      shapes;
  float                   tip;
  float                   angle;
  float                   angleVelocity;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   deltaAngleX;
  int                     xOrigin;
  float                   deltaZoomZ;
  int                     zOrigin;
};

#endif // GLUT_273_H
