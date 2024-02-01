#include "stdafx.h"

#include "glut_229.h"

#include <iomanip>

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

#include "defines_10.h"
#include "engine_common.h"

void
engine_glut_229_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);
  //glOrtho (0.0, static_cast<GLdouble> (width_in), static_cast<GLdouble> (height_in), 0.0, 0.0, 1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_229_key (unsigned char key_in,
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
engine_glut_229_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 1500.0f;
      cb_data_p->camera.position.z = 300.0f;

      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_229_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
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
engine_glut_229_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_229_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);
}

void
engine_glut_229_timer (int v)
{
  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_229_timer, v);
}

void
engine_glut_229_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_229_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_229_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // fps
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point time_point =
    std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> time_point =
    std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float, std::milli> elapsed_time = time_point - cb_data_p->lastTimeStamp;
  float fps_f = 1000.0f / elapsed_time.count ();
  std::stringstream stream;
  stream << std::fixed << std::setprecision (2) << fps_f;
  std::string s = stream.str ();
  s = ACE_TEXT_ALWAYS_CHAR ("engine GLUT 229 - ") + s;
  s += ACE_TEXT_ALWAYS_CHAR (" fps");
  glutSetWindowTitle (s.c_str ());
  cb_data_p->lastTimeStamp = time_point;

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

  double noiseOffX = 111.0 + frame_count_i;
  double noiseOffY = 333.0 + frame_count_i * 2.0;

  float posY = (-ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_Y / 2.0f) * ENGINE_GLUT_229_DEFAULT_SIZE_BAR;
  for (int y = 0; y < ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_Y; y += 2)
  {
    glColor4ub (static_cast<GLubyte> (32), static_cast<GLubyte> (192), static_cast<GLubyte> (160), static_cast<GLubyte> (Common_GL_Tools::map (y, 0, ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_Y, 4, 255)));
    float posX = (-ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_X / 2.0f) * ENGINE_GLUT_229_DEFAULT_SIZE_BAR;
    for (int x = 0; x < ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_X; x++)
    {
      // get height (n) as noise
      float n =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue ((x + noiseOffX) * 0.15, (y - noiseOffY) * 0.215, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
        //Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.Evaluate ((x + noiseOffX) * 0.15, (y - noiseOffY) * 0.215)), -1.0f, 1.0f, 0.0f, 1.0f);
      // scale n * 256 and shrink on edges (with limit 0.1)
      float d =
        n * 256.0f * std::max (0.1f, easeInOutExpo (1.0f - std::abs (x - ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_X / 2.0f) / (ENGINE_GLUT_229_DEFAULT_NUMBER_OF_BARS_X / 2.0f)));

      glPushMatrix ();
      glTranslatef (posX, posY, d / 2.0f);
      glScalef (ENGINE_GLUT_229_DEFAULT_SIZE_BAR, ENGINE_GLUT_229_DEFAULT_SIZE_BAR, d);

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
      glEnd ();  // End of drawing color-cube

      glPopMatrix ();

      posX += ENGINE_GLUT_229_DEFAULT_SIZE_BAR;
    } // end FOR
    posY += 2.0f * ENGINE_GLUT_229_DEFAULT_SIZE_BAR;
  } // end FOR

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_229_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_229_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_229_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

float
easeInOutExpo (float x)
{
  return x == 0.0f ? 0.0f
                   : x == 1.0f ? 1.0f
                               : x < 0.5f ? std::pow (2.0f, 20.0f * x - 10.0f) / 2.0f
                                          : (2.0f - std::pow (2.0f, -20.0f * x + 10.0f)) / 2.0f;
}
