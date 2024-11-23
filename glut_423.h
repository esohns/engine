#ifndef GLUT_423_H
#define GLUT_423_H

#include "noise/noise.h"

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

// GLUT routines
void engine_glut_423_reshape (int, int);
void engine_glut_423_key (unsigned char, int, int);
void engine_glut_423_key_special (int, int, int);
void engine_glut_423_menu (int);
void engine_glut_423_mouse_button (int, int, int, int);
void engine_glut_423_mouse_move (int, int);
void engine_glut_423_timer (int);
void engine_glut_423_draw (void);
void engine_glut_423_idle (void);
void engine_glut_423_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_423_CBData
{
  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;

  // mouse
  int                     mousex;
  int                     mousey;
  bool                    mouse_0_ButtonDown;

  // noise
  noise::module::Perlin   noise;
};

#endif // GLUT_423_H
