#include "stdafx.h"

#include "glut_308.h"

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

#include "common_math_tools.h"

#include "defines_14.h"
#include "engine_common.h"

void
engine_glut_308_reshape (int width_in, int height_in)
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
engine_glut_308_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_308_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 750.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_308_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
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
engine_glut_308_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
      //  cb_data_p->angle += cb_data_p->deltaAngle;
      //  cb_data_p->xOrigin = -1;
      } // end IF
      //else
      //  cb_data_p->xOrigin = x;
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
engine_glut_308_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // this will only be true when the left button is down
	//if (cb_data_p->xOrigin >= 0)
 // {
 //   cb_data_p->deltaAngle = (x - cb_data_p->xOrigin) * 0.001f;

 //   // update camera's direction
 //   //cb_data_p->camera.looking_at.x = sin (cb_data_p->angle + cb_data_p->deltaAngle);
 //   //cb_data_p->camera.looking_at.z = -cos (cb_data_p->angle + cb_data_p->deltaAngle);
 // } // end IF
  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_308_timer (int v)
{
  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_308_timer, v);
}

void
engine_glut_308_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_308_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_308_CBData*> (glutGetWindowData ());
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

  glColor3f (1.0f, 1.0f, 1.0f);

  glRotatef ((180.0f / static_cast<float> (M_PI)) * (-std::atan (std::sin (static_cast<float> (M_PI_4))) - cb_data_p->mouseY / 60.0f), 1.0f, 0.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI_4) + (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / 20.0f - cb_data_p->mouseX / 60.0f), 0.0f, 1.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (frame_count_i / 60.0f), 0.0f, 0.0f, 1.0f);
  float angle = -2.0f * static_cast<float> (M_PI) * frame_count_i / 60.0f / 60.0f;
  float radius = ENGINE_GLUT_308_DEFAULT_RADIUS;
  float deltaAngle;
  for (int i = 0; i < 1812; i++)
  {
    glPushMatrix ();
    deltaAngle = ENGINE_GLUT_308_DEFAULT_SPEED / radius;
    angle += deltaAngle;
    radius += 20.0f / (2.0f * static_cast<float> (M_PI)) * deltaAngle;
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI) / 64.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 12.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / 2.0f)), 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI) / 64.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 12.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / 2.1f)), 0.0f, 1.0f, 0.0f);
    glTranslatef (radius * std::cos (angle), radius * std::sin (angle), 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * angle, 0.0f, 0.0f, 1.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI) / 8.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 9.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / 3.0f)), 1.0f, 0.0f, 0.0f);

    float r, g, b;
    r =
      128.0f + 127.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 32.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / -1.9f);
    g =
      128.0f + 127.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 42.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f /  2.1f);
    b =
      128.0f + 127.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 22.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f / -2.0f);
    glColor3f (r / 255.0f, g / 255.0f, b / 255.0f);

    glPushMatrix ();

    glScalef (16.0f + 4.0f * std::cos ((2.0f * static_cast<float> (M_PI)) * i / 512.0f * 16.0f - (2.0f * static_cast<float> (M_PI)) * frame_count_i / 60.0f), 8.0f, 8.0f);

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

    glPopMatrix ();
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_308_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_308_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_308_idle : NULL);
}
