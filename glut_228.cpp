#include "stdafx.h"

#include "glut_228.h"

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

#include "defines_10.h"
#include "engine_common.h"

void
engine_glut_228_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  150.0, -300.0);
  //glOrtho (0.0, static_cast<GLdouble> (width_in), 0.0, static_cast<GLdouble> (height_in), 150.0, -300.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_228_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 'x':
      clear (*cb_data_p);
      break;
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_228_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
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
engine_glut_228_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
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
engine_glut_228_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
    cb_data_p->leftButtonIsDown = (state == GLUT_DOWN);
}

void
engine_glut_228_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (cb_data_p->leftButtonIsDown)
    addBall (*cb_data_p,
             static_cast<float> (x), static_cast<float> (y),
             Common_Tools::getRandomNumber (1.0f, ENGINE_GLUT_228_DEFAULT_MAX_RADIUS));
}

void
engine_glut_228_timer (int v)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_228_timer, v);
}

void
engine_glut_228_draw (void)
{
  struct Engine_OpenGL_GLUT_228_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_228_CBData*> (glutGetWindowData ());
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
  //glBegin (GL_LINES);
  //glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  //glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  //glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  //glEnd ();

  updateBalls (*cb_data_p);

  // update uniforms
  glProgramUniform1fv (cb_data_p->programId, cb_data_p->ballsXLoc,
                       static_cast<int> (cb_data_p->ballsX.size ()),
                       cb_data_p->ballsX.size () ? &cb_data_p->ballsX[0] : NULL);
  glProgramUniform1fv (cb_data_p->programId, cb_data_p->ballsYLoc,
                       static_cast<int> (cb_data_p->ballsY.size ()),
                       cb_data_p->ballsY.size () ? &cb_data_p->ballsY[0] : NULL);
  glProgramUniform1fv (cb_data_p->programId, cb_data_p->radiiLoc,
                       static_cast<int> (cb_data_p->radii.size ()),
                       cb_data_p->radii.size () ? &cb_data_p->radii[0] : NULL);
  glProgramUniform1i (cb_data_p->programId, cb_data_p->ballsLoc,
                      static_cast<int> (cb_data_p->ballsX.size ()));
  glProgramUniform1f (cb_data_p->programId, cb_data_p->distScaleLoc,
                      cb_data_p->distScale);
  glProgramUniform1f (cb_data_p->programId, cb_data_p->colorOffsetLoc,
                      cb_data_p->colorOffset);
  glProgramUniform1f (cb_data_p->programId, cb_data_p->widthLoc,
                      static_cast<float> (ENGINE_GLUT_228_DEFAULT_WIDTH));
  glProgramUniform1f (cb_data_p->programId, cb_data_p->heightLoc,
                      static_cast<float> (ENGINE_GLUT_228_DEFAULT_HEIGHT));

  glTranslatef (-static_cast<GLfloat> (ENGINE_GLUT_228_DEFAULT_WIDTH / 2.0f),
                -static_cast<GLfloat> (ENGINE_GLUT_228_DEFAULT_HEIGHT / 2.0f),
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
engine_glut_228_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_228_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_228_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
addBall (struct Engine_OpenGL_GLUT_228_CBData& CBData_in, float x, float y, float r)
{
  CBData_in.ballsX.push_back (x);
  CBData_in.ballsY.push_back (y);
  CBData_in.radii.push_back (r);

  CBData_in.velocX.push_back (Common_Tools::getRandomNumber (-1.0f, 1.0f));
  CBData_in.velocY.push_back (Common_Tools::getRandomNumber (-1.0f, 1.0f));
}

void
updateBalls (struct Engine_OpenGL_GLUT_228_CBData& CBData_in)
{
  for (size_t i = 0; i < CBData_in.velocX.size (); ++i)
  {
    // move the ball
    CBData_in.ballsX[i] += CBData_in.velocX[i] * ENGINE_GLUT_228_DEFAULT_SPEED;
    CBData_in.ballsY[i] += CBData_in.velocY[i] * ENGINE_GLUT_228_DEFAULT_SPEED;

    // collide with window edges
    if (static_cast<int> (CBData_in.ballsX[i] + CBData_in.radii[i]) >= ENGINE_GLUT_228_DEFAULT_WIDTH ||
        static_cast<int> (CBData_in.ballsX[i] - CBData_in.radii[i]) <= 0)
      CBData_in.velocX[i] *= -1.0f;
    if (static_cast<int> (CBData_in.ballsY[i] + CBData_in.radii[i]) >= ENGINE_GLUT_228_DEFAULT_HEIGHT ||
        static_cast<int> (CBData_in.ballsY[i] - CBData_in.radii[i]) <= 0)
      CBData_in.velocY[i] *= -1.0f;
  } // end FOR
}

void
clear (struct Engine_OpenGL_GLUT_228_CBData& CBData_in)
{
  CBData_in.ballsX.clear ();
  CBData_in.ballsY.clear ();
  CBData_in.radii.clear ();
  CBData_in.velocX.clear ();
  CBData_in.velocY.clear ();
}
