#include "stdafx.h"

#include "glut_25.h"

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

#include "common_gl_common.h"
#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_ui_defines.h"

#include "common_ui_gtk_manager_common.h"

#include "defines.h"
#include "engine_common.h"


struct spherical
{
  float r;
  float theta;
  float phi;
};

struct spherical
spherical (float x, float y, float z)
{
  struct spherical point_coordinates;
  point_coordinates.r = sqrt (x * x + y * y + z * z);
  point_coordinates.theta = atan2 (sqrt (x * x + y * y), z);
  point_coordinates.phi = atan2 (y, x);
  return point_coordinates;
}

void
engine_glut_25_generate (struct Engine_OpenGL_GLUT_25_CBData& CBData_in)
{
  for (int i = 0; i < CBData_in.dimension; i++)
    for (int j = 0; j < CBData_in.dimension; j++)
    {
      boolean edge = false;
      for (int k = 0; k < CBData_in.dimension; k++)
      {
        float x = Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (CBData_in.dimension), -1.0f, 1.0f);
        float y = Common_GL_Tools::map (static_cast<float> (j), 0.0f, static_cast<float> (CBData_in.dimension), -1.0f, 1.0f);
        float z = Common_GL_Tools::map (static_cast<float> (k), 0.0f, static_cast<float> (CBData_in.dimension), -1.0f, 1.0f);

        glm::fvec3 zeta = { 0.0f, 0.0f, 0.0f };
        int n = 8;
        int maxiterations = 20;
        int iteration = 0;
        while (true)
        {
          struct spherical c = spherical (zeta.x, zeta.y, zeta.z);
          float newx = static_cast<float> (pow (c.r, n)) * sin (c.theta * n) * cos (c.phi * n);
          float newy = static_cast<float> (pow (c.r, n) * sin (c.theta * n)) * sin (c.phi * n);
          float newz = static_cast<float> (pow (c.r, n)) * cos (c.theta * n);
          zeta.x = newx + x;
          zeta.y = newy + y;
          zeta.z = newz + z;
          iteration++;
          if (c.r > 2)
          {
            if (edge)
            {
              edge = false;
            } // end IF
            break;
          } // end IF
          if (iteration > maxiterations)
          {
            if (!edge)
            {
              edge = true;
              CBData_in.mandelbulb.push_back({x, y, z});
            } // end IF
            break;
          } // end IF
        } // end WHILE
      } // end FOR
    } // end FOR
}

void
engine_glut_25_reshape (int width_in, int height_in)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

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

  // set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);


  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
}

void
engine_glut_25_key_special (int key_in,
                            int x,
                            int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.rotation.x -= 0.5f;
      break;
    case GLUT_KEY_DOWN:
      cb_data_p->camera.rotation.x += 0.5f;
      break;
    case GLUT_KEY_HOME:
      cb_data_p->camera.position.x = 0.0F;
      cb_data_p->camera.position.y = -200.0f;
      cb_data_p->camera.position.z = -600.0f;

      cb_data_p->camera.rotation.x = 0.0f;
      cb_data_p->camera.rotation.y = 0.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_25_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_UP)
    {
      //cb_data_p->spinning = false;
      cb_data_p->xangle += cb_data_p->xDeltaAngle;
      cb_data_p->xOrigin = -1;
      cb_data_p->yangle += cb_data_p->yDeltaAngle;
      cb_data_p->yOrigin = -1;

      cb_data_p->camera.rotation.x = 0.0F;
      cb_data_p->camera.rotation.y = 0.0F;
      cb_data_p->camera.rotation.z = 0.0F;
    } // end IF
    else
    {
      //cb_data_p->spinning = true;
      cb_data_p->xOrigin = x;
      cb_data_p->yOrigin = y;
    } // end ELSE
  } // end IF
}

void
engine_glut_25_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData());
  ACE_ASSERT (cb_data_p);

  // this will only be true when the left button is down
  if (cb_data_p->xOrigin >= 0)
  {
    cb_data_p->xDeltaAngle = (x - cb_data_p->xOrigin) * 0.001f;

    // update camera's direction
    cb_data_p->camera.rotation.x = std::sin (cb_data_p->xangle + cb_data_p->xDeltaAngle);
    cb_data_p->camera.rotation.z = -std::cos (cb_data_p->xangle + cb_data_p->xDeltaAngle);
  } // end IF

  if (cb_data_p->yOrigin >= 0)
  {
    cb_data_p->yDeltaAngle = (y - cb_data_p->yOrigin) * 0.001f;

    // update camera's direction
    cb_data_p->camera.rotation.y = std::sin (cb_data_p->yangle + cb_data_p->yDeltaAngle);
    cb_data_p->camera.rotation.z = -std::cos (cb_data_p->yangle + cb_data_p->yDeltaAngle);
  } // end IF
}

void
engine_glut_25_timer (int v)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    //cb_data_p->xangle += 1.0;
    if (cb_data_p->xangle > 360.0)
    {
      cb_data_p->xangle -= 360.0;
    } // end IF

    //cb_data_p->yangle += 1.0;
    if (cb_data_p->yangle > 360.0)
    {
      cb_data_p->yangle -= 360.0;
    } // end IF
    //glutPostRedisplay ();
  //} // end IF

  glutTimerFunc (1000 / 30, engine_glut_25_timer, v);
}

void
engine_glut_25_draw (void)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT);
  //COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  //COMMON_GL_ASSERT;
  glLoadIdentity ();
  //COMMON_GL_ASSERT;

  // rotate the camera
  //glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
  //                                         glm::radians (cb_data_p->camera.rotation.x),
  //                                         glm::vec3 (1.0f, 0.0f, 0.0f));
  //rotation_matrix *= glm::rotate (glm::mat4 (1.0f),
  //                                glm::radians (cb_data_p->camera.rotation.y),
  //                                glm::vec3 (0.0f, 1.0f, 0.0f));
  //rotation_matrix *= glm::rotate (glm::mat4 (1.0f),
  //                                glm::radians (cb_data_p->camera.rotation.z),
  //                                glm::vec3 (0.0f, 0.0f, 1.0f));
  //glm::vec4 pos_rot_h =
  //  rotation_matrix * glm::vec4 (cb_data_p->camera.position,
  //                               1.0f);
  //cb_data_p->camera.position = glm::vec3 (pos_rot_h);
  //// set the camera
  //gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
  //           cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
  //           cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are hundred units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();
  //COMMON_GL_ASSERT;

  glColor3ub (255, 255, 255);
  //glTranslatef (ENGINE_GLUT_25_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_25_DEFAULT_HEIGHT / 2.0f, 0.0f);

  glBegin (GL_POINTS);
  for (std::vector<glm::fvec3>::const_iterator i = cb_data_p->mandelbulb.begin ();
       i != cb_data_p->mandelbulb.end ();
       ++i)
    glVertex3f ((*i).x * 200, (*i).y * 200, (*i).z * 200);
  glEnd ();

  glFlush ();
  //COMMON_GL_ASSERT;

  glutSwapBuffers ();
}
