#ifndef GLUT_511_H
#define GLUT_511_H

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
void engine_glut_511_reshape (int, int);
void engine_glut_511_key (unsigned char, int, int);
void engine_glut_511_key_up (unsigned char, int, int);
void engine_glut_511_key_special (int, int, int);
void engine_glut_511_key_special_up (int, int, int);
void engine_glut_511_menu (int);
void engine_glut_511_mouse_button (int, int, int, int);
void engine_glut_511_mouse_move (int, int);
void engine_glut_511_timer (int);
void engine_glut_511_draw (void);
void engine_glut_511_idle (void);
void engine_glut_511_visible (int);

struct Engine_OpenGL_GLUT_511_CBData
{
  // shader
  Common_GL_Shader                  shader1;
  Common_GL_Shader                  shader2;
  Common_GL_Shader                  shader3;

  GLint                             S1resolutionLoc;
  //GLint                             S1channel0Loc;

  GLint                             S2resolutionLoc;
  GLint                             S2channel0Loc;

  GLint                             S3resolutionLoc;
  GLint                             S3channel0Loc;
  GLint                             S3channel1Loc;

  Common_GL_Texture                 textureS1;
  Common_GL_Texture                 textureS2;

  GLuint                            FBO1;
  GLuint                            FBO2;

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

#endif // GLUT_511_H
