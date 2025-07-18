﻿#include "stdafx.h"

#include "glut_908.h"

#include "GL/glew.h"
#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"
#include "common_file_tools.h"

#include "common_time_common.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines_43.h"
#include "engine_common.h"

void
engine_glut_908_reshape (int width_in,
                         int height_in)
{
  glViewport (0, 0, width_in, height_in);

  //glMatrixMode (GL_PROJECTION);

  //glLoadIdentity ();

  //ACE_ASSERT (height_in);
  //gluPerspective (45.0,
  //                width_in / static_cast<GLdouble> (height_in),
  //                0.1, -1000.0);
  ////glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  ////         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  //glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_908_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;

    case 'p':
      cb_data_p->isPKeyPressed = !cb_data_p->isPKeyPressed;
      break;
    case 'a':
      cb_data_p->isAKeyPressed = !cb_data_p->isAKeyPressed;
      break;
    case 'r':
      cb_data_p->isRKeyPressed = !cb_data_p->isRKeyPressed;
      break;
    case 'c':
      cb_data_p->isCKeyPressed = !cb_data_p->isCKeyPressed;
      break;

    case 'g':
      cb_data_p->isGKeyPressed = !cb_data_p->isGKeyPressed;
      break;
    case 'b':
      cb_data_p->isBKeyPressed = !cb_data_p->isBKeyPressed;
      break;
    case 'd':
      cb_data_p->isDKeyPressed = !cb_data_p->isDKeyPressed;
      break;
    case 'l':
      cb_data_p->isLKeyPressed = !cb_data_p->isLKeyPressed;
      break;
  } // end SWITCH
}

void
engine_glut_908_key_up (unsigned char key_in,
                        int x,
                        int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 32: /* Space */
      //cb_data_p->spacePressed = false;
      break;
  } // end SWITCH
}

void
engine_glut_908_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      break;
    case GLUT_KEY_RIGHT:
      break;
    case GLUT_KEY_UP:
      break;
  } // end SWITCH
}

void
engine_glut_908_key_special_up (int key_in,
                                int x,
                                int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      //cb_data_p->leftPressed = false;
      break;
    case GLUT_KEY_RIGHT:
      //cb_data_p->rightPressed = false;
      break;
    case GLUT_KEY_UP:
      //cb_data_p->upPressed = false;
      break;
  } // end SWITCH
}

void
engine_glut_908_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (entry_in)
  {
    case 0:
      cb_data_p->wireframe = !cb_data_p->wireframe;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_908_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      cb_data_p->mouseLMBPressed = (state == GLUT_DOWN);
      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_908_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_908_timer (int v)
{
  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 60,
                 engine_glut_908_timer,
                 v);
}

void
engine_glut_908_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_908_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_908_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  //glMatrixMode (GL_MODELVIEW);
  //glLoadIdentity ();

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 = std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 = std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - cb_data_p->tp1;
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);

  cb_data_p->shader.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader.id_, cb_data_p->resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_908_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_908_DEFAULT_HEIGHT));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  glProgramUniform4f (cb_data_p->shader.id_, cb_data_p->mouseLoc,
                      static_cast<GLfloat> (cb_data_p->mouseX),
                      static_cast<GLfloat> (Common_GL_Tools::map (cb_data_p->mouseY, 0, ENGINE_GLUT_908_DEFAULT_HEIGHT - 1, ENGINE_GLUT_908_DEFAULT_HEIGHT - 1, 0)),
                      static_cast<GLfloat> (cb_data_p->mouseLMBPressed ? 1 : 0),
                      static_cast<GLfloat> (0));

  glProgramUniform4i (cb_data_p->shader.id_, cb_data_p->keys0Loc,
                      static_cast<GLint> (cb_data_p->isPKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isAKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isRKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isCKeyPressed ? 1 : 0));
  glProgramUniform4i (cb_data_p->shader.id_, cb_data_p->keys1Loc,
                      static_cast<GLint> (cb_data_p->isGKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isBKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isDKeyPressed ? 1 : 0),
                      static_cast<GLint> (cb_data_p->isLKeyPressed ? 1 : 0));

  glColor3f (1.0f, 1.0f, 1.0f);
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> (y * cb_data_p->scaleFactor), 0.0f);
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ((y + 1) * cb_data_p->scaleFactor), 0.0f);
    } // end FOR
    glEnd ();
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_908_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_908_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_908_idle);
  else
    glutIdleFunc (NULL);
}
