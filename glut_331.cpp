#include "stdafx.h"

#include "glut_331.h"

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

#include "common_image_tools.h"

#include "defines_15.h"
#include "engine_common.h"

void
engine_glut_331_reshape (int width_in, int height_in)
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
engine_glut_331_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_331_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 1200.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_331_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
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
engine_glut_331_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
      } // end IF
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
engine_glut_331_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_331_timer (int v)
{
  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_331_timer, v);
}

void
engine_glut_331_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_331_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_331_CBData*> (glutGetWindowData ());
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

  float locX = static_cast<float> (cb_data_p->mouseX) - (ENGINE_GLUT_331_DEFAULT_WIDTH / 2.0f);
  float locY = static_cast<float> (cb_data_p->mouseY) - (ENGINE_GLUT_331_DEFAULT_HEIGHT / 2.0f);
  //pointLight(250, 250, 250, locX - 25, locY, 50);
  static GLfloat light_position_a[] = {locX - 25.0f, locY, 50.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_a[] = {0.8f, 0.8f, 0.8f, 1.0f};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);

  static float r = 0xFC / 255.0f, g = 0x11 / 255.0f, b = 0x7D / 255.0f;
  glColor3f (r, g, b);
  //static GLfloat material_color_a[] = {r, g, b, 1.0f};
  //static GLfloat material_specular_a[] = {0.8f, 0.8f, 0.8f, 1.0f};
  //static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_shininess_f = 20.0f;
  //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  glTranslatef (-ENGINE_GLUT_331_DEFAULT_WIDTH / 2.0f, -ENGINE_GLUT_331_DEFAULT_HEIGHT / 2.0f, 0.0f);
  for (int x = 0; x < ENGINE_GLUT_331_DEFAULT_WIDTH; x += ENGINE_GLUT_331_DEFAULT_SIZE)
    for (int y = 0; y < ENGINE_GLUT_331_DEFAULT_HEIGHT; y += ENGINE_GLUT_331_DEFAULT_SIZE)
    {
      float n =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (x * ENGINE_GLUT_331_DEFAULT_COMPLEXITY, y * ENGINE_GLUT_331_DEFAULT_COMPLEXITY, frame_count_i * ENGINE_GLUT_331_DEFAULT_SPEED)), -1.0f, 1.0f, 0.0f, 1.0f);

      glPushMatrix ();

      glTranslatef (static_cast<float> (x), static_cast<float> (y), 0.0f);

      glRotatef ((180.0f / static_cast<float> (M_PI)) * n * 20.0f, 0.0f, 0.0f, 1.0f);
      glRotatef ((180.0f / static_cast<float> (M_PI)) * n * 10.0f, 1.0f, 0.0f, 0.0f);
      glRotatef ((180.0f / static_cast<float> (M_PI)) * n * 15.0f, 0.0f, 1.0f, 0.0f);

      glScalef (ENGINE_GLUT_331_DEFAULT_SIZE / 4.0f, ENGINE_GLUT_331_DEFAULT_SIZE / 4.0f, ENGINE_GLUT_331_DEFAULT_SIZE / 4.0f);

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

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_331_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_331_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_331_idle : NULL);
}
