#ifndef GLUT_670_H
#define GLUT_670_H

#include <chrono>

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_shader.h"
#include "common_gl_texture.h"

//#include "engine_common.h"

// GLUT routines
void engine_glut_670_reshape (int, int);
void engine_glut_670_key (unsigned char, int, int);
void engine_glut_670_key_up (unsigned char, int, int);
void engine_glut_670_key_special (int, int, int);
void engine_glut_670_key_special_up (int, int, int);
void engine_glut_670_menu (int);
void engine_glut_670_mouse_button (int, int, int, int);
void engine_glut_670_mouse_move (int, int);
void engine_glut_670_timer (int);
void engine_glut_670_draw (void);
void engine_glut_670_idle (void);
void engine_glut_670_visible (int);

struct Engine_OpenGL_GLUT_670_CBData
{
  // shader
  Common_GL_Shader                  shader1;
  Common_GL_Shader                  shader2;

  GLint                             S1resolutionLoc;
  GLint                             S1timeLoc;

  GLint                             S2resolutionLoc;
  GLint                             S2timeLoc;
  GLint                             S2mouseLoc;
  GLint                             S2channel0Loc;
  GLint                             S2channel1Loc;

  Common_GL_Texture                 texture0;
  Common_GL_Texture                 textureS1;

  GLuint                            FBO1;

  GLuint                            VAO;
  GLuint                            VBO;

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

  // mouse / keyboard
  int                               mouseX;
  int                               mouseY;
  bool                              mouseLMBPressed;
};

#endif // GLUT_670_H
