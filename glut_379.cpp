#include "stdafx.h"

#include "glut_379.h"

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

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines_17.h"
#include "engine_common.h"

void
engine_glut_379_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  150.0, -150.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_379_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_379_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      cb_data_p->camera.rotation.z -= 0.5f;
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.rotation.z += 0.5f;
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 500.0f;

      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_379_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (entry_in)
  {
    case 0:
      cb_data_p->wireframe = !cb_data_p->wireframe; break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_379_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
      } // end IF

      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_379_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_379_timer (int v)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_379_timer,
                 v);
}

void
engine_glut_379_draw (void)
{
  struct Engine_OpenGL_GLUT_379_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_379_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT);

  // update uniforms
  static float randomVec2[2] = { Common_Tools::getRandomNumber (0.0f, 300.0f), Common_Tools::getRandomNumber (0.0f, 300.0f) };
  glProgramUniform2fv (cb_data_p->programId, cb_data_p->randomVecLoc, 1, static_cast<GLfloat*> (randomVec2));
  glProgramUniform1f (cb_data_p->programId, cb_data_p->timeLoc,
                      static_cast<GLfloat> (cb_data_p->time));

  glBindVertexArray (cb_data_p->VAO);

  glDrawArrays (GL_POINTS, 0, cb_data_p->columns * cb_data_p->rows);

  glBindVertexArray (0);

  cb_data_p->time += 0.02f;

  glutSwapBuffers ();
}

void
engine_glut_379_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_379_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_379_idle);
  else
    glutIdleFunc (NULL);
}
