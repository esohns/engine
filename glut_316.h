#ifndef GLUT_316_H
#define GLUT_316_H

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

// GLUT routines
void engine_glut_316_reshape (int, int);
void engine_glut_316_key (unsigned char, int, int);
void engine_glut_316_key_special (int, int, int);
void engine_glut_316_menu (int);
void engine_glut_316_mouse_button (int, int, int, int);
void engine_glut_316_mouse_move (int, int);
void engine_glut_316_timer (int);
void engine_glut_316_draw (void);
void engine_glut_316_idle (void);
void engine_glut_316_visible (int);

class particle
{
 public:
  particle (glm::vec3 velocity)
   : position_ ()
   , velocity_ (velocity)
   , life_ (Common_Tools::getRandomNumber (50, 255))
   , maxLife_ (life_)
  {}

  void draw (bool wireFrame)
  {
    glTranslatef (position_.x, position_.y, position_.z);
    float gAndA =
      Common_GL_Tools::map (static_cast<float> (life_), 0.0f, static_cast<float> (maxLife_), 0.0f, 1.0f);
    static glm::vec2 zero (0.0f, 0.0f); glm::vec2 pos (position_.x, position_.y);
    float dt = glm::distance (zero, pos);
    float r, g, b;
    Common_Image_Tools::HSVToRGB (std::fmod (dt, 360.0f), 200.0f / 255.0f, 1.0f, r, g, b);
    glColor4f (r, g, b, gAndA);
    float radius =
      Common_GL_Tools::map (static_cast<float> (life_), 0.0f, static_cast<float> (maxLife_), 5.0f, 1.0f) / 2.0f;
    wireFrame ? glutWireSphere (radius, 12, 12)
              : glutSolidSphere (radius, 12, 12);

    life_--;
  }

  void move ()
  {
    position_ += velocity_;
  }

  glm::vec3 position_;
  glm::vec3 velocity_;
  int       life_;
  int       maxLife_;
};

struct Engine_OpenGL_GLUT_316_CBData
{
  //
  std::vector<particle>   particles;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  int                     mouseX;
  int                     mouseY;
};

#endif // GLUT_316_H
