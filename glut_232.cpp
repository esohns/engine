#include "stdafx.h"

#include "glut_232.h"

#if defined (GLEW_SUPPORT)
#include "GL/glew.h"
#endif // GLEW_SUPPORT
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

#include "defines_10.h"
#include "engine_common.h"

void
engine_glut_232_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  //ACE_ASSERT (height_in);
  //gluPerspective (45.0,
  //                width_in / static_cast<GLdouble> (height_in),
  //                -100.0, 100.0);
  glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
           static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), -100.0, 100.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_232_key (unsigned char key_in,
                     int x,
                     int y)
{
  switch (key_in)
  {
    case 27:  /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_232_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
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
engine_glut_232_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
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
engine_glut_232_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_232_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_232_timer (int v)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_232_timer,
                 v);
}

void
engine_glut_232_draw (void)
{
  struct Engine_OpenGL_GLUT_232_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_232_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  // rotate the camera
  //glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
  //                                         glm::radians (cb_data_p->camera.rotation.z),
  //                                         glm::vec3 (0.0f, 0.0f, 1.0f));
  //glm::vec3 rotation_center (0.0f, 0.0f, 0.0f);
  //glm::vec4 pos_rot_h =
  //  rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center,
  //                               1.0f);
  //cb_data_p->camera.position = glm::vec3 (pos_rot_h) + rotation_center;

  // set the camera
  //gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
  //           cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
  //           cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  for (int i3 = 0; i3 < 6; i3++)
  {
    glPushMatrix ();
    glRotatef ((i3 * static_cast<float> (M_PI) / 3.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
    for (std::vector<object>::iterator iterator = cb_data_p->objects.begin ();
         iterator != cb_data_p->objects.end ();
         ++iterator)
    {
      (*iterator).move ();
      (*iterator).show ();
    } // end FOR
    glPopMatrix ();
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_232_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_232_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_232_idle);
  else
    glutIdleFunc (NULL);
}
