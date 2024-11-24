#ifndef GLUT_426_H
#define GLUT_426_H

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
void engine_glut_426_reshape (int, int);
void engine_glut_426_key (unsigned char, int, int);
void engine_glut_426_key_special (int, int, int);
void engine_glut_426_menu (int);
void engine_glut_426_mouse_button (int, int, int, int);
void engine_glut_426_mouse_move (int, int);
void engine_glut_426_timer (int);
void engine_glut_426_draw (void);
void engine_glut_426_idle (void);
void engine_glut_426_visible (int);

//////////////////////////////////////////

struct Engine_OpenGL_GLUT_426_CBData
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
  float                   f;
};

#endif // GLUT_426_H
