#include "stdafx.h"

#include "glut_284.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

#include "defines_13.h"
#include "engine_common.h"

void
engine_glut_284_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  //ACE_ASSERT (height_in);
  //gluPerspective (45.0,
  //                width_in / static_cast<GLdouble> (height_in),
  //                1.0, -1.0);
  glOrtho (-width_in / 2.0, width_in / 2.0, -height_in / 2.0, height_in / 2.0, 0, width_in * 2.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_284_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_284_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_F1:
      break;
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
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    }
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_284_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
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
engine_glut_284_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
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
engine_glut_284_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
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
engine_glut_284_timer (int v)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_284_timer, v);
}

void
engine_glut_284_draw (void)
{
  struct Engine_OpenGL_GLUT_284_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_284_CBData*> (glutGetWindowData ());
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

  //glColor4f (1.0f, 1.0f, 1.0f, 1.0f);

  //directionalLight(0, 0, 150, -1, 1, -1);
  //ambientLight(60);
  //GLfloat light_position_a[] = {-1.0f, 1.0f, -1.0f};
  //GLfloat light_position_2[] = {-1.0f, 1.0f, -1.0f};
  //glLightfv (GL_LIGHT0, GL_POSITION, light_position_a);
  //glLightfv (GL_LIGHT1, GL_POSITION, light_position_2);
  //static GLint light_ambient_a[] = { 60, 60, 60, 255 };
  //static GLint light_diffuse_a[] = { 60, 60, 60, 255 };
  //static GLint light_specular_a[] = { 250, 250, 250, 255 };
  //glLightiv (GL_LIGHT0, GL_AMBIENT, light_ambient_a);
  //glLightiv (GL_LIGHT0, GL_DIFFUSE, light_diffuse_a);
  //glLightiv (GL_LIGHT0, GL_SPECULAR, light_specular_a);
  //static GLint light_ambient_2[] = { 0, 0, 150, 255 };
  //static GLint light_diffuse_2[] = { 0, 0, 150, 255 };
  //static GLint light_specular_2[] = { 250, 250, 250, 255 };
  //glLightiv (GL_LIGHT1, GL_AMBIENT, light_ambient_2);
  //glLightiv (GL_LIGHT1, GL_DIFFUSE, light_diffuse_2);
  //glLightiv (GL_LIGHT1, GL_SPECULAR, light_specular_2);

  //static GLfloat material_color_a[] = {1.0f, 1.0f, 0.0f, 1.0f};
  //static GLfloat material_specular_a[] = {0.0f, 0.0f, 1.0f, 1.0f};
  //static GLfloat material_emission_a[] = {0.0f, 0.0f, 0.0f, 1.0f};
  //static GLfloat material_shininess_f = 50.0f;
  //glMaterialfv (GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, material_color_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular_a);
  //glMaterialfv (GL_FRONT_AND_BACK, GL_EMISSION, material_emission_a);
  //glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, material_shininess_f);

  glTranslatef (0.0f, cb_data_p->minW / 20.0f, 0.0f);
  glRotatef ((180.0f / static_cast<float> (M_PI)) * (static_cast<float> (M_PI) / 5.0f), 1.0f, 0.0f, 0.0f);

  for (int i = 0; i < static_cast<int> (cb_data_p->rings.size ()); i++)
    cb_data_p->rings[i].draw (cb_data_p->wireframe);

  glutSwapBuffers ();
}

void
engine_glut_284_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_284_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_284_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
setObject (struct Engine_OpenGL_GLUT_284_CBData& CBData)
{
  static float wRatio = 0.9f;//2**0.5;//0.98;
  CBData.minW = std::min (ENGINE_GLUT_284_DEFAULT_WIDTH, ENGINE_GLUT_284_DEFAULT_HEIGHT) * wRatio;
  //background(0);//10);
  //translate(width / 2, height / 2);
  //noStroke();
  //ellipseMode(RADIUS);

  static int numTileX = 1;
  static int numTileY = 1;
  float step = CBData.minW / static_cast<float> (std::max (numTileX, numTileY));
  CBData.rings.clear ();

  static Common_GL_Color_t palette_a[6] =
    { Common_GL_Color_t (0xaf, 0x3e, 0x4d), Common_GL_Color_t (0x2e, 0x86, 0xab), Common_GL_Color_t (0x75, 0x8e, 0x4f),
      Common_GL_Color_t (0x00, 0x2A, 0x32), Common_GL_Color_t (0xf6, 0xae, 0x2d), Common_GL_Color_t (0xFA, 0xC9, 0xB8) };

  for (int xi = 0; xi < numTileX; xi++)
    for (int yi = 0; yi < numTileY; yi++)
    {
      glm::vec2 xyOut (-(step * static_cast<float> (numTileX)) / 2.0f + step * (static_cast<float> (xi) + 0.5f),
                       -(step * static_cast<float> (numTileY)) / 2.0f + step * (static_cast<float> (yi) + 0.5f));
      float rOut = step / 2.0f;
      int col_index = Common_Tools::getRandomNumber (0, 5);
      Common_GL_Color_t colParent = palette_a[col_index];
      CBData.rings.push_back (ring_parent (CBData.minW, xyOut, rOut, 1, colParent));
    } // end FOR
}
