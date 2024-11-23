#include "stdafx.h"

#include "glut_423.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_19.h"

void
engine_glut_423_reshape (int width_in, int height_in)
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
engine_glut_423_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_423_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
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
engine_glut_423_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
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
engine_glut_423_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_423_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = ENGINE_GLUT_423_DEFAULT_HEIGHT - y;
}

void
engine_glut_423_timer (int v)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_423_timer, v);
}

void
engine_glut_423_draw (void)
{
  struct Engine_OpenGL_GLUT_423_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_423_CBData*> (glutGetWindowData ());
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

  glPushMatrix ();

  float rotationX =
    Common_GL_Tools::map (static_cast<float> (cb_data_p->mousey), static_cast<float> (0), static_cast<float> (ENGINE_GLUT_423_DEFAULT_HEIGHT - 1), static_cast<float> (-M_PI), static_cast<float> (M_PI));
  float rotationY =
    Common_GL_Tools::map (static_cast<float> (cb_data_p->mousex), static_cast<float> (0), static_cast<float> (ENGINE_GLUT_423_DEFAULT_WIDTH - 1), static_cast<float> (-M_PI), static_cast<float> (M_PI));
  glRotatef (rotationX * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  glRotatef (rotationY * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);

  float r, g, b;
  for (int i = 0; i < 20; i++)
  {
    glRotatef ((static_cast<float> (M_PI) / 22.5f) * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);

    Common_Image_Tools::HSVToRGB (((i * 12.0f) / 255.0f) * 360.0f,
                                  255.0f / 255.0f,
                                  255.0f / 255.0f,
                                  r, g, b);
    glColor3f (r, g, b);

    glBegin (GL_LINE_STRIP);
    for (float theta_deg = 0.5f; theta_deg < 180.0f; theta_deg += 0.5f)
    {
      float phi_deg =
        Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (theta_deg * 0.008f + frame_count_i * 0.02f, 0.0, 0.0)), -1.0f, 1.0f, -1080.0f, 1080.0f);

      float theta_rad = theta_deg * (static_cast<float> (M_PI) / 180.0f);
      float phi_rad = phi_deg * (static_cast<float> (M_PI) / 180.0f);

      float x = ENGINE_GLUT_423_DEFAULT_RADIUS * std::sin (theta_rad) * std::cos (phi_rad);
      float y = ENGINE_GLUT_423_DEFAULT_RADIUS * std::sin (theta_rad) * std::sin (phi_rad);
      float z = ENGINE_GLUT_423_DEFAULT_RADIUS * std::cos (theta_rad);

      glVertex3f (x, y, z);
    } // end FOR
    glEnd ();
  } // end FOR
  glPopMatrix ();

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_423_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_423_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_423_idle);
  else
    glutIdleFunc (NULL);
}
