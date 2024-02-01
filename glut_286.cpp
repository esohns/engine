#include "stdafx.h"

#include "glut_286.h"

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

#include "defines_13.h"
#include "engine_common.h"

void
engine_glut_286_reshape (int width_in, int height_in)
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
engine_glut_286_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_286_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.x = 500.0f;
      cb_data_p->camera.position.y = -300.0f;
      cb_data_p->camera.position.z = -1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_286_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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
engine_glut_286_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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
engine_glut_286_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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
engine_glut_286_timer (int v)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_286_timer, v);
}

void
engine_glut_286_draw (void)
{
  struct Engine_OpenGL_GLUT_286_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_286_CBData*> (glutGetWindowData ());
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

  //; [-W, W, -W] .map(i = > pointLight([W], i / 2, i * 2, 99))
  static GLfloat light_position_a[] = {-ENGINE_GLUT_286_DEFAULT_W / 2.0f, -ENGINE_GLUT_286_DEFAULT_W * 2.0f, 99.0f};
  static GLfloat light_position_2[] = { ENGINE_GLUT_286_DEFAULT_W / 2.0f,  ENGINE_GLUT_286_DEFAULT_W * 2.0f, 99.0f};
  static GLfloat light_position_3[] = {-ENGINE_GLUT_286_DEFAULT_W / 2.0f, -ENGINE_GLUT_286_DEFAULT_W * 2.0f, 99.0f };
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  static GLfloat light_ambient_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular_2);
  static GLfloat light_ambient_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv (GL_LIGHT2, GL_AMBIENT, light_ambient_3);
  glLightfv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_3);
  glLightfv (GL_LIGHT2, GL_SPECULAR, light_specular_3);

  static GLfloat material_color_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  static GLfloat material_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static GLfloat material_shininess_f = 20.0f;
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  cb_data_p->f++;

  //glColor3f (1.0f, 1.0f, 1.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * std::cos (cb_data_p->f / static_cast<float> (ENGINE_GLUT_286_DEFAULT_W)), 0.0f, 1.0f, 0.0f);

  for (int y = -1000; y < 200; y += 20)
    for (int x = -ENGINE_GLUT_286_DEFAULT_W; x < ENGINE_GLUT_286_DEFAULT_W; x += 20)
    {
      glPushMatrix ();

      float z = static_cast<float> (y + cb_data_p->i % 20);
      glTranslatef (static_cast<float> (x), z, z);

      float n_x =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue ((y + cb_data_p->i % 20) / 50.0, 1.0 * x, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 50.0f;
      float n_y =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue ((y + cb_data_p->i % 20) / 50.0, 2.0 * x, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 150.0f;
      float n_z =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue ((y + cb_data_p->i % 20) / 50.0, 3.0 * x, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 50.0f;
      glScalef (n_x, n_y, n_z);

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
  cb_data_p->i++;

  glutSwapBuffers ();
}

void
engine_glut_286_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_286_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_286_idle);
  else
    glutIdleFunc (NULL);
}
