#include "stdafx.h"

#include "glut_236.h"

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

#include "common_image_tools.h"

#include "defines_10.h"
#include "engine_common.h"

void
engine_glut_236_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_236_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_236_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
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
engine_glut_236_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
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
engine_glut_236_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_UP)
    {
      cb_data_p->angle += cb_data_p->deltaAngle;
      cb_data_p->xOrigin = -1;
    } // end IF
    else
      cb_data_p->xOrigin = x;
  } // end IF
}

void
engine_glut_236_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // this will only be true when the left button is down
	if (cb_data_p->xOrigin >= 0)
  {
    cb_data_p->deltaAngle = (x - cb_data_p->xOrigin) * 0.001f;

    // update camera's direction
    //cb_data_p->camera.looking_at.x = sin (cb_data_p->angle + cb_data_p->deltaAngle);
    //cb_data_p->camera.looking_at.z = -cos (cb_data_p->angle + cb_data_p->deltaAngle);
  } // end IF
}

void
engine_glut_236_timer (int v)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    cb_data_p->angle += 1.0f;
    if (cb_data_p->angle > 360.0f)
    {
      cb_data_p->angle -= 360.0f;
    }
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_236_timer, v);
}

void
engine_glut_236_draw (void)
{
  struct Engine_OpenGL_GLUT_236_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_236_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  // rotate the camera
  glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
                                           glm::radians (cb_data_p->camera.rotation.z),
                                           glm::vec3 (0.0f, 0.0f, 1.0f));
  glm::vec3 rotation_center (0.0f, 0.0f, 0.0f);
  glm::vec4 pos_rot_h =
    rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center,
                                 1.0f);
  cb_data_p->camera.position = glm::vec3 (pos_rot_h) + rotation_center;

  // set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor4f (1.0f, 0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor4f (0.0f, 1.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor4f (0.0f, 0.0f, 1.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glRotatef (12.0f * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);

  int n = 0; // Rings counter
  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI); i += static_cast<float> (M_PI) / 16.0f)
  {
    glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

    glPushMatrix ();
    glRotatef (i * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glTranslatef (120.0f, 0.0f, 0.0f);
    cb_data_p->wireframe ? glutWireTorus  (2.0f, std::cos (i * 4.0f + cb_data_p->f) * 20.0f + 20.0f, 16, 40)
                         : glutSolidTorus (2.0f, std::cos (i * 4.0f + cb_data_p->f) * 20.0f + 20.0f, 16, 40);
    glPopMatrix ();

    glColor4f (0.0f, 1.0f, 0.0f, 1.0f);

    glPushMatrix ();
    glRotatef ((i - cb_data_p->f / 4.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
    glTranslatef (120.0f, 0.0f, 0.0f);
    if (n % 8 == 0)
      cb_data_p->wireframe ? glutWireSphere  (25.0, 16, 40)
                           : glutSolidSphere (25.0, 16, 40);
    glPopMatrix ();
    n++;
  } // end FOR

  cb_data_p->f += 0.03f;

  glutSwapBuffers ();
}

void
engine_glut_236_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_236_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_236_idle);
  else
    glutIdleFunc (NULL);
}
