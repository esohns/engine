#include "stdafx.h"

#include "glut_297.h"

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

#include "defines_13.h"
#include "engine_common.h"

void
engine_glut_297_reshape (int width_in, int height_in)
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
engine_glut_297_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_297_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 500.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_297_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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
engine_glut_297_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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
engine_glut_297_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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
engine_glut_297_timer (int v)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_297_timer, v);
}

void
engine_glut_297_draw (void)
{
  struct Engine_OpenGL_GLUT_297_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_297_CBData*> (glutGetWindowData ());
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

  //[2, -2, 1] .map(i = > spotLight(255, 255, 255, 0, -400 * i, 400 * i, 0, i, -i));
  static GLfloat light_position_a[] = {0.0f, static_cast<float> (-ENGINE_GLUT_297_DEFAULT_W) *  2.0f, static_cast<float> (ENGINE_GLUT_297_DEFAULT_W) *  2.0f};
  static GLfloat light_position_2[] = {0.0f, static_cast<float> (-ENGINE_GLUT_297_DEFAULT_W) * -2.0f, static_cast<float> (ENGINE_GLUT_297_DEFAULT_W) * -2.0f};
  static GLfloat light_position_3[] = {0.0f, static_cast<float> (-ENGINE_GLUT_297_DEFAULT_W) *  1.0f, static_cast<float> (ENGINE_GLUT_297_DEFAULT_W) *  1.0f};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  static GLfloat light_ambient_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_2[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular_2);
  static GLfloat light_ambient_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_3[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightfv (GL_LIGHT2, GL_AMBIENT, light_ambient_3);
  glLightfv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_3);
  glLightfv (GL_LIGHT2, GL_SPECULAR, light_specular_3);
  static GLfloat light_direction_a[] = {0.0f,  2.0f, -2.0f};
  static GLfloat light_direction_2[] = {0.0f, -2.0f,  2.0f};
  static GLfloat light_direction_3[] = {0.0f,  1.0f, -1.0f};
  glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction_a);
  glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, light_direction_2);
  glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_3);

  //static GLfloat material_color_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //static GLfloat material_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_shininess_f = 10.0f;
  //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  //glPushMatrix ();
  //glScalef (1.0f, 0.5f, 1.0f);
  //glTranslatef (0.0f, -1700.0f, 0.0f);
  //static float gray_f = 100.0f / 255.0f;
  //glColor3f (gray_f, gray_f, gray_f);
  //cb_data_p->wireframe ? glutWireSphere (2000.0, 32, 32)
  //                     : glutSolidSphere (2000.0, 32, 32);
  //glPopMatrix ();

  glColor3f (1.0f, 1.0f, 1.0f);

  glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI_2), 0.0f, 0.0f, 1.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (cb_data_p->f / 150.0f), 0.0f, 1.0f, 0.0f);
  static int max = 400;
  for (int i = 0; i < max; i++)
  {
    float I = i * (static_cast<float> (M_PI) * 2.0f / static_cast<float> (max));
    glPushMatrix ();
    
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (I + cb_data_p->f / 50.0f), 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * I, 0.0f, 0.0f, 1.0f);
    glTranslatef (std::sin (I) * 80.0f, std::cos (I) * 120.0f, 0.0f);
    glTranslatef (0.0f, 0.0f, -50.0f / 2.0f);
    cb_data_p->wireframe ? glutWireCylinder (3.0, 50.0, 16, 16)
                         : glutSolidCylinder (3.0, 50.0, 16, 16);

    if (cb_data_p->f % max == i)
    {
      glTranslatef (30.0f, 0.0f, 0.0f);

      //pointLight([255], 0, 400, 0);
      //pointLight([255], 0, -400, 0);

      glColor3ub (0xD3, 0x00, 0x2C);
      cb_data_p->wireframe ? glutWireSphere (28.0, 16, 16)
                           : glutSolidSphere (28.0, 16, 16);
      glColor3f (1.0f, 1.0f, 1.0f);
    } // end IF
    
    glPopMatrix ();
  } // end FOR

  ++cb_data_p->f;

  glutSwapBuffers ();
}

void
engine_glut_297_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_297_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_297_idle : NULL);
}
