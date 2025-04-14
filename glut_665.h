#ifndef GLUT_665_H
#define GLUT_665_H

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
void engine_glut_665_reshape (int, int);
void engine_glut_665_key (unsigned char, int, int);
void engine_glut_665_key_up (unsigned char, int, int);
void engine_glut_665_key_special (int, int, int);
void engine_glut_665_key_special_up (int, int, int);
void engine_glut_665_menu (int);
void engine_glut_665_mouse_button (int, int, int, int);
void engine_glut_665_mouse_move (int, int);
void engine_glut_665_timer (int);
void engine_glut_665_draw (void);
void engine_glut_665_idle (void);
void engine_glut_665_visible (int);

struct Engine_OpenGL_GLUT_665_CBData
{
  // shader
  Common_GL_Shader                  shader1;
  Common_GL_Shader                  shader2;
  Common_GL_Shader                  shader3;
  Common_GL_Shader                  shader4;

  GLint                             S1resolutionLoc;
  GLint                             S1timeLoc;

  GLint                             S2resolutionLoc;
  GLint                             S2channel0Loc;

  GLint                             S3resolutionLoc;
  GLint                             S3channel1Loc;

  GLint                             S4resolutionLoc;
  GLint                             S4channel0Loc;
  GLint                             S4channel1Loc;
  GLint                             S4channel2Loc;

  Common_GL_Texture                 textureS1;
  Common_GL_Texture                 textureS2;
  Common_GL_Texture                 textureS3;

  GLuint                            FBO1;
  GLuint                            FBO2;
  GLuint                            FBO3;

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

#endif // GLUT_665_H
