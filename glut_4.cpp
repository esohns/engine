#include "stdafx.h"

#include "glut_3.h"

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

#include "common_gl_common.h"
#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_ui_defines.h"

#include "common_ui_gtk_manager_common.h"

#include "defines.h"
#include "engine_common.h"

void
engine_glut_4_draw (void)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // compute terrain
  cb_data_p->offset += cb_data_p->step;
  double xoff;
  double yoff = 0.0;
  double zoff = cb_data_p->offset;
  for (int y = 0; y < cb_data_p->rows; ++y)
  {
    xoff = 0.0;
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      //cb_data_s.terrain[y * cb_data_s.rows + x] = Common_Tools::getRandomNumber (-10, 10);
      cb_data_p->terrain[y * cb_data_p->rows + x] =
        static_cast<float> ((cb_data_p->module.GetValue (cb_data_p->x + xoff, cb_data_p->y + yoff, cb_data_p->z + zoff) * cb_data_p->level));
      xoff += cb_data_p->step;
    } // end FOR
    yoff += cb_data_p->step;
  } // end FOR

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
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  glm::vec3 rotation_center (cb_data_p->size.cx / 2.0f,
                             cb_data_p->size.cy / 2.0f,
                             cb_data_p->camera.position.z);
#else
  glm::vec3 rotation_center (cb_data_p->size.width / 2.0f,
                             cb_data_p->size.height / 2.0f,
                             cb_data_p->camera.position.z);
#endif // ACE_WIN32 || ACE_WIN64
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

  Common_GL_Color_t color_s = { 255, 255, 255 };
  if (!cb_data_p->color)
    glColor3ub (color_s.r, color_s.g, color_s.b);

  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      if (cb_data_p->color)
      {
        color_s =
          Common_GL_Tools::toRGBColor (((static_cast<float> (cb_data_p->terrain[y * cb_data_p->rows + x]) / static_cast<float> (cb_data_p->level)) + 1.0F) / 2.0F);
        glColor3ub (color_s.r, color_s.g, color_s.b);
      } // end IF
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> (y * cb_data_p->scaleFactor), cb_data_p->terrain[y * cb_data_p->rows + x]);
      if (cb_data_p->color)
      {
        color_s =
          Common_GL_Tools::toRGBColor (((static_cast<float> (cb_data_p->terrain[(y + 1) * cb_data_p->rows + x]) / static_cast<float> (cb_data_p->level)) + 1.0F) / 2.0F);
        glColor3ub (color_s.r, color_s.g, color_s.b);
      } // end IF
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ((y + 1) * cb_data_p->scaleFactor), cb_data_p->terrain[(y + 1) * cb_data_p->rows + x]);
    } // end FOR
    glEnd ();
  } // end FOR
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}
