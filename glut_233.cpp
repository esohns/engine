#include "stdafx.h"

#include "glut_233.h"

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
engine_glut_233_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  //ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -100.0, 100.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), -100.0, 100.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_233_key (unsigned char key_in,
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
engine_glut_233_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
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
engine_glut_233_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
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
engine_glut_233_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_233_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_233_timer (int v)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_233_timer,
                 v);
}

void
engine_glut_233_draw (void)
{
  struct Engine_OpenGL_GLUT_233_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_233_CBData*> (glutGetWindowData ());
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
  //; [W, 0] .map(l = > pointLight([W], 0, l, W));
  //static GLfloat light_position_a[] = {0.0f, static_cast<float> (ENGINE_GLUT_233_DEFAULT_W), static_cast<float> (ENGINE_GLUT_233_DEFAULT_W)};
  //static GLfloat light_position_2[] = {0.0f, 0.0f, static_cast<float> (ENGINE_GLUT_233_DEFAULT_W)};
  //glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  //glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_a);
  //glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_a);

  glRotatef (cb_data_p->f * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);

  float i = 2.0f * static_cast<float> (M_PI);
  while (i > 0.0f)
  {
    glPushMatrix ();
    glRotatef (i * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
    i -= static_cast<float> (M_PI) / 512.0f; // PI/512 or PI/256
    glTranslatef (99.0f, 0.0f, 0.0f);
    glRotatef ((i + cb_data_p->f * 3.0f) * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f); // TRY THIS TOO: rotateY(f*3);
    if (cb_data_p->n % 64 == 0) // n++%64 or n++%32
    {
      //box(40, 9, 9);
      glPushMatrix ();

      glScalef (40.0f, 9.0f, 9.0f);

      glBegin (GL_QUADS);
      // Define vertices in counter-clockwise (CCW) order with normal pointing out
      glColor3f (0.0f, 1.0f, 0.0f);
      glVertex3f ( 1.0f, 1.0f, -1.0f);
      glVertex3f (-1.0f, 1.0f, -1.0f);
      glVertex3f (-1.0f, 1.0f,  1.0f);
      glVertex3f ( 1.0f, 1.0f,  1.0f);

      // Bottom face (y = -1.0f)
      glColor3f (1.0f, 0.5f, 0.0f);
      glVertex3f ( 1.0f, -1.0f,  1.0f);
      glVertex3f (-1.0f, -1.0f,  1.0f);
      glVertex3f (-1.0f, -1.0f, -1.0f);
      glVertex3f ( 1.0f, -1.0f, -1.0f);

      // Front face  (z = 1.0f)
      glColor3f (1.0f, 0.0f, 0.0f);
      glVertex3f ( 1.0f,  1.0f, 1.0f);
      glVertex3f (-1.0f,  1.0f, 1.0f);
      glVertex3f (-1.0f, -1.0f, 1.0f);
      glVertex3f ( 1.0f, -1.0f, 1.0f);

      // Back face (z = -1.0f)
      glColor3f (1.0f, 1.0f, 0.0f);
      glVertex3f ( 1.0f, -1.0f, -1.0f);
      glVertex3f (-1.0f, -1.0f, -1.0f);
      glVertex3f (-1.0f,  1.0f, -1.0f);
      glVertex3f ( 1.0f,  1.0f, -1.0f);

      // Left face (x = -1.0f)
      glColor3f (0.0f, 0.0f, 1.0f);
      glVertex3f (-1.0f,  1.0f,  1.0f);
      glVertex3f (-1.0f,  1.0f, -1.0f);
      glVertex3f (-1.0f, -1.0f, -1.0f);
      glVertex3f (-1.0f, -1.0f,  1.0f);

      // Right face (x = 1.0f)
      glColor3f (1.0f, 0.0f, 1.0f);
      glVertex3f (1.0f,  1.0f, -1.0f);
      glVertex3f (1.0f,  1.0f,  1.0f);
      glVertex3f (1.0f, -1.0f,  1.0f);
      glVertex3f (1.0f, -1.0f, -1.0f);
      glEnd ();

      glPopMatrix ();
    } // end IF
    cb_data_p->n++;

    glPushMatrix ();
    rail (30.0f);
    rail (-60.0f);
    glPopMatrix ();

    glPopMatrix ();
  } // end WHILE

  cb_data_p->f += 0.01f;

  glutSwapBuffers ();
}

void
engine_glut_233_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_233_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_233_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
rail (float r)
{
  glTranslatef (r, 0.0f, 0.0f);

  //box (20, 3, 20);
  glPushMatrix ();

  glScalef (20.0f, 3.0f, 20.0f);

  glBegin (GL_QUADS);
  // Define vertices in counter-clockwise (CCW) order with normal pointing out
  glColor3f (0.0f, 1.0f, 0.0f);
  glVertex3f (1.0f, 1.0f, -1.0f);
  glVertex3f (-1.0f, 1.0f, -1.0f);
  glVertex3f (-1.0f, 1.0f, 1.0f);
  glVertex3f (1.0f, 1.0f, 1.0f);

  // Bottom face (y = -1.0f)
  glColor3f (1.0f, 0.5f, 0.0f);
  glVertex3f (1.0f, -1.0f, 1.0f);
  glVertex3f (-1.0f, -1.0f, 1.0f);
  glVertex3f (-1.0f, -1.0f, -1.0f);
  glVertex3f (1.0f, -1.0f, -1.0f);

  // Front face  (z = 1.0f)
  glColor3f (1.0f, 0.0f, 0.0f);
  glVertex3f (1.0f, 1.0f, 1.0f);
  glVertex3f (-1.0f, 1.0f, 1.0f);
  glVertex3f (-1.0f, -1.0f, 1.0f);
  glVertex3f (1.0f, -1.0f, 1.0f);

  // Back face (z = -1.0f)
  glColor3f (1.0f, 1.0f, 0.0f);
  glVertex3f (1.0f, -1.0f, -1.0f);
  glVertex3f (-1.0f, -1.0f, -1.0f);
  glVertex3f (-1.0f, 1.0f, -1.0f);
  glVertex3f (1.0f, 1.0f, -1.0f);

  // Left face (x = -1.0f)
  glColor3f (0.0f, 0.0f, 1.0f);
  glVertex3f (-1.0f, 1.0f, 1.0f);
  glVertex3f (-1.0f, 1.0f, -1.0f);
  glVertex3f (-1.0f, -1.0f, -1.0f);
  glVertex3f (-1.0f, -1.0f, 1.0f);

  // Right face (x = 1.0f)
  glColor3f (1.0f, 0.0f, 1.0f);
  glVertex3f (1.0f, 1.0f, -1.0f);
  glVertex3f (1.0f, 1.0f, 1.0f);
  glVertex3f (1.0f, -1.0f, 1.0f);
  glVertex3f (1.0f, -1.0f, -1.0f);
  glEnd ();

  glPopMatrix ();
}
