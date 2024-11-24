#include "stdafx.h"

#include "glut_426.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_19.h"

void
engine_glut_426_reshape (int width_in, int height_in)
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
engine_glut_426_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_426_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 750.0f;

      cb_data_p->camera.rotation.y = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_426_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
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
engine_glut_426_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_426_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = ENGINE_GLUT_426_DEFAULT_HEIGHT - y;
}

void
engine_glut_426_timer (int v)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_426_timer, v);
}

void
engine_glut_426_draw (void)
{
  struct Engine_OpenGL_GLUT_426_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_426_CBData*> (glutGetWindowData ());
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

  glColor3f (1.0f, 1.0f, 1.0f);

  static float P = static_cast<float> (M_PI) / 68.0f;
  for (float u = 0.0f; u < 2.0f * static_cast<float> (M_PI); u += P)
    for (float v = 0.0f; v < static_cast<float> (M_PI); v += P)
    {
      glPushMatrix ();
      float r = std::sin (v + cb_data_p->f / 9.0f) * 200.0f;
      float U = u + cb_data_p->f;
      float Z = std::cos (U) * std::sin (v) * r;
      float S = Z * std::cos (u + v + cb_data_p->f * 1.2f) * 9.0f;

      glRotatef (U * 180.0f / static_cast<float> (M_PI), 1.0f, 0.0f, 0.0f);
      glRotatef (U * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
      glTranslatef (std::cos (v) * 200.0f, std::sin (Z / static_cast<float> (ENGINE_GLUT_426_DEFAULT_W)) * std::sin (v) * Z, Z);
      
      cb_data_p->wireframe ? glutWireSphere  (S / 1000.0f, 12, 12)
                           : glutSolidSphere (S / 1000.0f, 12, 12);
      glPopMatrix ();
    } // end FOR

  glutSwapBuffers ();

  cb_data_p->f += 0.05f;
}

void
engine_glut_426_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_426_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_426_idle);
  else
    glutIdleFunc (NULL);
}
