#include "stdafx.h"

#include "glut_281.h"

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

#include "common_math_tools.h"

#include "defines_13.h"
#include "engine_common.h"

void
engine_glut_281_reshape (int width_in, int height_in)
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
engine_glut_281_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_281_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_F1:
      break;
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
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 200.0f;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    }
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_281_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
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
engine_glut_281_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
        //cb_data_p->angle += cb_data_p->deltaAngleX;
        cb_data_p->xOrigin = -1;
      } // end IF
      else
      {
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
engine_glut_281_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
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
engine_glut_281_timer (int v)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_281_timer, v);
}

void
engine_glut_281_draw (void)
{
  struct Engine_OpenGL_GLUT_281_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_281_CBData*> (glutGetWindowData ());
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

  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

  //; [-W, -W / 3, W] .map(j = > spotLight(W, 0, W, 0, j * 2, j, 0, -j, -j / 2))
  //static GLfloat light_position_a[] = { 2.0f * static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W), static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W), 0.0f };
  //static GLfloat light_position_2[] = { 2.0f * static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W) / 3.0f, static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W) / 3.0f, 0.0f };
  //static GLfloat light_position_3[] = { 2.0f * static_cast<float> (ENGINE_GLUT_282_DEFAULT_W), static_cast<float> (ENGINE_GLUT_282_DEFAULT_W), 0.0f };
  //glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  //glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  //static GLfloat light_ambient_a[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  //static GLfloat light_diffuse_a[] = { 1.0f, 1.0f, 1.0f, 1.0f };
  //glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_a);
  //glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_a);
  //glLightfv (GL_LIGHT2, GL_AMBIENT, light_ambient_a);
  //glLightfv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_a);
  //static GLfloat light_direction_a[] = { 0.0f, static_cast<float> (ENGINE_GLUT_282_DEFAULT_W), static_cast<float> (ENGINE_GLUT_282_DEFAULT_W) / 2.0f };
  //static GLfloat light_direction_2[] = { 0.0f, static_cast<float> (ENGINE_GLUT_282_DEFAULT_W) / 3.0f, static_cast<float> (ENGINE_GLUT_282_DEFAULT_W) / 3.0f / 2.0f };
  //static GLfloat light_direction_3[] = { 0.0f, static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W), static_cast<float> (-ENGINE_GLUT_282_DEFAULT_W) / 2.0f };
  //glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction_a);
  //glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, light_direction_2);
  //glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_3);

  glRotatef ((180.0f / static_cast<float> (M_PI)) * (cb_data_p->f / 2.0f), 0.0f, 1.0f, 0.0f);
  float r, g, b;
  for (int i = 0; i < 53; i++)
  {
    glPushMatrix ();

    Common_Image_Tools::HSVToRGB (i * 20.0f, 0.8f, 1.0f, r, g, b);
    glColor4f (r, g, b, 0.1f);

    float Q = 40.0f * i + (cb_data_p->q % 408) / static_cast<float> (M_PI) / 8.0f;
    glRotatef ((180.0f / static_cast<float> (M_PI)) * std::cos (Q), 0.0f, 1.0f, 0.0f);
    glScalef (Q, Q, Q);

    glBegin (GL_QUADS);
    // Top face (y = 0.5f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glVertex3f ( 0.5f, 0.5f, -0.5f);
    glVertex3f (-0.5f, 0.5f, -0.5f);
    glVertex3f (-0.5f, 0.5f,  0.5f);
    glVertex3f ( 0.5f, 0.5f,  0.5f);

    // Bottom face (y = -0.5f)
    glVertex3f ( 0.5f, -0.5f,  0.5f);
    glVertex3f (-0.5f, -0.5f,  0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f ( 0.5f, -0.5f, -0.5f);

    // Front face  (z = 0.5f)
    glVertex3f ( 0.5f,  0.5f, 0.5f);
    glVertex3f (-0.5f,  0.5f, 0.5f);
    glVertex3f (-0.5f, -0.5f, 0.5f);
    glVertex3f ( 0.5f, -0.5f, 0.5f);

    // Back face (z = -0.5f)
    glVertex3f ( 0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f,  0.5f, -0.5f);
    glVertex3f ( 0.5f,  0.5f, -0.5f);

    // Left face (x = -0.5f)
    glVertex3f (-0.5f,  0.5f,  0.5f);
    glVertex3f (-0.5f,  0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f,  0.5f);

    // Right face (x = 0.5f)
    glVertex3f (0.5f,  0.5f, -0.5f);
    glVertex3f (0.5f,  0.5f,  0.5f);
    glVertex3f (0.5f, -0.5f,  0.5f);
    glVertex3f (0.5f, -0.5f, -0.5f);
    glEnd ();

    glPopMatrix ();
  } // end FOR

  ++cb_data_p->q;
  cb_data_p->f += 0.03f;

  glutSwapBuffers ();
}

void
engine_glut_281_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_281_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_281_idle);
  else
    glutIdleFunc (NULL);
}
