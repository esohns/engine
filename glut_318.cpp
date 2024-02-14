#include "stdafx.h"

#include "glut_318.h"

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

#include "common_image_tools.h"

#include "defines_14.h"
#include "engine_common.h"

void
engine_glut_318_reshape (int width_in, int height_in)
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
engine_glut_318_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_318_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
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
engine_glut_318_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
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
engine_glut_318_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
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
engine_glut_318_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_318_timer (int v)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_318_timer, v);
}

void
engine_glut_318_draw (void)
{
  struct Engine_OpenGL_GLUT_318_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_318_CBData*> (glutGetWindowData ());
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

  //;[-2,2,-1].map(i=>spotLight(W, 0, W, 0, i*W, W,0,-1*i,-1))
  //static GLfloat light_position_a[] = {0.0f, -2.0f * static_cast<float> (ENGINE_GLUT_318_DEFAULT_W), static_cast<float> (ENGINE_GLUT_318_DEFAULT_W)};
  //static GLfloat light_position_2[] = {0.0f,  2.0f * static_cast<float> (ENGINE_GLUT_318_DEFAULT_W), static_cast<float> (ENGINE_GLUT_318_DEFAULT_W)};
  //static GLfloat light_position_3[] = {0.0f,        -static_cast<float> (ENGINE_GLUT_318_DEFAULT_W), static_cast<float> (ENGINE_GLUT_318_DEFAULT_W)};
  //glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  //glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  //static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  //glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  //glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  //static GLfloat light_ambient_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_specular_2[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  //glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  //glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular_2);
  //static GLfloat light_ambient_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_specular_3[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //glLightfv (GL_LIGHT2, GL_AMBIENT, light_ambient_3);
  //glLightfv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_3);
  //glLightfv (GL_LIGHT2, GL_SPECULAR, light_specular_3);
  //static GLfloat light_direction_a[] = {0.0f, -1.0f * -2.0f, -1.0f};
  //static GLfloat light_direction_2[] = {0.0f, -1.0f *  2.0f, -1.0f};
  //static GLfloat light_direction_3[] = {0.0f, -1.0f * -1.0f, -1.0f};
  //glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction_a);
  //glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, light_direction_2);
  //glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_3);

  //static GLfloat material_color_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //static GLfloat material_specular_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_shininess_f = 10.0f;
  //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  //cb_data_p->wireframe ? glutWireTorus (260.0, static_cast<double> (ENGINE_GLUT_318_DEFAULT_W), 64, 64)
  //                     : glutSolidTorus (260.0, static_cast<double> (ENGINE_GLUT_318_DEFAULT_W), 64, 64);

  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI); i += static_cast<float> (M_PI) / 872.0f) // change to 128 if slow
  {
    glPushMatrix ();

    glRotatef ((180.0f / static_cast<float> (M_PI)) * (i + cb_data_p->f / 42.0f), 1.0f, 0.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (std::sin (cb_data_p->f / 83.0f) * 3.5f), 0.0f, 1.0f, 0.0f);
    glRotatef ((180.0f / static_cast<float> (M_PI)) * (i - cb_data_p->f / 83.0f), 0.0f, 0.0f, 1.0f);

    float r, g, b;
    Common_Image_Tools::HSVToRGB ((i / (2.0f * static_cast<float> (M_PI))) * 360.0f, 200.0f / 255.0f, 1.0f, r, g, b);
    //GLfloat material_color_2[] = {r, g, b, 1.0f};
    //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_2);
    glColor3f (r, g, b);
    cb_data_p->wireframe ? glutWireTorus (5.0, 270.0, 24, 24)
                         : glutSolidTorus (5.0, 270.0, 24, 24);

    glPopMatrix ();
  } // end FOR

  //cb_data_p->wireframe ? glutWireSphere (17.0, 32, 32)
  //                     : glutSolidSphere (17.0, 32, 32);

  ++cb_data_p->f;

  glutSwapBuffers ();
}

void
engine_glut_318_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_318_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_318_idle : NULL);
}
