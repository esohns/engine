#include "stdafx.h"

#include "glut_315.h"

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
engine_glut_315_reshape (int width_in, int height_in)
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
engine_glut_315_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_315_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 2500.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_315_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
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
engine_glut_315_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
        cb_data_p->angle += cb_data_p->deltaAngle;
        cb_data_p->xOrigin = -1;
      } // end IF
      else
        cb_data_p->xOrigin = x;
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
engine_glut_315_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
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
engine_glut_315_timer (int v)
{
  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_315_timer, v);
}

void
engine_glut_315_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_315_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_315_CBData*> (glutGetWindowData ());
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

  static GLfloat light_position_a[] = {0.0f, 0.0f, 2500.0f};
  // *TODO*: light direction is not position ! (use GL_SPOT_DIRECTION)
  //directionalLight(235, 38, 113, -dirX, -dirY, -1)
  static float dirX = (125.0f / static_cast<float> (ENGINE_GLUT_315_DEFAULT_WIDTH) - 0.5f) * 2.0f;
  static float dirY = (500.0f / static_cast<float> (ENGINE_GLUT_315_DEFAULT_HEIGHT) - 0.5f) * 2.0f;
  static GLfloat light_position_2[] = {-dirX, -dirY, -1.0f};
  static float dirX2 = (500.0f / static_cast<float> (ENGINE_GLUT_315_DEFAULT_WIDTH) - 0.5f) * 2.0f;
  static float dirY2 = (125.0f / static_cast<float> (ENGINE_GLUT_315_DEFAULT_HEIGHT) - 0.5f) * 2.0f;
  //directionalLight(207, 87, 132, -dirX2, -dirY2, -1);
  static GLfloat light_position_3[] = {-dirX2, -dirY2, -1.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  static GLint light_ambient_a[] = {145, 145, 145, 255};
  static GLint light_diffuse_a[] = {145, 145, 145, 255};
  static GLfloat light_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightiv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightiv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  static GLint light_ambient_2[] = {235, 38, 113, 255};
  static GLint light_diffuse_2[] = {235, 38, 113, 255};
  static GLfloat light_specular_2[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightiv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  glLightiv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular_2);
  static GLint light_ambient_3[] = {207, 87, 132, 255};
  static GLint light_diffuse_3[] = { 207, 87, 132, 255 };
  static GLfloat light_specular_3[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightiv (GL_LIGHT2, GL_AMBIENT, light_ambient_3);
  glLightiv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_3);
  glLightfv (GL_LIGHT2, GL_SPECULAR, light_specular_3);

  //glColor3f (1.0f, 1.0f, 1.0f);

  cb_data_p->wireframe ? glutWireSphere (10.0, 12, 24)
                       : glutSolidSphere (10.0, 12, 24);

  glRotatef ((180.0f / static_cast<float> (M_PI)) * frame_count_i * 0.01f, 1.0f, 0.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * frame_count_i * 0.03f, 0.0f, 0.0f, 1.0f);
  for (float j = 0.0f; j < 1500.0f; j += 7.5f)
  {
    glRotatef ((180.0f / static_cast<float> (M_PI)) * frame_count_i * 0.001f, 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * frame_count_i * 0.003f, 0.0f, 0.0f, 1.0f);
    
    float r, g, b;
    Common_Image_Tools::HSVToRGB ((j / 6.0f) * 360.0f / 255.0f, 1.0f, 1.0f, r, g, b);
    //glColor3f (r, g, b);
    GLfloat material_color_a[] = {r, g, b, 1.0f};
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);

    cb_data_p->wireframe ? glutWireTorus (5.0, j, 12, 24)
                         : glutSolidTorus (5.0, j, 12, 24);
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_315_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_315_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_315_idle : NULL);
}
