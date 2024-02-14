#include "stdafx.h"

#include "glut_310.h"

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
engine_glut_310_reshape (int width_in, int height_in)
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
engine_glut_310_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_310_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 800.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_310_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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
engine_glut_310_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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
engine_glut_310_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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
engine_glut_310_timer (int v)
{
  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_310_timer, v);
}

void
engine_glut_310_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_310_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_310_CBData*> (glutGetWindowData ());
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

  //    pointLight(A,0,A,0,-W,W);
  static GLfloat light_position_a[] = {0.0f, -static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH), static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH)};
  //static GLfloat light_position_2[] = {0.0f, -static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH), static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH)};
  //static GLfloat light_position_3[] = {0.0f, -static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH), static_cast<float> (ENGINE_GLUT_310_DEFAULT_WIDTH)};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  //glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //glLightfv (GL_LIGHT2, GL_POSITION, light_position_3);
  static float r, g, b;
  Common_Image_Tools::HSVToRGB (0.66f * 360.0f, 0.0f, 0.66f, r, g, b);
  static GLfloat light_ambient_a[] = {r, g, b, 1.0f};
  static GLfloat light_diffuse_a[] = {r, g, b, 1.0f};
  static GLfloat light_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
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
  //static GLfloat light_direction_a[] = {0.0f, -1.0f, 0.0f};
  //static GLfloat light_direction_2[] = {0.0f, -1.0f, 0.0f};
  //static GLfloat light_direction_3[] = {0.0f,  1.0f, 0.0f};
  //glLightfv (GL_LIGHT0, GL_SPOT_DIRECTION, light_direction_a);
  //glLightfv (GL_LIGHT1, GL_SPOT_DIRECTION, light_direction_2);
  //glLightfv (GL_LIGHT2, GL_SPOT_DIRECTION, light_direction_3);

  //static GLfloat material_color_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //static GLfloat material_specular_a[] = {0.5f, 0.5f, 0.5f, 1.0f};
  //static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_shininess_f = 10.0f;
  //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  glColor3f (1.0f, 1.0f, 1.0f);

  glTranslatef (0.0f, -100.0f, 0.0f);
  glRotatef (-1.0f * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  //float n =
  //  Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (cb_data_p->f * 9.0, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 9.0f - 2.0f;
  //box(n,n,1000);
  static float w = static_cast<float> (M_PI) / 8.0f;
  for (float i = 0.0f; i < 2.0f * static_cast<float> (M_PI); i += w)
  {
    glRotatef ((180.0f / static_cast<float> (M_PI)) * frame_count_i / 1000.0f, 0.0f, 0.0f, 1.0f);

    glPushMatrix ();
    float I = 50.0f * 4.0f;
    //float I = 50 * (i - std::fmod (cb_data_p->f, w));
    glTranslatef (0, 0, I);
    
    Common_Image_Tools::HSVToRGB (i / (2.0f * static_cast<float> (M_PI)) * 360.0f, 0.8f, 1.0f, r, g, b);
    GLfloat material_color_a[] = {r, g, b, 1.0f};
    glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);

    cb_data_p->wireframe ? glutWireTorus (0.5, I, 32, 4)
                         : glutSolidTorus (0.5, I, 32, 4);
    cb_data_p->wireframe ? glutWireTorus (10.0, I * 1.5, 32, 4)
                         : glutSolidTorus (10.0, I * 1.5, 32, 4);
    glPopMatrix ();
  } // end FOR

  //cb_data_p->f -= 0.01f;

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_310_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_310_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_310_idle : NULL);
}
