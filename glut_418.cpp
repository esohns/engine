#include "stdafx.h"

#include "glut_418.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

void
engine_glut_418_reshape (int width_in, int height_in)
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
engine_glut_418_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_418_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 400.0f;
      cb_data_p->camera.position.z = 750.0f;

      cb_data_p->camera.rotation.y = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_418_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
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
engine_glut_418_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_418_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = ENGINE_GLUT_418_DEFAULT_HEIGHT - y;
}

void
engine_glut_418_timer (int v)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_418_timer, v);
}

void
engine_glut_418_draw (void)
{
  struct Engine_OpenGL_GLUT_418_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_418_CBData*> (glutGetWindowData ());
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

  if (cb_data_p->mouse_0_ButtonDown)
  {
    cb_data_p->falling = !cb_data_p->falling;
    cb_data_p->counter = 0;
  } // end IF

  static int frame_count_i = 1;

  glPushMatrix ();
  glTranslatef (0.0f, 200.0f, 0.0f);
  cb_data_p->spin =
    Common_Math_Tools::lerp (cb_data_p->spin, static_cast<float> (cb_data_p->mousex) - ENGINE_GLUT_418_DEFAULT_WIDTH / 2.0f, 0.01f);
  glRotatef (cb_data_p->spin / 150.0f + frame_count_i / 150.0f, 0.0f, 1.0f, 0.0f);

  static float radius = (cb_data_p->size / 30.0f);
  float cnt = 0.0f;
  float r, g, b;
  for (std::vector<bead>::iterator iterator = cb_data_p->beads.begin ();
       iterator != cb_data_p->beads.end ();
       ++iterator)
  {
    Common_Image_Tools::HSVToRGB (((static_cast<int> (cnt) % 256) / 255.0f) * 360.0f,
                                  200.0f / 255.0f,
                                  255.0f / 255.0f,
                                  r, g, b);

    glColor3f (r, g, b);
    cnt += 1.5f;

    glPushMatrix ();
    glTranslatef ((*iterator).position.x, (*iterator).position.y, (*iterator).position.z);
    cb_data_p->wireframe ? glutWireSphere (radius, 8, 8)
                         : glutSolidSphere (radius, 8, 8);
    glPopMatrix ();

    if (cb_data_p->falling)
      (*iterator).update (cb_data_p->counter);
    else
      (*iterator).arrange (cb_data_p->counter);
  } // end FOR
  glPopMatrix ();

  cb_data_p->counter += 2;

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_418_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_418_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_418_idle);
  else
    glutIdleFunc (NULL);
}
