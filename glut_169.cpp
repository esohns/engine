#include "stdafx.h"

#include "glut_169.h"

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

#include "defines_7.h"
#include "engine_common.h"

void
engine_glut_169_reshape (int width_in, int height_in)
{
  glViewport (0, 0,
              static_cast<GLsizei> (width_in), static_cast<GLsizei> (height_in));

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
engine_glut_169_key (unsigned char key_in,
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
engine_glut_169_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.x = ENGINE_GLUT_169_DEFAULT_WIDTH / 2.0f;
      cb_data_p->camera.position.y = ENGINE_GLUT_169_DEFAULT_HEIGHT / 2.0f;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_169_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
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
engine_glut_169_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
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
engine_glut_169_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glProgramUniform2f (cb_data_p->programId, cb_data_p->mouseLoc, static_cast<GLfloat> (x),
                                                                 static_cast<GLfloat> (ENGINE_GLUT_169_DEFAULT_HEIGHT - y));

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
engine_glut_169_timer (int v)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_169_timer, v);
}

void
engine_glut_169_draw (void)
{
  struct Engine_OpenGL_GLUT_169_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_169_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  static int frame_count_i = 1;

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  // rotate the camera
  glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
                                           glm::radians (cb_data_p->camera.rotation.z),
                                           glm::vec3 (0.0f, 0.0f, 1.0f));
  glm::vec3 rotation_center (ENGINE_GLUT_169_DEFAULT_WIDTH / 2.0f,
                             ENGINE_GLUT_169_DEFAULT_HEIGHT / 2.0f,
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

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are ten units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glProgramUniform1f (cb_data_p->programId, cb_data_p->frameCountLoc, static_cast<GLfloat> (frame_count_i));

  //glBegin (GL_QUADS);
  //glColor3f (1.0F, 1.0F, 1.0F);
  //glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, 0, 0); glVertex3i (0, 0, 0);
  //glVertex3i (0, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0); glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0);
  //glEnd ();
  //glBegin (GL_TRIANGLES);
  //glColor3f (1.0F, 1.0F, 1.0F);
  //glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, 0, 0); glVertex3i (0, 0, 0); glVertex3i (0, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0);
  //glVertex3i (0, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0); glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0); glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, 0, 0);
  //glEnd ();
  //glBegin (GL_TRIANGLE_STRIP);
  //glVertex3i (0, 0, 0); glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, 0, 0); glVertex3i (0, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0);
  //glVertex3i (ENGINE_GLUT_169_DEFAULT_WIDTH - 1, ENGINE_GLUT_169_DEFAULT_HEIGHT - 1, 0);
  //glEnd ();
  glColor3f (1.0F, 1.0F, 1.0F);
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

  glFlush ();

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_169_idle (void)
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
engine_glut_169_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_169_idle);
  else
    glutIdleFunc (NULL);
}
