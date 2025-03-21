#ifndef GLUT_478_H
#define GLUT_478_H

#include <chrono>

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
void engine_glut_478_reshape (int, int);
void engine_glut_478_key (unsigned char, int, int);
void engine_glut_478_key_special (int, int, int);
void engine_glut_478_menu (int);
void engine_glut_478_mouse_button (int, int, int, int);
void engine_glut_478_mouse_move (int, int);
void engine_glut_478_timer (int);
void engine_glut_478_draw (void);
void engine_glut_478_idle (void);
void engine_glut_478_visible (int);

struct Engine_OpenGL_GLUT_478_CBData
{
  // canvas
  int                               columns;
  int                               rows;
  int                               scaleFactor;

  // shader
  Common_GL_Shader                  shader1;
  Common_GL_Shader                  shader2;
  GLint                             S1resolutionLoc;
  GLint                             S1timeLoc;
  GLint                             S1channel0Loc;
  GLint                             S1channel1Loc;
  GLint                             S1channel2Loc;
  GLint                             S2resolutionLoc;
  GLint                             S2channel0Loc;
  Common_GL_Texture                 texture0;
  Common_GL_Texture                 texture1;
  Common_GL_Texture                 texture2;
  Common_GL_Texture                 textureS1;

  GLuint                            FBO;
  GLuint                            VAO;
  GLuint                            VAO_2;
  GLuint                            VBO;
  GLuint                            VBO_2;

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
};

#endif // GLUT_478_H
