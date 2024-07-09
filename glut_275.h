#ifndef GLUT_275_H
#define GLUT_275_H

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

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

// GLUT routines
void engine_glut_275_reshape (int, int);
void engine_glut_275_key (unsigned char, int, int);
void engine_glut_275_key_special (int, int, int);
void engine_glut_275_menu (int);
void engine_glut_275_mouse_button (int, int, int, int);
void engine_glut_275_mouse_move (int, int);
void engine_glut_275_timer (int);
void engine_glut_275_draw (void);
void engine_glut_275_idle (void);
void engine_glut_275_visible (int);

class cell
{
 public:
  cell (float x, float y, float z)
   : position_ (x, y, z)
   , density_ (0.0f)
  {}

  void display ()
  {
    if (density_ >= 1.0f)
    {
      glPushMatrix ();

      glTranslatef (position_.x, position_.y, position_.z);

      glScalef (static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_STEP), static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_STEP), static_cast<float> (ENGINE_GLUT_275_DEFAULT_GRID_STEP));

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
    } // end IF
  }

  void update (OpenSimplexNoise& noise,
               int frameCount)
  {
    static float scale = 0.01f;
    static float speed = 0.05f;

    density_ =
      Common_GL_Tools::map (static_cast<float> (noise.Evaluate (position_.x * scale + frameCount * speed, position_.y * scale + frameCount * speed, position_.z * scale + frameCount * speed)), -1.0f, 1.0f, 0.0f, 2.0f);
  }

  glm::vec3 position_;
  float     density_;
};

struct Engine_OpenGL_GLUT_275_CBData
{
  //
  std::vector<cell>       cells;

  // noise
  //noise::module::Perlin   noise;
  OpenSimplexNoise        noise;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_275_H
