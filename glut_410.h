#ifndef GLUT_410_H
#define GLUT_410_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"
#include "common_gl_texture.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_410_reshape (int, int);
void engine_glut_410_key (unsigned char, int, int);
void engine_glut_410_key_special (int, int, int);
void engine_glut_410_menu (int);
void engine_glut_410_mouse_button (int, int, int, int);
void engine_glut_410_mouse_move (int, int);
void engine_glut_410_timer (int);
void engine_glut_410_draw (void);
void engine_glut_410_idle (void);
void engine_glut_410_visible (int);

struct Engine_OpenGL_GLUT_410_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             imageLoc;
  GLint                             mouseLoc;

  Common_GL_Shader                  shader;
  Common_GL_Texture                 texture;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_410_H
