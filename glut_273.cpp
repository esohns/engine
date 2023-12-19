#include "stdafx.h"

#include "glut_273.h"

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

#include "defines_12.h"
#include "engine_common.h"

void
engine_glut_273_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  1.0, -1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_273_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_273_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_PAGE_UP:
      cb_data_p->camera.position.z -= 100.0f;
      break;
    case GLUT_KEY_PAGE_DOWN:
      cb_data_p->camera.position.z += 100.0f;
      break;
    case GLUT_KEY_LEFT:
      cb_data_p->camera.rotation.z -= 0.5f;
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.rotation.z += 0.5f;
      break;
    case GLUT_KEY_UP:
    {
      cb_data_p->tip = 0.0f;
      cb_data_p->angle = 0.0f;
      cb_data_p->angleVelocity = 0.0f;

      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 200.0f;
      cb_data_p->camera.position.z = 750.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    }
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_273_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
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
engine_glut_273_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
        //cb_data_p->angle += cb_data_p->deltaAngleX;
        cb_data_p->xOrigin = -1;
        cb_data_p->zOrigin = -1;
      } // end IF
      else
      {
        cb_data_p->style++;
        if (cb_data_p->style >= 4)
          cb_data_p->style = 0;

        cb_data_p->xOrigin = x;
      } // end ELSE
      break;
    }
    case 3:
      cb_data_p->camera.position.y -= 10.0f;
      cb_data_p->camera.position.z -= 10.0f;
      break;
    case 4:
      cb_data_p->camera.position.y += 10.0f;
      cb_data_p->camera.position.z += 10.0f;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_273_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->tip =
    Common_Math_Tools::lerp (cb_data_p->tip, Common_GL_Tools::map (static_cast<float> (y), 0.0f, static_cast<float> (ENGINE_GLUT_273_DEFAULT_HEIGHT), static_cast<float> (-M_PI), static_cast<float> (M_PI)), 0.1f);

  cb_data_p->angleVelocity =
    Common_Math_Tools::lerp (cb_data_p->angleVelocity, Common_GL_Tools::map (static_cast<float> (x), 0.0f, static_cast<float> (ENGINE_GLUT_273_DEFAULT_WIDTH), -1.0f, 1.0f), 0.05f);
  if (std::abs (static_cast<float> (x) - static_cast<float> (ENGINE_GLUT_273_DEFAULT_WIDTH) / 2.0f) < static_cast<float> (ENGINE_GLUT_273_DEFAULT_WIDTH) / 10.0f)
    cb_data_p->angleVelocity *= 0.9f;

  // this will only be true when the left button is down
	if (cb_data_p->xOrigin >= 0)
  {
    cb_data_p->deltaAngleX = (x - cb_data_p->xOrigin) * 0.001f;
  } // end IF
}

void
engine_glut_273_timer (int v)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    //cb_data_p->angle += 1.0f;
    //if (cb_data_p->angle > 360.0f)
    //{
    //  cb_data_p->angle -= 360.0f;
    //}
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_273_timer, v);
}

void
engine_glut_273_draw (void)
{
  struct Engine_OpenGL_GLUT_273_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_273_CBData*> (glutGetWindowData ());
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

  glPolygonMode (GL_FRONT_AND_BACK, cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor4f (1.0f, 0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor4f (0.0f, 1.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor4f (0.0f, 0.0f, 1.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

  // pointLight(0, 0, 200, height / 8, -height, height);
  static GLfloat light_position_a[] = {static_cast<float> (ENGINE_GLUT_273_DEFAULT_HEIGHT) / 8.0f, static_cast<float> (-ENGINE_GLUT_273_DEFAULT_HEIGHT), static_cast<float> (ENGINE_GLUT_273_DEFAULT_HEIGHT)};
  static GLfloat light_position_2[] = {static_cast<float> (ENGINE_GLUT_273_DEFAULT_HEIGHT) / 8.0f, static_cast<float> (-ENGINE_GLUT_273_DEFAULT_HEIGHT) / 8.0f, static_cast<float> (ENGINE_GLUT_273_DEFAULT_HEIGHT) / 8.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  static GLint light_ambient_a[] = { 0, 0, 200, 255 };
  static GLint light_diffuse_a[] = { 0, 0, 200, 255 };
  static GLint light_specular_a[] = { 0, 0, 200, 255 };
  glLightiv (GL_LIGHT1, GL_AMBIENT, light_ambient_a);
  glLightiv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_a);
  glLightiv (GL_LIGHT1, GL_SPECULAR, light_specular_a);

  glRotatef (cb_data_p->tip * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);
  glRotatef (static_cast<float> (M_PI_2) * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);

  cb_data_p->angle += cb_data_p->angleVelocity;
  glRotatef (cb_data_p->angle * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);

  for (std::vector<shape>::iterator iterator = cb_data_p->shapes.begin ();
       iterator != cb_data_p->shapes.end ();
       ++iterator)
  {
    (*iterator).show (cb_data_p->style,
                      cb_data_p->wireframe);
    (*iterator).animate ();
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_273_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_273_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_273_idle);
  else
    glutIdleFunc (NULL);
}
