#include "stdafx.h"

#include "glut_279.h"

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
engine_glut_279_reshape (int width_in, int height_in)
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
engine_glut_279_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_279_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
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
    {
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 750.0f;
      cb_data_p->camera.position.z = 2000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    }
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_279_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
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
engine_glut_279_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_UP)
      {
        //cb_data_p->angle += cb_data_p->deltaAngleX;
        cb_data_p->xOrigin = -1;
      } // end IF
      else
      {
        cb_data_p->xOrigin = x;
      } // end ELSE
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
engine_glut_279_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
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
engine_glut_279_timer (int v)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_279_timer, v);
}

void
engine_glut_279_draw (void)
{
  struct Engine_OpenGL_GLUT_279_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_279_CBData*> (glutGetWindowData ());
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

  glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

  float halfSponge = cb_data_p->spongeSize / 2.0f;
  glTranslatef (-halfSponge, -halfSponge, -halfSponge);
  //draw all the cubes to create the sponge
  for (std::vector<glm::vec3>::iterator iterator = cb_data_p->points.begin ();
       iterator != cb_data_p->points.end ();
       ++iterator)
  {
    glPushMatrix ();
    glTranslatef ((*iterator).x + 20, (*iterator).y + 20, (*iterator).z);
    float hue =
      Common_GL_Tools::map ((*iterator).x, 0.0f, static_cast<float> (cb_data_p->spongeSize), 0.0f, 360.0f);
    float saturation =
      Common_GL_Tools::map ((*iterator).y, 0.0f, static_cast<float> (cb_data_p->spongeSize), 0.2f, 1.0f);
    float brightness =
      Common_GL_Tools::map ((*iterator).z, 0.0f, static_cast<float> (cb_data_p->spongeSize), 0.2f, 1.0f);
    float r, g, b;
    Common_Image_Tools::HSVToRGB (hue, saturation, brightness, r, g, b);
    glColor4f (r, g, b, 0.5f);

    glScalef (static_cast<float> (ENGINE_GLUT_279_DEFAULT_CUBE_SIZE), static_cast<float> (ENGINE_GLUT_279_DEFAULT_CUBE_SIZE), static_cast<float> (ENGINE_GLUT_279_DEFAULT_CUBE_SIZE));

    glBegin (GL_QUADS);
    // Top face (y = 0.5f)
    // Define vertices in counter-clockwise (CCW) order with normal pointing out
    glVertex3f ( 0.5f, 0.5f, -0.5f);
    glVertex3f (-0.5f, 0.5f, -0.5f);
    glVertex3f (-0.5f, 0.5f,  0.5f);
    glVertex3f ( 0.5f, 0.5f,  0.5f);

    // Bottom face (y = -0.5f)
    glVertex3f ( 0.5f, -0.5f,  0.5f);
    glVertex3f (-0.5f, -0.5f,  0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f ( 0.5f, -0.5f, -0.5f);

    // Front face  (z = 0.5f)
    glVertex3f ( 0.5f,  0.5f, 0.5f);
    glVertex3f (-0.5f,  0.5f, 0.5f);
    glVertex3f (-0.5f, -0.5f, 0.5f);
    glVertex3f ( 0.5f, -0.5f, 0.5f);

    // Back face (z = -0.5f)
    glVertex3f ( 0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f,  0.5f, -0.5f);
    glVertex3f ( 0.5f,  0.5f, -0.5f);

    // Left face (x = -0.5f)
    glVertex3f (-0.5f,  0.5f,  0.5f);
    glVertex3f (-0.5f,  0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f, -0.5f);
    glVertex3f (-0.5f, -0.5f,  0.5f);

    // Right face (x = 0.5f)
    glVertex3f (0.5f,  0.5f, -0.5f);
    glVertex3f (0.5f,  0.5f,  0.5f);
    glVertex3f (0.5f, -0.5f,  0.5f);
    glVertex3f (0.5f, -0.5f, -0.5f);
    glEnd ();

    glPopMatrix ();
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_279_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_279_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_279_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
createSponge (int level,
              int xOffset, int yOffset, int zOffset,
              struct Engine_OpenGL_GLUT_279_CBData& CBData)
{
  if (level == 0)
  {
    glm::vec3 vec (xOffset, yOffset, zOffset);
    CBData.points.push_back (vec);
    return;
  } // end IF

  //calculate the size of one cube that makes up the current level
  int levelOffset = static_cast<int> (std::pow (3, level - 1)) * ENGINE_GLUT_279_DEFAULT_CUBE_SIZE;
  int x, z, y;
  for (int i = 0; i < ENGINE_GLUT_279_DEFAULT_CUBES_PER_UNIT; i++)
  {
    //check if we are calculating the current cube
    static std::vector<int> numbers_to_ignore_a = {4, 10, 12, 13, 14, 16, 22};
    std::vector<int>::iterator iterator =
      std::find (numbers_to_ignore_a.begin (), numbers_to_ignore_a.end (), i);
    if (iterator == numbers_to_ignore_a.end ())
    {
      x = i % 3;
      y = static_cast<int> (std::floor (i / 9.0f));
      z = static_cast<int> (std::floor (i / 3.0f)) % 3;
      createSponge (level - 1, xOffset + (levelOffset * x), yOffset + (levelOffset * y), zOffset + (levelOffset * z), CBData);
    } // end IF
  } // end FOR
}
