#include "stdafx.h"

#include "glut_460.h"

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

#include "defines_21.h"
#include "engine_common.h"

void
engine_glut_460_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (ENGINE_GLUT_460_DEFAULT_FOV,
                  width_in / static_cast<GLdouble> (height_in),
                  ENGINE_GLUT_460_DEFAULT_ZNEAR, ENGINE_GLUT_460_DEFAULT_ZFAR);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_460_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_460_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      cb_data_p->camera.left (200.0f);
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.right (200.0f);
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position_.x = 0.0f;
      cb_data_p->camera.position_.y = 0.0f;
      cb_data_p->camera.position_.z = 1500.0f;
      break;
  } // end SWITCH
}

void
engine_glut_460_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
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
engine_glut_460_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
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
engine_glut_460_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_460_timer (int v)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 60,
                 engine_glut_460_timer,
                 v);
}

void
engine_glut_460_draw (void)
{
  struct Engine_OpenGL_GLUT_460_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_460_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  cb_data_p->camera.mouseLook (cb_data_p->mouseX, cb_data_p->mouseY);

  static int frame_count_i = 1;
#if defined (GLM_SUPPORT)
  glm::mat4 model_matrix = glm::mat4 (1.0f); // make sure to initialize matrix to identity matrix first
  model_matrix = glm::rotate (model_matrix,
                              frame_count_i * 0.002f, // radians
                              glm::vec3 (0.0f, 1.0f, 0.0f));

  glm::mat4 view_matrix = glm::lookAt (cb_data_p->camera.position_,
                                       cb_data_p->camera.looking_at_,
                                       cb_data_p->camera.up_);

  glm::mat4 projection_matrix =
    glm::perspective (glm::radians (static_cast<float> (ENGINE_GLUT_460_DEFAULT_FOV)),
                      ENGINE_GLUT_460_DEFAULT_WIDTH / static_cast<float> (ENGINE_GLUT_460_DEFAULT_HEIGHT),
                      static_cast<float> (ENGINE_GLUT_460_DEFAULT_ZNEAR), static_cast<float> (ENGINE_GLUT_460_DEFAULT_ZFAR));
#endif // GLM_SUPPORT

  // update uniforms
  cb_data_p->shader.setMat4 (ACE_TEXT_ALWAYS_CHAR ("uProjectionMatrix"), projection_matrix);
  cb_data_p->shader.setMat4 (ACE_TEXT_ALWAYS_CHAR ("uModelViewMatrix"), view_matrix * model_matrix);

  glBindVertexArray (cb_data_p->VAO);

  glDrawArrays (GL_POINTS, 0, cb_data_p->numPoints);

  glBindVertexArray (0);

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_460_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_460_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_460_idle);
  else
    glutIdleFunc (NULL);
}
