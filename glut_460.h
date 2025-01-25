#ifndef GLUT_460_H
#define GLUT_460_H

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64

#include "common_gl_camera.h"
#include "common_gl_shader.h"

// GLUT routines
void engine_glut_460_reshape (int, int);
void engine_glut_460_key (unsigned char, int, int);
void engine_glut_460_key_special (int, int, int);
void engine_glut_460_menu (int);
void engine_glut_460_mouse_button (int, int, int, int);
void engine_glut_460_mouse_move (int, int);
void engine_glut_460_timer (int);
void engine_glut_460_draw (void);
void engine_glut_460_idle (void);
void engine_glut_460_visible (int);

struct Engine_OpenGL_GLUT_460_CBData
{
  // shader
  Common_GL_Shader                  shader;
  GLint                             positionLoc;
  GLint                             colorLoc;
  GLint                             modelViewLoc;
  GLint                             projectionLoc;

  GLuint                            VAO;
  GLuint                            VBO;

  // scene
  GLuint                            numPoints;

  // menu
  bool                              wireframe;

  // camera
  Common_GL_Camera                  camera;

  // mouse
  int                               mouseX;
  int                               mouseY;
};

#endif // GLUT_460_H
