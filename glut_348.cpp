#include "stdafx.h"

#include "glut_348.h"

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

#include "common_file_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_16.h"
#include "engine_common.h"

void
engine_glut_348_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  //ACE_ASSERT (height_in);
  //gluPerspective (45.0,
  //                width_in / static_cast<GLdouble> (height_in),
  //                -1.0, 1.0);
  glOrtho (-width_in / 2.0, width_in / 2.0, -height_in / 2.0, height_in / 2.0, 1.0, 800.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_348_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_348_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 750.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_348_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
engine_glut_348_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
engine_glut_348_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
engine_glut_348_timer (int v)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_348_timer, v);
}

void
engine_glut_348_draw (void)
{
  struct Engine_OpenGL_GLUT_348_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_348_CBData*> (glutGetWindowData ());
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
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glTranslatef (-ENGINE_GLUT_348_DEFAULT_WIDTH / 1.1f + 64.0f, -ENGINE_GLUT_348_DEFAULT_HEIGHT / 1.1f + 64.0f, 0.0f);

  glScalef (1.7f, 1.7f, 1.7f);

  Common_GL_Color_t color;
  color.r = 0xFF; color.g = 0xFF; color.b = 0x00;
  ring (ENGINE_GLUT_348_DEFAULT_WIDTH / 2.0f - 50.0f, ENGINE_GLUT_348_DEFAULT_HEIGHT / 2.0f - 40.0f, 0.0f, 100.0f, 50.0f, color, false);
  color.r = 0xFF; color.g = 0x00; color.b = 0xFF;
  ring (ENGINE_GLUT_348_DEFAULT_WIDTH / 2.0f + 50.0f, ENGINE_GLUT_348_DEFAULT_HEIGHT / 2.0f - 40.0f, 1.0f, 100.0f, 50.0f, color, false);
  color.r = 0x00; color.g = 0xFF; color.b = 0xFF;
  ring (ENGINE_GLUT_348_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_348_DEFAULT_HEIGHT / 2.0f + 45.0f,         0.5f, 100.0f, 50.0f, color, true);

  glutSwapBuffers ();
}

void
engine_glut_348_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_348_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_348_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
ring (float x, float y, float z, float radius, float size, Common_GL_Color_t& color, bool zosc)
{
  int steps = static_cast<int> (radius / 2.0f);
  for (int i = 0; i < steps; i++)
  {
    glPushMatrix ();

    if (zosc)
    {
      // z oscillation. Move forward and back to achieve trick. 
      glTranslatef (x, y, z + ((i / static_cast<int> (steps / 5.5f)) % 2 < 1 ? 2.0f : -2.0f));
      glRotatef ((180.0f / static_cast<float> (M_PI)) * (i + 11) * (2.0f * static_cast<float> (M_PI)) / static_cast<float> (steps), 0.0f, 0.0f, 1.0f);
    } // end IF
    else
    {
      glTranslatef (x, y, z);
      glRotatef ((180.0f / static_cast<float> (M_PI)) * i * (2.0f * static_cast<float> (M_PI)) / static_cast<float> (steps), 0.0f, 0.0f, 1.0f);
    } // end ELSE

    glColor3f (color.r / 255.0f, color.g / 255.0f, color.b / 255.0f);
    float height_f = steps * 0.33f;
    glRectf (radius - size / 2.0f, 0.0f - height_f / 2.0f, radius + size / 2.0f, 0.0f + height_f / 2.0f);

    glPopMatrix ();
  } // end FOR
}
