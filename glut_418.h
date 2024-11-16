#ifndef GLUT_418_H
#define GLUT_418_H

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

#include "engine_common.h"

#include "defines_19.h"

// GLUT routines
void engine_glut_418_reshape (int, int);
void engine_glut_418_key (unsigned char, int, int);
void engine_glut_418_key_special (int, int, int);
void engine_glut_418_menu (int);
void engine_glut_418_mouse_button (int, int, int, int);
void engine_glut_418_mouse_move (int, int);
void engine_glut_418_timer (int);
void engine_glut_418_draw (void);
void engine_glut_418_idle (void);
void engine_glut_418_visible (int);

//////////////////////////////////////////

class bead
{
 public:
  bead (int id_in, glm::vec3& position_in)
   : id (id_in)
   , position (position_in)
   , orig_position (position_in)
   , dy (0.0f)
  {}

  void update (int counter)
  {
    if (counter > id)
    {
      dy += 0.2f;
      position.y -= dy;
      if (position.y < -ENGINE_GLUT_418_DEFAULT_HEIGHT / 3.0f)
      {
        position.y = -ENGINE_GLUT_418_DEFAULT_HEIGHT / 3.0f;
        dy *= -0.3f;
      } // end IF
    } // end IF
  }

  void arrange (int counter)
  {
    if (counter > id)
      position = glm::mix (position, orig_position, 0.1f);
  }

  int       id;
  glm::vec3 position;
  glm::vec3 orig_position;
  float     dy;
};

struct Engine_OpenGL_GLUT_418_CBData
{
  // context
  std::vector<bead>       beads;
  bool                    falling;
  int                     counter;
  float                   size;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mousex;
  int                     mousey;
  bool                    mouse_0_ButtonDown;
  float                   spin;
};

#endif // GLUT_418_H
