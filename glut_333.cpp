#include "stdafx.h"

#include "glut_333.h"

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

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"
#include "engine_common.h"

void
engine_glut_333_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  1.0, -1.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_333_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_333_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_PAGE_UP:
      cb_data_p->camera.position.z -= 100.0f;
      break;
    case GLUT_KEY_PAGE_DOWN:
      cb_data_p->camera.position.z += 100.0f;
      break;
    case GLUT_KEY_LEFT:
      cb_data_p->camera.rotation.z -= 0.5f;
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.rotation.z += 0.5f;
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 1200.0f;
      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_333_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
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
engine_glut_333_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
      } // end IF
      break;
    }
    case 3:
      cb_data_p->camera.position.y -= 10.0f;
      cb_data_p->camera.position.z -= 10.0f;
      break;
    case 4:
      cb_data_p->camera.position.y += 10.0f;
      cb_data_p->camera.position.z += 10.0f;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_333_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_333_timer (int v)
{
  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    //cb_data_p->angle += 1.0f;
    //if (cb_data_p->angle > 360.0f)
    //{
    //  cb_data_p->angle -= 360.0f;
    //}
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_333_timer, v);
}

void
engine_glut_333_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_333_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_333_CBData*> (glutGetWindowData ());
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
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glColor3f (1.0f, 1.0f, 1.0f);

  glRotatef ((180.0f / static_cast<float> (M_PI)) * (frame_count_i * 0.009f), 0.0f, 1.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (frame_count_i * 0.001f), 1.0f, 0.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (frame_count_i * 0.0015f), 0.0f, 0.0f, 1.0f);

  float s = 0.0f;
  float t = 0.0f;
  float lastx = 0.0f;
  float lasty = 0.0f;
  float lastz = 0.0f;

  cb_data_p->angleStep += 0.01f;
  cb_data_p->radiusNoise += 0.001f;

  float noiseLevel = cb_data_p->angleStep;

  while (t <= 180.0f)
  {
    t += 2.0f;
    float radianT = (static_cast<float> (M_PI) / 180.0f) * t;

    noiseLevel += 0.005f;

    float radius =
      (Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (cb_data_p->radiusNoise, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 150.0f * std::sin (t)) + 250.0f;

    lastx = 0.0f;

    s = 0.0f;
    while (s <= 360.0f)
    {
      s += 10.0f;
      float radianS = (static_cast<float> (M_PI) / 180.0f) * s;

      float thisx = radius * std::cos (radianS) * std::sin (radianT);
      float thisy = radius * std::sin (radianS) * std::sin (radianT);
      float thisz = radius * std::cos (radianT);

      if (likely (lastx != 0.0f))
      {
        glColor3f (1.0f, 1.0f, 1.0f);

        glBegin (GL_LINES);
        glVertex3f (lastx, lasty, lastz);
        glVertex3f (thisx, thisy, thisz);
        glEnd ();
      } // end IF

      lastx = thisx;
      lasty = thisy;
      lastz = thisz;

      glPushMatrix ();

      glTranslatef (thisx, thisy, thisz);

      float r, g, b;
      Common_Image_Tools::HSVToRGB (std::fmod (std::abs (thisx + thisy + thisz), 360.0f), 1.0f, 1.0f, r, g, b);
      glColor3f (r, g, b);

      drawCircle_2 (0.0f, 0.0f, 3.0f, 32, !cb_data_p->wireframe);

      glPopMatrix ();
    } // end WHILE
  } // end WHILE

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_333_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_333_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_333_idle : NULL);
}

void
drawCircle_2 (float cx, float cy, float r, int num_segments, bool filled)
{
  if (filled)
  {
    glBegin (GL_TRIANGLE_FAN);
    glVertex2f (cx, cy);
    for (int ii = 0; ii < num_segments + 1; ii++)
    {
      float theta = 2.0f * 3.1415926f * float (ii) / float (num_segments);

      float x = r * std::cos (theta);
      float y = r * std::sin (theta);

      glVertex2f (x + cx, y + cy);
    } // end FOR
    glEnd ();
  } // end IF
  else
  {
    glBegin (GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
      float theta = 2.0f * 3.1415926f * float (ii) / float (num_segments);

      float x = r * std::cos (theta);
      float y = r * std::sin (theta);

      glVertex2f (x + cx, y + cy);
    } // end FOR
    glEnd ();
  } // end ELSE
}
