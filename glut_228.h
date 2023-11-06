#ifndef GLUT_228_H
#define GLUT_228_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"

// other functions
void addBall (struct Engine_OpenGL_GLUT_228_CBData&, float, float, float);
void updateBalls (struct Engine_OpenGL_GLUT_228_CBData&);
void clear (struct Engine_OpenGL_GLUT_228_CBData&);

// GLUT routines
void engine_glut_228_reshape (int, int);
void engine_glut_228_key (unsigned char, int, int);
void engine_glut_228_key_special (int, int, int);
void engine_glut_228_menu (int);
void engine_glut_228_mouse_button (int, int, int, int);
void engine_glut_228_mouse_move (int, int);
void engine_glut_228_timer (int);
void engine_glut_228_draw (void);
void engine_glut_228_idle (void);
void engine_glut_228_visible (int);

struct Engine_OpenGL_GLUT_228_CBData
{
  //
  std::vector<float>      ballsX;
  std::vector<float>      ballsY;
  std::vector<float>      radii;
  std::vector<float>      velocX;
  std::vector<float>      velocY;
  float                   distScale;
  float                   colorOffset;

  // canvas
  int                     columns;
  int                     rows;
  int                     scaleFactor;

  // shader
  GLint                   programId;
  GLint                   ballsXLoc;
  GLint                   ballsYLoc;
  GLint                   radiiLoc;
  GLint                   ballsLoc;
  GLint                   distScaleLoc;
  GLint                   colorOffsetLoc;
  GLint                   widthLoc;
  GLint                   heightLoc;

  // menu
  bool                    wireframe;

  // camera
  struct Common_GL_Camera camera;

  // mouse
  bool                    leftButtonIsDown;
};

#endif // GLUT_228_H
