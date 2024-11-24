#ifndef GLUT_425_H
#define GLUT_425_H

#include <chrono>
#include <vector>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_common.h"
#include "common_gl_shader.h"

#include "engine_common.h"

// GLUT routines
void engine_glut_425_reshape (int, int);
void engine_glut_425_key (unsigned char, int, int);
void engine_glut_425_key_special (int, int, int);
void engine_glut_425_menu (int);
void engine_glut_425_mouse_button (int, int, int, int);
void engine_glut_425_mouse_move (int, int);
void engine_glut_425_timer (int);
void engine_glut_425_draw (void);
void engine_glut_425_idle (void);
void engine_glut_425_visible (int);

struct Engine_OpenGL_GLUT_425_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  GLint                             resolutionLoc;
  GLint                             timeLoc;
  GLint                             skewFactorLoc;
  GLint                             rotationSpeedLoc;
  GLint                             wobblynessLoc;
  GLint                             subDivisionsLoc;
  GLint                             paletteLoc;
  GLint                             spiralFactorLoc;
  GLint                             spiralSpeedLoc;
  GLint                             xOffLoc;
  GLint                             yOffLoc;
  GLint                             numColorsLoc;
  GLint                             blendColorsLoc;
  GLint                             blendSameLoc;

  Common_GL_Shader                  shader;

  // time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp1;
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp1;
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX

  // menu
  bool                              wireframe;

  // camera
  struct Engine_GL_Camera           camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
  bool                              mouseLMBPressed;

  // palette
  int                               paletteIndex;

  // parameters
  float                             skewFactor;
  float                             rotationSpeed;
  float                             wobblyness;
  float                             subDivisions;
  std::vector<Common_GL_Color_t>    palette;
  float                             spiralFactor;
  float                             spiralSpeed;
  float                             xOff;
  float                             yOff;
  int                               numColors;
  bool                              blendColors;
  bool                              blendSame;
};

//////////////////////////////////////////

void randomizeParameters (struct Engine_OpenGL_GLUT_425_CBData&);

#endif // GLUT_425_H
