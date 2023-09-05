#ifndef GLUT_169_H
#define GLUT_169_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// GLUT routines
void engine_glut_169_reshape (int, int);
void engine_glut_169_key (unsigned char, int, int);
void engine_glut_169_key_special (int, int, int);
void engine_glut_169_menu (int);
void engine_glut_169_mouse_button (int, int, int, int);
void engine_glut_169_mouse_move (int, int);
void engine_glut_169_timer (int);
void engine_glut_169_draw (void);
void engine_glut_169_idle (void);
void engine_glut_169_visible (int);

struct Engine_OpenGL_GLUT_169_CBData
{
  // geometry
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   resolutionLoc;
  GLint                   mouseLoc;
  GLint                   frameCountLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  float                   angle;
  float                   deltaAngle;
  int                     xOrigin;
};

#endif // GLUT_132_H
