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
      bool edge = false;
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
          float newx = static_cast<float> (powf (c.r, static_cast<float> (n))) * sinf (c.theta * n) * cosf (c.phi * n);
          float newy = static_cast<float> (powf (c.r, static_cast<float> (n)) * sinf (c.theta * n)) * sinf (c.phi * n);
          float newz = static_cast<float> (powf (c.r, static_cast<float> (n))) * cosf (c.theta * n);
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
  gluLookAt (cb_data_p->camera.getX (), cb_data_p->camera.getY (), cb_data_p->camera.getZ (),
             cb_data_p->camera.getSightX (), cb_data_p->camera.getSightY (), cb_data_p->camera.getSightZ (),
             //0, 0, 0,
             0, 1, 0);


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

  cb_data_p->camera.setKeyboard (key_in, true);
}

void
engine_glut_25_key_down (unsigned char key_in,
                         int x,
                         int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->camera.setKeyboard (key_in, true);

  switch (key_in)
  {
    case 27:  /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_25_key_up (unsigned char key_in,
                       int x,
                       int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->camera.setKeyboard (key_in, false);
}

void
engine_glut_25_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData());
  ACE_ASSERT (cb_data_p);

  cb_data_p->camera.rotation (x, y);
}

void
engine_glut_25_draw (void)
{
  struct Engine_OpenGL_GLUT_25_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_25_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT);
  COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
  glLoadIdentity ();
  COMMON_GL_ASSERT;

  // rotate the camera
  cb_data_p->camera.translation ();
  // set the camera
  gluLookAt (cb_data_p->camera.getX (), cb_data_p->camera.getY (), cb_data_p->camera.getZ (),
             cb_data_p->camera.getSightX (), cb_data_p->camera.getSightY (), cb_data_p->camera.getSightZ (),
             //0, 0, 0,
             0, 1, 0);

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are hundred units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();
  COMMON_GL_ASSERT;

  glColor3ub (255, 255, 255);
  COMMON_GL_ASSERT;

  glBegin (GL_POINTS);
  for (std::vector<glm::fvec3>::const_iterator i = cb_data_p->mandelbulb.begin ();
       i != cb_data_p->mandelbulb.end ();
       ++i)
    glVertex3f ((*i).x * 200, (*i).y * 200, (*i).z * 200);
  glEnd ();
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}
