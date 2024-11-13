#include "stdafx.h"

#include "glut_413.h"

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

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_19.h"

void
engine_glut_413_reshape (int width_in, int height_in)
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
engine_glut_413_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_413_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
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
engine_glut_413_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
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
engine_glut_413_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
      } // end IF

      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_413_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_413_timer (int v)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_413_timer,
                 v);
}

void
engine_glut_413_draw (void)
{
  struct Engine_OpenGL_GLUT_413_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_413_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

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
  glProgramUniform2f (cb_data_p->shader.id_, cb_data_p->resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_413_DEFAULT_WIDTH / 1000.0f),
                      static_cast<GLfloat> (ENGINE_GLUT_413_DEFAULT_HEIGHT / 1000.0f));

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
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);
  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  static int frame_count_i = 1;
  //float rr =
  //  (0.8f + std::sin (frame_count_i / 100.0f) * 0.3f) * engine_glut_413_DEFAULT_WIDTH / 4.0f + engine_glut_413_DEFAULT_WIDTH / 5.0f;
  //float mouseX = engine_glut_413_DEFAULT_WIDTH / 2.0f + std::cos (frame_count_i / 400.0f) * rr;
  //float mouseY = engine_glut_413_DEFAULT_WIDTH / 2.0f + std::sin (frame_count_i / 200.0f) * rr;
  glProgramUniform2f (cb_data_p->shader.id_, cb_data_p->mouseLoc,
                      static_cast<GLfloat> (cb_data_p->mouseX / static_cast<float> (ENGINE_GLUT_413_DEFAULT_WIDTH)),
                      static_cast<GLfloat> (cb_data_p->mouseY / static_cast<float> (ENGINE_GLUT_413_DEFAULT_HEIGHT)));
                      //static_cast<GLfloat> (mouseX / static_cast<float> (engine_glut_413_DEFAULT_WIDTH)),
                      //static_cast<GLfloat> (mouseY / static_cast<float> (engine_glut_413_DEFAULT_HEIGHT)));

  std::vector<float> whirl_data_a;
  for (int i = 0; i < ENGINE_GLUT_413_DEFAULT_NUMBER_OF_WHIRLS; i++)
  {
    whirl_data_a.push_back (cb_data_p->whirls[i].p.x);
    whirl_data_a.push_back (cb_data_p->whirls[i].p.y);
    whirl_data_a.push_back (cb_data_p->whirls[i].distortForce);
    whirl_data_a.push_back (cb_data_p->whirls[i].r);
  } // end FOR
  glProgramUniform4fv (cb_data_p->shader.id_, cb_data_p->whirlsLoc,
                       ENGINE_GLUT_413_DEFAULT_NUMBER_OF_WHIRLS, whirl_data_a.data ());

  for (std::vector<struct whirl>::iterator iterator = cb_data_p->whirls.begin ();
       iterator != cb_data_p->whirls.end ();
       ++iterator)
  {
    (*iterator).p.x =
      Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (frame_count_i / 500.0f, 1000.0f, static_cast<float> ((*iterator).id))), -1.0f, 1.0f, 0.0f, 1.0f);
    (*iterator).p.y =
      Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (frame_count_i / 500.0f, static_cast<float> ((*iterator).id), 1000.0f)), -1.0f, 1.0f, 0.0f, 1.0f);
  } // end FOR

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_413_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_413_DEFAULT_HEIGHT / 2.0f),
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

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_413_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_413_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_413_idle);
  else
    glutIdleFunc (NULL);
}
