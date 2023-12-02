#include "stdafx.h"

#include "glut_132.h"

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

#include "defines_5.h"
#include "engine_common.h"

void
engine_glut_132_reshape (int width_in, int height_in)
{
  glViewport (0, 0,
              static_cast<GLsizei> (width_in), static_cast<GLsizei> (height_in));
  COMMON_GL_ASSERT;

  glMatrixMode (GL_PROJECTION);
  COMMON_GL_ASSERT;

  glLoadIdentity ();
  COMMON_GL_ASSERT;

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);
  COMMON_GL_ASSERT;

  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
}

void
engine_glut_132_key (unsigned char key_in,
                     int x,
                     int y)
{
  switch (key_in)
  {
    case 27:  /* Escape */
      //exit (0);
      glutLeaveMainLoop ();

      break;
  } // end SWITCH
}

void
engine_glut_132_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.x = ENGINE_GLUT_132_DEFAULT_WIDTH / 2.0f;
      cb_data_p->camera.position.y = ENGINE_GLUT_132_DEFAULT_HEIGHT;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_132_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
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
engine_glut_132_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
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
engine_glut_132_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
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
engine_glut_132_timer (int v)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    cb_data_p->angle += 1.0f;
    if (cb_data_p->angle > 360.0f)
    {
      cb_data_p->angle -= 360.0f;
    }
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_132_timer, v);
}

void
engine_glut_132_draw (void)
{
  struct Engine_OpenGL_GLUT_132_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_132_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
  glLoadIdentity ();
  COMMON_GL_ASSERT;

  // rotate the camera
  glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
                                           glm::radians (cb_data_p->camera.rotation.z),
                                           glm::vec3 (0.0f, 0.0f, 1.0f));
  glm::vec3 rotation_center (ENGINE_GLUT_132_DEFAULT_WIDTH / 2.0f,
                             ENGINE_GLUT_132_DEFAULT_HEIGHT / 2.0f,
                             cb_data_p->camera.position.z);
  glm::vec4 pos_rot_h =
    rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center,
                                 1.0f);
  glm::vec3 pos_rot = glm::vec3 (pos_rot_h) + rotation_center;
  cb_data_p->camera.position = pos_rot;

  // set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);
  COMMON_GL_ASSERT;

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are ten units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();
  COMMON_GL_ASSERT;

  static Common_GL_Color_t color_s = { 0, 0, 0 };
  glColor3ub (color_s.r, color_s.g, color_s.b);

  cb_data_p->a = std::sin (cb_data_p->f) * 20.0f + 30.0f;
  cb_data_p->f += 0.005f;

  for (int y = -ENGINE_GLUT_132_DEFAULT_M; y < ENGINE_GLUT_132_DEFAULT_WIDTH + ENGINE_GLUT_132_DEFAULT_M; y += ENGINE_GLUT_132_DEFAULT_STEP)
    for (int x = -ENGINE_GLUT_132_DEFAULT_M; x < ENGINE_GLUT_132_DEFAULT_WIDTH + ENGINE_GLUT_132_DEFAULT_M; x += ENGINE_GLUT_132_DEFAULT_STEP)
    {
      glBegin (GL_TRIANGLE_STRIP);
      engine_glut_132_Z (x,                                y, *cb_data_p);
      engine_glut_132_Z (x + ENGINE_GLUT_132_DEFAULT_STEP, y, *cb_data_p);
      engine_glut_132_Z (x,                                y + ENGINE_GLUT_132_DEFAULT_STEP, *cb_data_p);
      glEnd ();
    } // end FOR
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}

void
engine_glut_132_idle (void)
{
  //static float vel0 = -100.0;
  //static double t0 = -1.;
  //double t, dt;
  //t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  //if (t0 < 0.)
  //  t0 = t;
  //dt = t - t0;
  //t0 = t;

  //Zrot += Zstep * dt;

  //Xpos += Xvel * dt;
  //if (Xpos >= Xmax) {
  //  Xpos = Xmax;
  //  Xvel = -Xvel;
  //  Zstep = -Zstep;
  //}
  //if (Xpos <= Xmin) {
  //  Xpos = Xmin;
  //  Xvel = -Xvel;
  //  Zstep = -Zstep;
  //}
  //Ypos += Yvel * dt;
  //Yvel += G * dt;
  //if (Ypos < Ymin) {
  //  Ypos = Ymin;
  //  if (vel0 == -100.0)
  //    vel0 = fabs (Yvel);
  //  Yvel = vel0;
  //}
  glutPostRedisplay ();
}

void
engine_glut_132_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_132_idle);
  else
    glutIdleFunc (NULL);
}

void
engine_glut_132_Z (int x, int y, struct Engine_OpenGL_GLUT_132_CBData& cbData_in)
{
  float v = x - ENGINE_GLUT_132_DEFAULT_WIDTH / 2.0f;
  float w = y - ENGINE_GLUT_132_DEFAULT_HEIGHT / 2.0f;
  cbData_in.x = v * v + w * w - v * w / static_cast<double> (ENGINE_GLUT_132_DEFAULT_WIDTH) / static_cast<double> (ENGINE_GLUT_132_DEFAULT_WIDTH) - cbData_in.f;
  float n =
    //Common_GL_Tools::map (static_cast<float> (cbData_in.noise.GetValue (cbData_in.x, 0.0, 0.0)),
    //                      -1.0f, 1.0f, 0.0f, 1.0f);
    Common_GL_Tools::map (static_cast<float> (cbData_in.noise.Evaluate (cbData_in.x, 0.0, 0.0)),
                          -1.0f, 1.0f, 0.0f, 1.0f);
  glVertex3f (x + std::cos (n * cbData_in.a) * ENGINE_GLUT_132_DEFAULT_M / 7.0f,
              y + std::sin (n) * ENGINE_GLUT_132_DEFAULT_M / 7.0f,
              0.0f);
}
