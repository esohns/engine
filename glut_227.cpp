﻿#include "stdafx.h"

#include "glut_227.h"

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
engine_glut_227_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);
  //glOrtho (0.0, static_cast<GLdouble> (width_in), static_cast<GLdouble> (height_in), 0.0, 0.0, 1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_227_key (unsigned char key_in,
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
engine_glut_227_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 800.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_227_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
engine_glut_227_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
engine_glut_227_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
engine_glut_227_timer (int v)
{
  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_227_timer, v);
}

void
engine_glut_227_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_227_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_227_CBData*> (glutGetWindowData ());
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
  // axes are 10 units long
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (10, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 10, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 10);
  glEnd ();

  glRotatef (cb_data_p->angleX, 1.0f, 0.0f, 0.0f);
  glRotatef (cb_data_p->angleY, 0.0f, 1.0f, 0.0f);
  glRotatef (cb_data_p->angleZ, 0.0f, 0.0f, 1.0f);

  for (int i = -ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; i <= ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; i += 50)
    for (int j = -ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; j <= ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; j += 50)
      for (int k = -ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; k <= ENGINE_GLUT_227_DEFAULT_HALF_CUBE_SIZE; k += 50)
      {
        float r = Common_GL_Tools::map (std::sin (frame_count_i * 0.01f + i + j + k), -1.0f, 1.0f, 0.0f, 1.0f);
        float g = Common_GL_Tools::map (std::sin (frame_count_i * 0.02f + i + j + k), -1.0f, 1.0f, 0.0f, 1.0f);
        float b = Common_GL_Tools::map (std::sin (frame_count_i * 0.03f + i + j + k), -1.0f, 1.0f, 0.0f, 1.0f);

        glPushMatrix ();
        glTranslatef (static_cast<float> (i), static_cast<float> (j), static_cast<float> (k));
        glScalef (ENGINE_GLUT_227_DEFAULT_HALF_CUBE_EDGE, ENGINE_GLUT_227_DEFAULT_HALF_CUBE_EDGE, ENGINE_GLUT_227_DEFAULT_HALF_CUBE_EDGE);

        glColor3f (r, g, b);
        glBegin (GL_QUADS); // Begin drawing the color cube with 6 quads
        // Top face (y = 1.0f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glVertex3f (1.0f, 1.0f, -1.0f);
        glVertex3f (-1.0f, 1.0f, -1.0f);
        glVertex3f (-1.0f, 1.0f, 1.0f);
        glVertex3f (1.0f, 1.0f, 1.0f);

        // Bottom face (y = -1.0f)
        glVertex3f (1.0f, -1.0f, 1.0f);
        glVertex3f (-1.0f, -1.0f, 1.0f);
        glVertex3f (-1.0f, -1.0f, -1.0f);
        glVertex3f (1.0f, -1.0f, -1.0f);

        // Front face  (z = 1.0f)
        glVertex3f (1.0f, 1.0f, 1.0f);
        glVertex3f (-1.0f, 1.0f, 1.0f);
        glVertex3f (-1.0f, -1.0f, 1.0f);
        glVertex3f (1.0f, -1.0f, 1.0f);

        // Back face (z = -1.0f)
        glVertex3f (1.0f, -1.0f, -1.0f);
        glVertex3f (-1.0f, -1.0f, -1.0f);
        glVertex3f (-1.0f, 1.0f, -1.0f);
        glVertex3f (1.0f, 1.0f, -1.0f);

        // Left face (x = -1.0f)
        glVertex3f (-1.0f, 1.0f, 1.0f);
        glVertex3f (-1.0f, 1.0f, -1.0f);
        glVertex3f (-1.0f, -1.0f, -1.0f);
        glVertex3f (-1.0f, -1.0f, 1.0f);

        // Right face (x = 1.0f)
        glVertex3f (1.0f, 1.0f, -1.0f);
        glVertex3f (1.0f, 1.0f, 1.0f);
        glVertex3f (1.0f, -1.0f, 1.0f);
        glVertex3f (1.0f, -1.0f, -1.0f);
        glEnd();  // End of drawing color-cube

        glPopMatrix ();
      } // end FOR

  cb_data_p->angleX += 0.01f;
  cb_data_p->angleY += 0.02f;
  cb_data_p->angleZ += 0.03f;

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_227_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_227_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_227_idle);
  else
    glutIdleFunc (NULL);
}
