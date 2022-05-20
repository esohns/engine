#include "stdafx.h"

#include "glut_3.h"

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines.h"
#include "engine_common.h"

void
engine_glut_3_reshape (int width_in, int height_in)
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
engine_glut_3_key (unsigned char k, int x, int y)
{
  //struct OpenGL_GLUT_WindowData* cb_data_p = 
  //  static_cast<struct OpenGL_GLUT_WindowData*> (glutGetWindowData ());
  //ACE_ASSERT (cb_data_p);

  switch (k) {
  case 27:  /* Escape */
    //exit (0);
    glutLeaveMainLoop ();

    //ACE_ASSERT (cb_data_p->queue);
    //ACE_Message_Block* message_block_p = NULL;
    //ACE_NEW_NORETURN (message_block_p,
    //                  ACE_Message_Block ());
    //ACE_ASSERT (message_block_p);
    //cb_data_p->queue->enqueue (message_block_p);

    break;
  }
}

void
engine_glut_3_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
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
engine_glut_3_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
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
engine_glut_3_timer (int v)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    cb_data_p->angle += 1.0;
    if (cb_data_p->angle > 360.0)
    {
      cb_data_p->angle -= 360.0;
    }
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_3_timer, v);
}

void
engine_glut_3_draw (void)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // compute terrain
  cb_data_p->yOffset += 0.03;
  double xoff;
  double yoff = cb_data_p->yOffset;
  for (int y = 0; y < cb_data_p->rows; ++y)
  {
    xoff = 0.0;
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      //cb_data_s.terrain[y * cb_data_s.rows + x] = Common_Tools::getRandomNumber (-10, 10);
      cb_data_p->terrain[y * cb_data_p->rows + x] =
        static_cast<float> ((cb_data_p->module.GetValue (cb_data_p->x + xoff, cb_data_p->y + yoff, cb_data_p->z) * 100.0) - 50.0);
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

  //glTranslatef (cb_data_p->size.cx / 2.0F, cb_data_p->size.cy / 2.0F, 0);
  //glRotatef (static_cast<float> (M_PI) / 3.0F, 1.0F, 0.0F, 0.0F);

  // Set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are ten units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glColor3f (1.0F, 1.0F, 1.0F);
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      glVertex3f (x * cb_data_p->scaleFactor, y * cb_data_p->scaleFactor, cb_data_p->terrain[y * cb_data_p->rows + x]);
      glVertex3f (x * cb_data_p->scaleFactor, (y + 1) * cb_data_p->scaleFactor, cb_data_p->terrain[(y + 1) * cb_data_p->rows + x]);
    } // end FOR
    glEnd ();
  } // end FOR
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}

void
engine_glut_3_idle(void)
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
engine_glut_3_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_3_idle);
  else
    glutIdleFunc (NULL);
}
