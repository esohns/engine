#include "stdafx.h"

#include "glut_317.h"

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

#include "common_gl_defines.h"

#include "common_image_tools.h"

#include "defines_14.h"
#include "engine_common.h"

void
engine_glut_317_reshape (int width_in, int height_in)
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
engine_glut_317_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_317_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_317_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
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
engine_glut_317_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
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
engine_glut_317_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //cb_data_p->mouseX = x;
  //cb_data_p->mouseY = y;
}

void
engine_glut_317_timer (int v)
{
  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_317_timer, v);
}

void
engine_glut_317_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_317_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_317_CBData*> (glutGetWindowData ());
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

  for (int j = 0; j < ENGINE_GLUT_317_DEFAULT_M; j++)
  {
    float R = cb_data_p->r + j * 2.0f * ENGINE_GLUT_317_DEFAULT_SIZE;
    int n = static_cast<int> ((2.0f * static_cast<float> (M_PI) * R) / (ENGINE_GLUT_317_DEFAULT_SIZE * 2.0f));
    for (int i = 0; i < n; i++)
    {
      float t = (2.0f * static_cast<float> (M_PI) * i) / static_cast<float> (n);
      float x = R * std::cos (t);
      float y = R * std::sin (t);

      float z =
        -cb_data_p->zStart + 2.0f * (1.0f - i / static_cast<float> (n)) * cb_data_p->zStart;
      z +=
        -cb_data_p->zStart + (2.0f * cb_data_p->zStart * (frame_count_i % cb_data_p->loopTime)) / static_cast<float> (cb_data_p->loopTime);
      z =
        std::fmod ((z + cb_data_p->zStart), (2.0f * cb_data_p->zStart)) - cb_data_p->zStart;

      float anim =
        std::fmod ((z + cb_data_p->zStart), (2.0f * cb_data_p->zStart)) / (2.0f * cb_data_p->zStart); //-.5 to .5

      float dotR = static_cast<float> (cb_data_p->w);
      float r = ENGINE_GLUT_317_DEFAULT_SIZE + dotR * std::fmod (anim + 0.5f, 1.0f);
      float sphR = 2.0f * static_cast<float> (std::pow (1.0f - r / (ENGINE_GLUT_317_DEFAULT_SIZE + dotR), 4));
      float r_2, g, b;
      for (int k = 0; k < ENGINE_GLUT_317_DEFAULT_O; k++)
      {
        Common_Image_Tools::HSVToRGB (k / static_cast<float> (ENGINE_GLUT_317_DEFAULT_O) * 360.0f, 1.0f, 1.0f, r_2, g, b);
        glColor3f (r_2, g, b);
        float t_2 =
          (2.0f * static_cast<float> (M_PI) * k) / static_cast<float> (ENGINE_GLUT_317_DEFAULT_O);
        float dotX = x + r * std::cos (t_2);
        float dotY = y + r * std::sin (t_2);
        glTranslatef (dotX, dotY, 0.0f);
        cb_data_p->wireframe ? glutWireSphere (sphR, 12, 12)
                             : glutSolidSphere (sphR, 12, 12);
        glTranslatef (-dotX, -dotY, 0.0f);
      } // end FOR

      glColor3f (1.0f, 1.0f, 1.0f);
      float siz2 =
        ENGINE_GLUT_317_DEFAULT_SIZE * e (static_cast<float> (std::pow (std::fmod (anim + 0.5f, 1.0f), 2)));
      drawCircle (x, y, siz2, 24);

      Common_Image_Tools::HSVToRGB (i / static_cast<float> (n) * 360.0f, 1.0f, 1.0f, r_2, g, b);
      glColor4f (r_2, g, b, 240.0f / 255.0f);
      glTranslatef (x, y, z);
      cb_data_p->wireframe ? glutWireSphere (static_cast<double> (ENGINE_GLUT_317_DEFAULT_SIZE), 24, 24)
                           : glutSolidSphere (static_cast<double> (ENGINE_GLUT_317_DEFAULT_SIZE), 24, 24);
      glTranslatef (-x, -y, -z);

      z = z + 2.0f * cb_data_p->zStart;
      glTranslatef (x, y, z);
      cb_data_p->wireframe ? glutWireSphere (static_cast<double> (ENGINE_GLUT_317_DEFAULT_SIZE), 24, 24)
                           : glutSolidSphere (static_cast<double> (ENGINE_GLUT_317_DEFAULT_SIZE), 24, 24);
      glTranslatef (-x, -y, -z);
    } // end FOR
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_317_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_317_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_317_idle : NULL);
}

float
e (float x)
{
  static float n1 = 7.5625f;
  static float d1 = 2.75f;

  if (x < 1.0f / d1) return n1 * x * x;
  if (x < 2.0f / d1) return n1 * (x -= 1.5f / d1) * x + 0.75f;
  if (x < 2.5f / d1) return n1 * (x -= 2.25f / d1) * x + 0.9375f;
  return n1 * (x -= 2.625f / d1) * x + 0.984375f;
}

void
drawCircle (float cx, float cy, float r, int num_segments)
{
  glBegin (GL_LINE_LOOP);
  
  for (int ii = 0; ii < num_segments; ii++)
  {
    float theta = 2.0f * static_cast<float> (M_PI) * float(ii) / float (num_segments);

    float x = r * std::cos (theta);
    float y = r * std::sin (theta);

    glVertex2f (x + cx, y + cy);
  } // end FOR

  glEnd ();
}
