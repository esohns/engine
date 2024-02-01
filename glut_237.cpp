#include "stdafx.h"

#include "glut_237.h"

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

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines_10.h"
#include "engine_common.h"

void
engine_glut_237_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  100.0, -100.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_237_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_237_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 500.0f;
      cb_data_p->camera.position.z = 800.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      cb_data_p->ring += 1.0f;

      break;
    case GLUT_KEY_DOWN:
      if (cb_data_p->ring > 1.0f)
        cb_data_p->ring -= 1.0f;
      break;
  } // end SWITCH
}

void
engine_glut_237_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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
engine_glut_237_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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
engine_glut_237_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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
engine_glut_237_timer (int v)
{
  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_237_timer, v);
}

void
engine_glut_237_draw (void)
{
  static int half_width_i = ENGINE_GLUT_237_DEFAULT_WIDTH / 2;
  static int half_height_i = ENGINE_GLUT_237_DEFAULT_HEIGHT / 2;

  struct Engine_OpenGL_GLUT_237_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_237_CBData*> (glutGetWindowData ());
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

  //glTranslatef (static_cast<float> (half_width_i), static_cast<float> (half_height_i), 0.0f);
  glRotatef (cb_data_p->Yangle * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
  glRotatef (-90.0f * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);
  glRotatef (-10.0f * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
  for (float y = -ENGINE_GLUT_237_DEFAULT_RADIUS / 2.0f; y < ENGINE_GLUT_237_DEFAULT_RADIUS / 2.0f; y += std::round (cb_data_p->ring))
  {
    float rad_2 = cb_data_p->ring * std::sin (y + cb_data_p->offset);
    float t = 0.0f;
    while (t < 2.0f * static_cast<float> (M_PI))
    {
      float radius =
        static_cast<float> (std::sqrt (std::pow ((ENGINE_GLUT_237_DEFAULT_RADIUS - 40.0f) / 2.0f, 2) - std::pow (y, 2)));
      float x = radius * std::cos (t);
      float z = radius * std::sin (t);
      uint8_t color_f =
        static_cast<uint8_t> (Common_GL_Tools::map (std::sin (y + cb_data_p->offset), -1.0f, 1.0f, 0.0f, 255.0f));
      glColor3ub (color_f, color_f, color_f);

      glPushMatrix ();
      glTranslatef (x, y, z);

      glBegin (GL_TRIANGLE_FAN);
      glVertex3f (0.0f, 0.0f, 0.0f);
      for (float angle = 0.0f; angle < 2.0f * static_cast<float> (M_PI); angle += 0.1f)
      {
        float x2 = std::sin (angle) * rad_2;
        float y2 = std::cos (angle) * rad_2;
        glVertex3f (x2, y2, 0.0f);
      } // end FOR
      glEnd ();
      //glutSolidSphere (rad_2, 16, 40);

      glPopMatrix ();
      t += 0.1f;
    } // end WHILE
  } // end FOR
  cb_data_p->Yangle += ENGINE_GLUT_237_DEFAULT_SPEED;
  cb_data_p->offset += 0.1f;

  glutSwapBuffers ();
}

void
engine_glut_237_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_237_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_237_idle);
  else
    glutIdleFunc (NULL);
}
