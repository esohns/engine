#ifndef GLUT_600_H
#define GLUT_600_H

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
void engine_glut_600_reshape (int, int);
void engine_glut_600_key (unsigned char, int, int);
void engine_glut_600_key_up (unsigned char, int, int);
void engine_glut_600_key_special (int, int, int);
void engine_glut_600_key_special_up (int, int, int);
void engine_glut_600_menu (int);
void engine_glut_600_mouse_button (int, int, int, int);
void engine_glut_600_mouse_move (int, int);
void engine_glut_600_timer (int);
void engine_glut_600_draw (void);
void engine_glut_600_idle (void);
void engine_glut_600_visible (int);

struct Engine_OpenGL_GLUT_600_CBData
{
  // shader
  Common_GL_Shader                  shader1;
  Common_GL_Shader                  shader2;
  Common_GL_Shader                  shader3;

  GLint                             S1resolutionLoc;
  GLint                             S1frameLoc;
  GLint                             S1mouseLoc;
  GLint                             S1channel0Loc;

  GLint                             S2resolutionLoc;
  GLint                             S2frameLoc;
  GLint                             S2channel0Loc;
  GLint                             S2channel1Loc;

  GLint                             S3resolutionLoc;
  GLint                             S3timeLoc;
  GLint                             S3frameLoc;
  GLint                             S3channel0Loc;
  GLint                             S3channel1Loc;
  GLint                             S3channel2Loc;

  Common_GL_Texture                 texture0;
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

#endif // GLUT_600_H
