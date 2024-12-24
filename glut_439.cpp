#include "stdafx.h"

#include "glut_439.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

void
engine_glut_439_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_439_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_439_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      cb_data_p->camera.rotation.y -= 0.5f;
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.rotation.y += 0.5f;
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = -200.0f;
      cb_data_p->camera.position.z = 750.0f;

      cb_data_p->camera.rotation.y = 0.0f;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_439_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (entry_in)
  {
    case 0:
      cb_data_p->wireframe = !cb_data_p->wireframe;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_439_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_439_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = y;
}

void
engine_glut_439_timer (int v)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_439_timer, v);
}

void
engine_glut_439_draw (void)
{
  struct Engine_OpenGL_GLUT_439_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_439_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  // rotate the camera
  glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
                                           glm::radians (cb_data_p->camera.rotation.y),
                                           glm::vec3 (0.0f, 1.0f, 0.0f));
  glm::vec3 rotation_center (0.0f, 0.0f, 0.0f);
  glm::vec4 pos_rot_h =
    rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center, 1.0f);
  cb_data_p->camera.position = glm::vec3 (pos_rot_h) + rotation_center;

  // set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  static int frame_count_i = 1;

  //glTranslatef (ENGINE_GLUT_439_DEFAULT_WIDTH / 2.0f, ENGINE_GLUT_439_DEFAULT_HEIGHT / 2.0f, ENGINE_GLUT_439_DEFAULT_WIDTH / 2.0f);
  float angle_f =
    static_cast<float> (M_PI_2) - static_cast<float> (M_PI) / 6.0f + static_cast<float> (M_PI) / 12.0f * std::sin (static_cast<float> (M_PI) / 60.0f * frame_count_i);
  glRotatef (angle_f * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  glRotatef ((static_cast<float> (M_PI) / 240.0f * frame_count_i) * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);

  for (int i = 0; i <= cb_data_p->top; i++)
  {
    int l = static_cast<int> (cb_data_p->sz[i] / ENGINE_GLUT_439_DEFAULT_BB);
    if (cb_data_p->random[i] < 0.6f)
    {
      for (int j = 0; j < l; j++)
      {
        Common_GL_Color_t c = cb_data_p->color[(i + j) & cb_data_p->top];
        glColor3ub (c.r, c.g, c.b);
        quader (cb_data_p->xm[i],
                cb_data_p->ym[i],
                ENGINE_GLUT_439_DEFAULT_BB * j,
                cb_data_p->sz[i] - ENGINE_GLUT_439_DEFAULT_BB * j,
                ENGINE_GLUT_439_DEFAULT_BB);
      } // end FOR
    } // end IF
    else
    {
      Common_GL_Color_t c = cb_data_p->color[i];
      glColor3ub (c.r, c.g, c.b);
      quader (cb_data_p->xm[i],
              cb_data_p->ym[i],
              0.0f,
              cb_data_p->sz[i],
              cb_data_p->sz[i] / 4.0f);

      for (int k = 0; k < cb_data_p->o[i]; k++)
        for (int h = 0; h < cb_data_p->o[i]; h++)
          quader (cb_data_p->xm[i] - cb_data_p->sz[i] / 2.0f + cb_data_p->sz[i] / cb_data_p->o[i] / 2.0f + cb_data_p->sz[i] / cb_data_p->o[i] * k,
                  cb_data_p->ym[i] - cb_data_p->sz[i] / 2.0f + cb_data_p->sz[i] / cb_data_p->o[i] / 2.0f + cb_data_p->sz[i] / cb_data_p->o[i] * h,
                  cb_data_p->sz[i] / 4.0f,
                  cb_data_p->sz[i] / cb_data_p->o[i] * 0.9f,
                  cb_data_p->sz[i] / cb_data_p->o[i] / 3.0f);
    } // end ELSE
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_439_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_439_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_439_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
quader (float x, float y, float z, float s, float b)
{
  glPushMatrix ();

  glTranslatef (x, y, z);
  glRectf (0.0f, 0.0f, s, s);

  glTranslatef (0.0f, 0.0f, b);
  glRectf (0.0f, 0.0f, s, s);

  glRotatef (static_cast<float> (M_PI_2) * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  for (int i = 0; i < 4; i++)
  {
    glPushMatrix ();
    glTranslatef (0.0f, -b / 2.0f, s / 2.0f);
    glRectf (0.0f, 0.0f, s, b);
    glPopMatrix ();

    glRotatef (static_cast<float> (M_PI_2) * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);
  } // end FOR

  glPopMatrix ();
}
