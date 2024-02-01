#include "stdafx.h"

#include "glut_271.h"

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

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_12.h"
#include "engine_common.h"

void
engine_glut_271_reshape (int width_in, int height_in)
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
engine_glut_271_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_271_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      cb_data_p->camera.rotation.z -= 0.5f;
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.rotation.z += 0.5f;
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 1500.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_271_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
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
engine_glut_271_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
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
engine_glut_271_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
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
engine_glut_271_timer (int v)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    cb_data_p->angle += 1.0f;
    if (cb_data_p->angle > 360.0f)
    {
      cb_data_p->angle -= 360.0f;
    }
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_271_timer, v);
}

void
engine_glut_271_draw (void)
{
  struct Engine_OpenGL_GLUT_271_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_271_CBData*> (glutGetWindowData ());
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
  glColor4f (1.0f, 0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor4f (0.0f, 1.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor4f (0.0f, 0.0f, 1.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  glPushMatrix ();

  glRotatef (cb_data_p->initRot.x + (cb_data_p->count / 300.0f) * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  glRotatef (cb_data_p->initRot.y + (cb_data_p->count / 100.0f) * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);
  glRotatef (cb_data_p->initRot.z + (cb_data_p->count / 200.0f) * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);
  for (int j = 0; j < ENGINE_GLUT_271_DEFAULT_NUMBER_OF_SPHERES; j++)
    for (int i = 0; i < static_cast<int> (cb_data_p->aryTriangle[j].size ()); i++)
    {
      float noiseVal =
        //static_cast<float> (std::pow (std::sin (ENGINE_GLUT_271_DEFAULT_FREQUENCY * 2.0f * static_cast<float> (M_PI) * Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (
        static_cast<float> (std::pow (std::sin (ENGINE_GLUT_271_DEFAULT_FREQUENCY * 2.0f * static_cast<float> (M_PI) * Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.Evaluate (
          cb_data_p->initNoiseXYZ.x + cb_data_p->noiseRangeXYZ.x * cb_data_p->aryTriCenter[j][i].x / cb_data_p->maxAreaR,
          cb_data_p->initNoiseXYZ.y + (cb_data_p->noiseRangeXYZ.y * cb_data_p->aryTriCenter[j][i].y / cb_data_p->maxAreaR) - (ENGINE_GLUT_271_DEFAULT_NOISE_STEP * cb_data_p->count),
          cb_data_p->initNoiseXYZ.z + cb_data_p->noiseRangeXYZ.z * cb_data_p->aryTriCenter[j][i].z / cb_data_p->maxAreaR)), -1.0f, 1.0f, 0.0f, 1.0f)), ENGINE_GLUT_271_DEFAULT_D));

      float ratio = (noiseVal - ENGINE_GLUT_271_DEFAULT_THRESHOLD) / (1.0f - ENGINE_GLUT_271_DEFAULT_THRESHOLD);
      if (noiseVal > ENGINE_GLUT_271_DEFAULT_THRESHOLD)
      {
        glPushMatrix ();
        glTranslatef (cb_data_p->arySphereCenter[j].x, cb_data_p->arySphereCenter[j].y, cb_data_p->arySphereCenter[j].z);
        glRotatef (static_cast<float> (-M_PI_2) * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);
        glRotatef (cb_data_p->aryCentRotYZ[j][i].x * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
        glRotatef (cb_data_p->aryCentRotYZ[j][i].y * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);
        glTranslatef (0.0f, cb_data_p->aryR[j], 0.0f);
        float coneHeight = cb_data_p->sphereR * ratio;
        glTranslatef (0.0f, coneHeight / 2.0f, 0.0f);
        float hue_f = (i % 256) / 255.0f * 360.0f;
        float r, g, b;
        Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
        glColor3f (r, g, b);
        cb_data_p->wireframe ? glutWireCone (coneHeight / ENGINE_GLUT_271_DEFAULT_CONE_RATIO, coneHeight, 40, 40)
                             : glutSolidCone (coneHeight / ENGINE_GLUT_271_DEFAULT_CONE_RATIO, coneHeight, 40, 40);
        glPopMatrix ();
      } // end IF
    } // end FOR
  glPopMatrix ();

  cb_data_p->count++;

  glutSwapBuffers ();
}

void
engine_glut_271_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_271_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_271_idle);
  else
    glutIdleFunc (NULL);
}
