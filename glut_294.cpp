#include "stdafx.h"

#include "glut_294.h"

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
engine_glut_294_reshape (int width_in, int height_in)
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
engine_glut_294_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case ' ':
    {
      cb_data_p->amped = !cb_data_p->amped;
      if (cb_data_p->amptarget == 5.0f)
        cb_data_p->amptarget = 1.0f;
      else
        cb_data_p->amptarget = 5.0f;
      break;
    }
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_294_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = -1500.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_294_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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
engine_glut_294_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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
engine_glut_294_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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
engine_glut_294_timer (int v)
{
  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_294_timer, v);
}

void
engine_glut_294_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_294_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_294_CBData*> (glutGetWindowData ());
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

  //ambientLight(150);
  //pointLight(150, 150, 150, width * sin(frameCount / 100), -height, width * cos(frameCount / 100));
  static GLfloat light_position_2[] = {ENGINE_GLUT_294_DEFAULT_WIDTH * std::sin (frame_count_i / 100.0f), static_cast<float> (-ENGINE_GLUT_294_DEFAULT_HEIGHT), ENGINE_GLUT_294_DEFAULT_WIDTH * std::cos (frame_count_i / 100.0f)};
  //glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //static GLfloat light_ambient_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_specular_a[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  //glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  //glLightfv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  static GLfloat light_ambient_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_diffuse_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  static GLfloat light_specular_2[] = {1.0f, 1.0f, 1.0f, 1.0f};
  glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  glLightfv (GL_LIGHT1, GL_SPECULAR, light_specular_2);
  //static GLfloat light_ambient_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_diffuse_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //static GLfloat light_specular_3[] = {1.0f, 1.0f, 1.0f, 1.0f};
  //glLightfv (GL_LIGHT2, GL_AMBIENT, light_ambient_3);
  //glLightfv (GL_LIGHT2, GL_DIFFUSE, light_diffuse_3);
  //glLightfv (GL_LIGHT2, GL_SPECULAR, light_specular_3);
  //static GLfloat light_direction_a[] = { 2.0f, 0.0f, 0.0f};
  //static GLfloat light_direction_2[] = {-2.0f, 0.0f, 0.0f};
  //static GLfloat light_direction_3[] = { 1.0f, 0.0f, 0.0f};
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

  cb_data_p->freq =
    Common_Math_Tools::lerp (cb_data_p->freq, ENGINE_GLUT_294_DEFAULT_FREQ - cb_data_p->f, 0.1f);
  glRotatef (frame_count_i / 300.0f, 1.0f, 0.0f, 0.0f);
  int stop = std::min (frame_count_i / 2, ENGINE_GLUT_294_DEFAULT_NUM);
  cb_data_p->amp = Common_Math_Tools::lerp (cb_data_p->amp, cb_data_p->amptarget, 0.1f);
  for (int i = 2; i < stop; i++)
  {
    glPushMatrix ();
    if (cb_data_p->amped)
      glTranslatef (0.0f, 0.0f, cb_data_p->amp * i * cb_data_p->dim * std::sin ((i * cb_data_p->freq) + frame_count_i / 100.0f));
    else
      glTranslatef (0.0f, 0.0f, cb_data_p->amp * cb_data_p->dim * cb_data_p->dim * std::sin ((i * cb_data_p->freq) + frame_count_i / 100.0f));
    float h = Common_GL_Tools::map (static_cast<float> (i), 2.0f, static_cast<float> (ENGINE_GLUT_294_DEFAULT_NUM), 0.0f, 360.0f);
    float r, g, b;
    Common_Image_Tools::HSVToRGB (h, 1.0f, 1.0f, r, g, b);
    glColor3f (r, g, b);
    //specularMaterial(h, 360, 360);
    cb_data_p->wireframe ? glutWireTorus (cb_data_p->dim, i * cb_data_p->dim, 64, 36)
                         : glutSolidTorus (cb_data_p->dim, i * cb_data_p->dim, 64, 36);
    glPopMatrix ();
  } // end FOR

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_294_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_294_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_294_idle : NULL);
}
