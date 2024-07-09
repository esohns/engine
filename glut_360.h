#ifndef GLUT_360_H
#define GLUT_360_H

#include <vector>

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

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

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_17.h"

// GLUT routines
void engine_glut_360_reshape (int, int);
void engine_glut_360_key (unsigned char, int, int);
void engine_glut_360_key_special (int, int, int);
void engine_glut_360_menu (int);
void engine_glut_360_mouse_button (int, int, int, int);
void engine_glut_360_mouse_move (int, int);
void engine_glut_360_timer (int);
void engine_glut_360_draw (void);
void engine_glut_360_idle (void);
void engine_glut_360_visible (int);

//////////////////////////////////////////

class platform
{
 public:
  platform ()
   : position_ (0.0f, 0.0f, 0.0f)
   , angle_ (0.0f)
   , affect_ (125.0f)
  {}

  void position (float tx, float ty)
  {
    position_.x = tx;
    position_.y = ty;
    position_.z = -100.0f;

    angle_ = 0.0f;
  }

  void update (float ux, float uy, bool mouse_0_Pressed, float zPos)
  {
    glm::vec2 pos (position_.x, position_.y);
    glm::vec2 pos_2 (ux, uy);
    float mdist = glm::distance (pos, pos_2);

    float lift = Common_GL_Tools::map (mdist, 0.0f, affect_, static_cast<float> (M_PI), 0.0f);

    float easing = Common_GL_Tools::map (mdist, 0.0f, affect_, 0.25f, 0.01f);
    if (easing < 0.01f)
      easing = 0.01f;
    if (easing > 1.0f)
      easing = 1.0f;

    if (mdist < affect_)
      angle_ += (lift - angle_) * easing;
    else
      angle_ += (0.0f - angle_) * easing;
    if (mouse_0_Pressed)
      angle_ += (0.0f - angle_) * 0.1f;

    position_.z = -100.0f + ((50.0f * std::cos (angle_)) * Common_GL_Tools::map (zPos, -115.0f, 0.0f, 1.0f, 0.0f));
  }

  void show ()
  {
    uint8_t g = static_cast<uint8_t> (Common_GL_Tools::map (angle_, 0.0f, static_cast<float> (M_PI), 255.0f, 100.0f));
    glColor3ub (0, g, 0);

    glPushMatrix ();
    glTranslatef (position_.x, position_.y, position_.z);
    glScalef (22.0f, 22.0f, 22.0f);

    glBegin (GL_QUADS);
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
  }

  glm::vec3 position_;
  float     angle_;
  float     affect_;
};

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_360_CBData
{
  // context
  float                   zPos;
  std::vector<platform>   boxes;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mousex;
  int                     mousey;
  bool                    mouse_0_ButtonDown;
};

#endif // GLUT_360_H
