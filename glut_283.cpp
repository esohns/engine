#include "stdafx.h"

#include "glut_283.h"

#include <cmath>

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
engine_glut_283_reshape (int width_in, int height_in)
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
engine_glut_283_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_283_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_F1:
      break;
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
    {
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 200.0f;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    }
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_283_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
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
engine_glut_283_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
        //cb_data_p->angle += cb_data_p->deltaAngleX;
        cb_data_p->xOrigin = -1;
      } // end IF
      else
      {
        cb_data_p->xOrigin = x;
      } // end ELSE
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
engine_glut_283_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
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
engine_glut_283_timer (int v)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_283_timer, v);
}

void
engine_glut_283_draw (void)
{
  struct Engine_OpenGL_GLUT_283_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_283_CBData*> (glutGetWindowData ());
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

  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

  static float r = 0x42 / 255.0f, g = 0x87 / 255.0f, b = 0xF5 / 255.0f;
  static float r_2 = 0xF5 / 255.0f, g_2 = 0x42 / 255.0f, b_2 = 0xAA / 255.0f;

  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 =
    std::chrono::high_resolution_clock::now();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 =
    std::chrono::high_resolution_clock::now();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - cb_data_p->tp1;
  float ms = elapsed_time.count () * 1000.0f;

  float x = std::cos (0 * 1.234f + std::sin (ms * 0.001f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_WIDTH / 2;
  float y = std::sin (0 * 1.234f + std::sin (ms * 0.0012f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_HEIGHT / 2;
  float z = std::sin (0 * 2.692f + std::sin (ms * 0.0015f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_HEIGHT / 2;
  float x_2 = std::cos (1 * 1.234f + std::sin (ms * 0.001f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_WIDTH / 2;
  float y_2 = std::sin (1 * 1.234f + std::sin (ms * 0.0012f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_HEIGHT / 2;
  float z_2 = std::sin (1 * 2.692f + std::sin (ms * 0.0015f) * 0.3f) * ENGINE_GLUT_283_DEFAULT_HEIGHT / 2;

  // pointLight(c, x, y, z);
  GLfloat light_position_a[] = {x, y, z};
  GLfloat light_position_2[] = {x_2, y_2, z_2};
  glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  static GLfloat light_ambient_a[] = { r, g, b, 1.0f };
  static GLfloat light_diffuse_a[] = { r, g, b, 1.0f };
  static GLint light_specular_a[] = { 250, 250, 250, 255 };
  glLightfv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  glLightfv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  glLightiv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  static GLfloat light_ambient_2[] = { r_2, g_2, b_2, 1.0f };
  static GLfloat light_diffuse_2[] = { r_2, g_2, b_2, 1.0f };
  static GLint light_specular_2[] = { 250, 250, 250, 255 };
  glLightfv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  glLightfv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  glLightiv (GL_LIGHT1, GL_SPECULAR, light_specular_2);

  static GLfloat material_color_a[] = {1.0f, 1.0f, 0.0f, 1.0f};
  static GLfloat material_specular_a[] = {0.0f, 0.0f, 1.0f, 1.0f};
  static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  static GLfloat material_shininess_f = 50.0f;
  glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  glTranslatef (0.0f, 0.0f, 400.0f);
  float loopTime = 18000.0f;
  float progress =
    Common_GL_Tools::map (std::fmod (ms, loopTime) / loopTime, 0.0f, 0.7f, 0.0f, 1.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (ms * 0.0001f), 0.0f, 0.0f, 1.0f);
  glTranslatef (100.0f * Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (ms * 0.00025, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f),
                50.0f  * Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (ms * 0.00025 + 1000.0, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f),
                0.0f);
  static int numRows = 50;
  for (int row = 0; row < numRows; row++)
  {
    glTranslatef (0.0f, 0.0f, row % 2 == 0 ? -60.0f : -40.0f);
    static int numAtoms = 12;
    float r =
      200.0f * std::sqrt (Common_GL_Tools::map (static_cast<float> (row), 0.0f, static_cast<float> (numRows), 1.0f, 0.0f));
    float nextR =
      200.0f * std::sqrt (Common_GL_Tools::map (static_cast<float> (row + 1), 0.0f, static_cast<float> (numRows), 1.0f, 0.0f));
    //float base = r * std::cos (static_cast<float> (M_PI) / static_cast<float> (numAtoms));
    //float sideLength =
    //  2.0f * r * std::sin (static_cast<float> (M_PI) / static_cast<float> (numAtoms));
    float nextSideLength =
      2.0f * nextR * std::sin (static_cast<float> (M_PI) / static_cast<float> (numAtoms));
    float internalAngle = std::atan2 (40.0f, nextSideLength / 2.0f);
    float perRowAngle = std::atan2 (60.0f, nextR - r);
    float connectorLength = std::hypot (60.0f, nextR - r, nextSideLength / 2.0f);
    if (row % 2 == 1)
      glRotatef ((180.0f / static_cast<float> (M_PI)) * ((0.5f / static_cast<float> (numAtoms)) * 2.0f * static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);
    for (int i = 0; i < numAtoms; i++)
    {
      float t = (row + i / static_cast<float> (numAtoms)) / static_cast<float> (numRows);
      float entrance =
        easeInOutCubic (Common_GL_Tools::map (progress, t / 12.0f, (3.0f + t) / 12.0f, 0.0f, 1.0f));
      float exit =
        easeInOutCubic (Common_GL_Tools::map (progress, (8.0f + t) / 12.0f, (11.0f + t) / 12.0f, 1.0f, 0.0f));
      float atomProgress = entrance * exit;
      float sphereOff =
        (200.0f + 200.0f * Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (t * 2000.0, (ms / loopTime) * 5.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) *
                           Common_GL_Tools::map (static_cast<float> (std::pow (row / static_cast<float> (numRows), 3)), 0.0f, 1.0f, 1.0f, 0.75f));
      float angleOff =
        0.1f * static_cast<float> (M_PI) *
               static_cast<float> (cb_data_p->noise.GetValue (t * 2000.0, 10.0 + (ms / loopTime) * 5.0, 0.0));

      glPushMatrix ();
      glRotatef ((180.0f / static_cast<float> (M_PI)) * ((i / static_cast<float> (numAtoms)) * (2.0f * static_cast<float> (M_PI)) + (1.0f - atomProgress) * angleOff), 0.0f, 0.0f, 1.0f);
      glTranslatef (Common_GL_Tools::map (atomProgress, 0.0f, 1.0f, sphereOff, r), 0.0f, 0.0f);
      cb_data_p->wireframe ? glutWireSphere (8.0f, 12, 12)
                           : glutSolidSphere (8.0f, 12, 12);
      glPopMatrix ();

      glPushMatrix ();
      glRotatef ((180.0f / static_cast<float> (M_PI)) * ((i / static_cast<float> (numAtoms)) * (2.0f * static_cast<float> (M_PI)) + (1.0f - atomProgress) * angleOff), 0.0f, 0.0f, 1.0f);
      glTranslatef (Common_GL_Tools::map (atomProgress, 0.0f, 1.0f, sphereOff, r), 0.0f, 0.0f);
      glRotatef ((180.0f / static_cast<float> (M_PI)) * static_cast<float> (M_PI_2), 1.0f, 0.0f, 0.0f);
      float barScale = easeInOutCubic (Common_GL_Tools::map (atomProgress, 0.5f, 1.0f, 0.0f, 1.0f));
      if (row % 2 == 1)
      {
        glTranslatef (0.0f, -60.0f / 2.0f, 0.0f);
        glScalef (barScale, barScale, barScale);
        glTranslatef (0.0f, 0.0f, -60.0f / 2.0f);
        cb_data_p->wireframe ? glutWireCylinder (2.0, 60.0, 6, 1)
                             : glutSolidCylinder (2.0, 60.0, 6, 1);
      } // end IF
      else
      {
        glPushMatrix ();
        glRotatef ((180.0f / static_cast<float> (M_PI)) * -internalAngle, 1.0f, 0.0f, 0.0f);
        glRotatef ((180.0f / static_cast<float> (M_PI)) * perRowAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef (0.0f, -connectorLength / 2.0f, 0.0f);
        glScalef (barScale, barScale, barScale);
        glTranslatef (0.0f, 0.0f, -connectorLength / 2.0f);
        cb_data_p->wireframe ? glutWireCylinder (2.0, connectorLength, 6, 1)
                             : glutSolidCylinder (2.0, connectorLength, 6, 1);
        glPopMatrix ();

        glPushMatrix ();
        glRotatef ((180.0f / static_cast<float> (M_PI)) * internalAngle, 1.0f, 0.0f, 0.0f);
        glRotatef ((180.0f / static_cast<float> (M_PI)) * perRowAngle, 0.0f, 0.0f, 1.0f);
        glTranslatef (0.0f, -connectorLength / 2.0f, 0.0f);
        glScalef (barScale, barScale, barScale);
        glTranslatef (0.0f, 0.0f, -connectorLength / 2.0f);
        cb_data_p->wireframe ? glutWireCylinder (2.0, connectorLength, 6, 1)
                             : glutSolidCylinder (2.0, connectorLength, 6, 1);
        glPopMatrix ();
      } // end ELSE
      glPopMatrix ();
    } // end FOR
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_283_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_283_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_283_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

float easeInOutCubic (float t)
{
  return t < 0.5f ? 4.0f * t * t * t
                  : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}
