#include "stdafx.h"

#include "glut_18.h"

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
engine_glut_18_generate (struct Engine_OpenGL_GLUT_18_CBData& CBData_in)
{
  CBData_in.length *= 0.5f;

  std::string next_sentence_string;
  for (std::string::const_iterator i = CBData_in.sentence.begin ();
       i != CBData_in.sentence.end ();
       ++i)
  {
    bool found_b = false;
    for (rules_t::const_iterator j = CBData_in.rules.begin ();
         j != CBData_in.rules.end ();
         ++j)
      if (*i == (*j).first)
      {
        found_b = true;
        next_sentence_string += (*j).second;
        break;
      } // end IF
    if (!found_b)
      next_sentence_string += *i;
  } // end FOR
  CBData_in.sentence = next_sentence_string;
}

void
engine_glut_18_reshape (int width_in, int height_in)
{
  struct Engine_OpenGL_GLUT_18_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_18_CBData*> (glutGetWindowData ());
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
engine_glut_18_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_18_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_18_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    if (state == GLUT_UP)
    {
      cb_data_p->xangle += cb_data_p->xDeltaAngle;
      cb_data_p->xOrigin = -1;

      engine_glut_18_generate (*cb_data_p);
    } // end IF
    else
      cb_data_p->xOrigin = x;
  } // end IF
}

void
engine_glut_18_draw (void)
{
  struct Engine_OpenGL_GLUT_18_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_18_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT);
  //COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  //COMMON_GL_ASSERT;
  glLoadIdentity ();
  //COMMON_GL_ASSERT;

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are hundred units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();
  //COMMON_GL_ASSERT;

  glColor3ub (255, 255, 255);
  //glTranslatef (ENGINE_GLUT_18_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_18_DEFAULT_HEIGHT / 2.0f, 0.0f);

  for (std::string::const_iterator i = cb_data_p->sentence.begin ();
       i != cb_data_p->sentence.end ();
       ++i)
  {
    switch (*i)
    {
      case 'F':
      {
        glBegin (GL_LINES);
        glVertex3f (0.0f, 0.0f, 0.0f);
        glVertex3f (0.0f, cb_data_p->length, 0.0f);
        glEnd ();

        glTranslatef (0.0f, cb_data_p->length, 0.0f);
        break;
      }
      case '+':
      {
        glRotatef (cb_data_p->angle, 0.0f, 0.0f, 1.0f);
        //glRotatef (cb_data_p->angle * static_cast<float> (180.0 / M_PI), 1.0f, 0.0f, 0.0f);
        break;
      }
      case '-':
      {
        glRotatef (-cb_data_p->angle, 0.0f, 0.0f, 1.0f);
        //glRotatef (-cb_data_p->angle * static_cast<float> (180.0 / M_PI), 1.0f, 0.0f, 0.0f);
        break;
      }
      case '[':
      {
        glPushMatrix ();
        break;
      }
      case ']':
      {
        glPopMatrix ();
        break;
      }
      default:
        break;
    } // end SWITCH
  } // end FOR

  glFlush ();
  //COMMON_GL_ASSERT;

  glutSwapBuffers ();
}
