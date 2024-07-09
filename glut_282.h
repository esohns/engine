#ifndef GLUT_282_H
#define GLUT_282_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "engine_common.h"

// GLUT routines
void engine_glut_282_reshape (int, int);
void engine_glut_282_key (unsigned char, int, int);
void engine_glut_282_key_special (int, int, int);
void engine_glut_282_menu (int);
void engine_glut_282_mouse_button (int, int, int, int);
void engine_glut_282_mouse_move (int, int);
void engine_glut_282_timer (int);
void engine_glut_282_draw (void);
void engine_glut_282_idle (void);
void engine_glut_282_visible (int);

struct Engine_OpenGL_GLUT_282_CBData
{
  //

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   pixelDensityLoc;
  GLint                   canvasLoc;
  GLint                   mouseLoc;
  GLint                   timeLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Engine_GL_Camera camera;
};

#endif // GLUT_282_H
