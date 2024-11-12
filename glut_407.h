#ifndef GLUT_407_H
#define GLUT_407_H

#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_407_reshape (int, int);
void engine_glut_407_key (unsigned char, int, int);
void engine_glut_407_key_special (int, int, int);
void engine_glut_407_menu (int);
void engine_glut_407_mouse_button (int, int, int, int);
void engine_glut_407_mouse_move (int, int);
void engine_glut_407_timer (int);
void engine_glut_407_draw (void);
void engine_glut_407_idle (void);
void engine_glut_407_visible (int);

struct Engine_OpenGL_GLUT_407_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             colorPhaseLoc;
  GLint                             displaceWeiLoc;
  GLint                             floorUVLoc;
  GLint                             UVScaleLoc;
  GLint                             seedLoc;

  Common_GL_Shader                  shader;

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;

  std::vector<float>                colorPhase;
  float                             displaceWei;
  int                               floorUVMode;
  int                               UVScale;
  std::vector<float>                seed;
};

#endif // GLUT_407_H
