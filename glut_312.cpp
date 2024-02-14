#include "stdafx.h"

#include "glut_312.h"

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
engine_glut_312_reshape (int width_in, int height_in)
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
engine_glut_312_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
    case 32: /* Space */
      cb_data_p->balls.clear ();
      break;
  } // end SWITCH
}

void
engine_glut_312_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
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
engine_glut_312_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
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
engine_glut_312_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
        struct ball_2 ball_s;
        ball_s.position.x = static_cast<float> (x);
        ball_s.position.y = static_cast<float> (y);
        float a = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
        ball_s.velocity.x = std::cos (a);
        ball_s.velocity.y = std::sin (a);
        ball_s.radius = static_cast<float> (ENGINE_GLUT_311_DEFAULT_RADIUS);
        cb_data_p->balls.push_back (ball_s);
      } // end IF

      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_312_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_312_timer (int v)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_312_timer,
                 v);
}

void
engine_glut_312_draw (void)
{
  struct Engine_OpenGL_GLUT_312_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_312_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  // rotate the camera
  //glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
  //                                         glm::radians (cb_data_p->camera.rotation.z),
  //                                         glm::vec3 (0.0f, 0.0f, 1.0f));
  //glm::vec3 rotation_center (0.0f, 0.0f, 0.0f);
  //glm::vec4 pos_rot_h =
  //  rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center,
  //                               1.0f);
  //cb_data_p->camera.position = glm::vec3 (pos_rot_h) + rotation_center;

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

  for (std::vector<struct ball_2>::iterator iterator = cb_data_p->balls.begin ();
       iterator != cb_data_p->balls.end ();
       ++iterator)
  {
    float f = ENGINE_GLUT_312_DEFAULT_NUMBER_OF_BALLS / static_cast<float> (ENGINE_GLUT_312_DEFAULT_RADIUS);
    float a = std::atan2 (static_cast<float> (cb_data_p->mouseY) - (*iterator).position.y, static_cast<float> (cb_data_p->mouseX) - (*iterator).position.x);
    (*iterator).velocity.x =
      std::min (std::max ((*iterator).velocity.x + std::cos (a) * (f / (*iterator).radius), -static_cast<float> (ENGINE_GLUT_312_DEFAULT_MAX_SPEED)), static_cast<float> (ENGINE_GLUT_312_DEFAULT_MAX_SPEED));
    (*iterator).velocity.y =
      std::min (std::max ((*iterator).velocity.y + std::sin (a) * (f / (*iterator).radius), -static_cast<float> (ENGINE_GLUT_312_DEFAULT_MAX_SPEED)), static_cast<float> (ENGINE_GLUT_312_DEFAULT_MAX_SPEED));
  } // end IF

  for (std::vector<struct ball_2>::iterator iterator = cb_data_p->balls.begin ();
       iterator != cb_data_p->balls.end ();
       ++iterator)
  {
    (*iterator).position.x += (*iterator).velocity.x;
    (*iterator).position.y += (*iterator).velocity.y;

    if ((*iterator).position.x < (*iterator).radius) { (*iterator).position.x = (*iterator).radius; (*iterator).velocity.x *= -1.0f; }
    if ((*iterator).position.x > static_cast<float> (ENGINE_GLUT_312_DEFAULT_WIDTH) - (*iterator).radius) { (*iterator).position.x = static_cast<float> (ENGINE_GLUT_312_DEFAULT_WIDTH) - (*iterator).radius; (*iterator).velocity.x *= -1.0f; }
    if ((*iterator).position.y < (*iterator).radius) { (*iterator).position.y = (*iterator).radius; (*iterator).velocity.y *= -1.0f; }
    if ((*iterator).position.y > static_cast<float> (ENGINE_GLUT_312_DEFAULT_HEIGHT) - (*iterator).radius) { (*iterator).position.y = static_cast<float> (ENGINE_GLUT_312_DEFAULT_HEIGHT) - (*iterator).radius; (*iterator).velocity.y *= -1.0f; }
  } // end FOR

  // update uniforms
  GLfloat data_a[ENGINE_GLUT_312_DEFAULT_NUMBER_OF_BALLS * 3];
  ACE_OS::memset (data_a, 0, sizeof (float) * ENGINE_GLUT_312_DEFAULT_NUMBER_OF_BALLS * 3);
  int i = 0;
  for (std::vector<struct ball_2>::iterator iterator = cb_data_p->balls.begin ();
       iterator != cb_data_p->balls.end ();
       ++iterator, ++i)
  {
    data_a[i * 3 + 0] = (*iterator).position.x;
    data_a[i * 3 + 1] = (*iterator).position.y;
    data_a[i * 3 + 2] = (*iterator).radius;
  } // end FOR
  glProgramUniform3fv (cb_data_p->programId, cb_data_p->ballsLoc,
                       ENGINE_GLUT_312_DEFAULT_NUMBER_OF_BALLS,
                       data_a);

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_312_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_312_DEFAULT_HEIGHT / 2.0f),
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
}

void
engine_glut_312_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_312_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_312_idle);
  else
    glutIdleFunc (NULL);
}
