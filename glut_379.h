#ifndef GLUT_379_H
#define GLUT_379_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_379_reshape (int, int);
void engine_glut_379_key (unsigned char, int, int);
void engine_glut_379_key_special (int, int, int);
void engine_glut_379_menu (int);
void engine_glut_379_mouse_button (int, int, int, int);
void engine_glut_379_mouse_move (int, int);
void engine_glut_379_timer (int);
void engine_glut_379_draw (void);
void engine_glut_379_idle (void);
void engine_glut_379_visible (int);

struct Engine_OpenGL_GLUT_379_CBData
{
  // canvas
  int                               columns;
  int                               rows;

  // shader
  GLint                             programId;
  GLint                             positionLoc;
  GLint                             texCoordLoc;
  GLint                             randomVecLoc;
  GLint                             timeLoc;

  GLuint                            VAO;
  GLuint                            VBO;

  float                             randomVec[2];

  // time
  float                             time;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_379_H
