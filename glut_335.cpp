#include "stdafx.h"

#include "glut_335.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_15.h"
#include "engine_common.h"

void
engine_glut_335_reshape (int width_in, int height_in)
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
engine_glut_335_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_335_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 2000.0f;

      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_335_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
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
engine_glut_335_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
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
engine_glut_335_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_335_timer (int v)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_335_timer, v);
}

void
engine_glut_335_draw (void)
{
  struct Engine_OpenGL_GLUT_335_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_335_CBData*> (glutGetWindowData ());
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

  glRotatef ((180.0f / static_cast<float> (M_PI)) * (cb_data_p->initRot[0] + cb_data_p->count / 300.0f), 1.0f, 0.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (cb_data_p->initRot[1] + cb_data_p->count / 100.0f * cb_data_p->direction), 0.0f, 1.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (cb_data_p->initRot[2] + cb_data_p->count / 200.0f), 0.0f, 0.0f, 1.0f);

  //directionalLight(0, 0, 100, -1, 1, -1);
  //ambientLight(220, 20, 70);

  for (std::vector<cube>::iterator iterator = cb_data_p->cubes.begin ();
       iterator != cb_data_p->cubes.end ();
       ++iterator)
    (*iterator).update (ENGINE_GLUT_335_DEFAULT_NUMBER_OF_TILE_ANGLES, ENGINE_GLUT_335_DEFAULT_NUMBER_OF_TILE_Y,
                        cb_data_p->wireframe,
                        &cb_data_p->noise,
                        cb_data_p->initNoiseXYZ,
                        cb_data_p->noiseRangeXYZ,
                        cb_data_p->count,
                        ENGINE_GLUT_335_DEFAULT_NOISE_STEP,
                        cb_data_p->colH,
                        cb_data_p->direction);

  cb_data_p->count++;

  glutSwapBuffers ();
}

void
engine_glut_335_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_335_visible (int vis)
{
  glutIdleFunc ((vis == GLUT_VISIBLE) ? engine_glut_335_idle : NULL);
}
