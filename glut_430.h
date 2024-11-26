#ifndef GLUT_430_H
#define GLUT_430_H

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

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#include "common_gl_common.h"

#include "engine_common.h"

#include "defines_20.h"

// GLUT routines
void engine_glut_430_reshape (int, int);
void engine_glut_430_key (unsigned char, int, int);
void engine_glut_430_key_special (int, int, int);
void engine_glut_430_menu (int);
void engine_glut_430_mouse_button (int, int, int, int);
void engine_glut_430_mouse_move (int, int);
void engine_glut_430_timer (int);
void engine_glut_430_draw (void);
void engine_glut_430_idle (void);
void engine_glut_430_visible (int);

//////////////////////////////////////////

class scale
{
 public:
  scale (glm::vec3& pos, glm::vec3& ang, Common_GL_Color_t& col)
   : position (pos)
   , angle (ang)
   , color (col)
  {}

  void draw (glm::vec3& twist, float wiggle)
  {
    //shininess(5);
    glColor3ub (color.r, color.g, color.b);

    glPushMatrix ();
    glRotatef ((twist.y * position.x / 1000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef ((twist.z * position.x / 2000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
    if (position.x > 0.0f)
      glRotatef ((wiggle * position.x / 2000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glTranslatef (position.x, position.y, position.z);
    glRotatef (angle.y * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef (-angle.x * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);

    glBegin (GL_LINE_STRIP);
    for (float angle_f = 0.0f; angle_f <= 2.0f * static_cast<float> (M_PI); angle_f += (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_GLUT_430_DEFAULT_ELLIPSE_SEGMENTS)))
    {
      float x = std::cos (angle_f) * 15.0f;
      float y = std::sin (angle_f) * 15.0f;
      glVertex3f (x, y, 0.0f);
    } // end FOR
    glEnd ();
    glPopMatrix ();

    glPushMatrix ();
    glRotatef ((twist.y * position.x / 1000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef ((twist.z * position.x / 2000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
    if (position.x > 0.0f)
      glRotatef ((wiggle * position.x / 2000.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glTranslatef (position.x, position.y, -position.z);
    glRotatef (-angle.y * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glRotatef (angle.x * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);
    
    glBegin (GL_LINE_STRIP);
    for (float angle_f = 0.0f; angle_f <= 2.0f * static_cast<float> (M_PI); angle_f += (2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_GLUT_430_DEFAULT_ELLIPSE_SEGMENTS)))
    {
      float x = std::cos (angle_f) * 15.0f;
      float y = std::sin (angle_f) * 15.0f;
      glVertex3f (x, y, 0.0f);
    } // end FOR
    glEnd ();
    glPopMatrix ();
  }

  glm::vec3         position;
  glm::vec3         angle;
  Common_GL_Color_t color;
};

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_430_CBData
{
  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mousex;
  int                     mousey;
  bool                    mouse_0_ButtonDown;

  // scene
  glm::vec3               orient;
  glm::vec3               otarget;
  glm::vec3               twist;
  glm::vec3               ttwist;
  glm::vec3               tpos;
  glm::vec3               tvel;
  float                   amp;
  float                   wiggle;
  std::vector<scale>      scales;
};

//////////////////////////////////////////

void makeScales (struct Engine_OpenGL_GLUT_430_CBData&);
void drawFish (struct Engine_OpenGL_GLUT_430_CBData&);

#endif // GLUT_430_H
