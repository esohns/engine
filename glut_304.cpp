#include "stdafx.h"

#include "glut_304.h"

#include "GL/glew.h"
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

#include "defines_14.h"
#include "engine_common.h"

void
engine_glut_304_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  150.0, -150.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_304_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_304_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 500.0f;

      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_304_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
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
engine_glut_304_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (button == GLUT_LEFT_BUTTON)
  //  cb_data_p->leftButtonIsDown = (state == GLUT_DOWN);
}

void
engine_glut_304_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  float yMouse =
    (Common_GL_Tools::map (static_cast<float> (y), 0.0f, static_cast<float> (ENGINE_GLUT_304_DEFAULT_HEIGHT), static_cast<float> (ENGINE_GLUT_304_DEFAULT_HEIGHT), 0.0f) / static_cast<float> (ENGINE_GLUT_304_DEFAULT_HEIGHT)) * 2.0f - 1.0f;
  float xMouse = (x / static_cast<float> (ENGINE_GLUT_304_DEFAULT_WIDTH)) * 2.0f - 1.0f;

  // Make sure pixels are square
  xMouse = (xMouse * static_cast<float> (ENGINE_GLUT_304_DEFAULT_WIDTH)) / static_cast<float> (ENGINE_GLUT_304_DEFAULT_HEIGHT);
  yMouse = yMouse;

  glProgramUniform2f (cb_data_p->programId, cb_data_p->mouseLoc,
                      xMouse, yMouse);
}

void
engine_glut_304_timer (int v)
{
  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_304_timer,
                 v);
}

void
engine_glut_304_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_304_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_304_CBData*> (glutGetWindowData ());
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
  //gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
  //           cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
  //           cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  // update uniforms
  glProgramUniform2f (cb_data_p->programId, cb_data_p->resolutionLoc,
                      static_cast<float> (ENGINE_GLUT_304_DEFAULT_WIDTH),
                      static_cast<float> (ENGINE_GLUT_304_DEFAULT_HEIGHT));

  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 =
    std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 =
    std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - cb_data_p->tp1;
  glProgramUniform1f (cb_data_p->programId, cb_data_p->timeLoc,
                      elapsed_time.count ());

  float increment = 20.0f / (60.0f * 140.0f); // timestep based on framerate, will rotate same speed on all regardless of framerate
  cb_data_p->time += increment;
  if (cb_data_p->time > 2.0f * static_cast<float> (M_PI))
    cb_data_p->time -= 2.0f * static_cast<float> (M_PI); // prevent time from getting to big and maybe causing errors?

  float pow = Common_GL_Tools::map (std::sin (cb_data_p->time), -1.0f, 1.0f, 6.0f, 12.0f); // make pow change over time between 6 and 12
  glProgramUniform1f (cb_data_p->programId, cb_data_p->powerLoc,
                      pow);

  glProgramUniform1f (cb_data_p->programId, cb_data_p->angleLoc,
                      cb_data_p->time);

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_304_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_304_DEFAULT_HEIGHT / 2.0f),
                0.0f);

  glColor3f (1.0f, 1.0f, 1.0f);
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> (y * cb_data_p->scaleFactor), 0.0f);
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ((y + 1) * cb_data_p->scaleFactor), 0.0f);
    } // end FOR
    glEnd ();
  } // end FOR

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_304_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_304_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_304_idle);
  else
    glutIdleFunc (NULL);
}
