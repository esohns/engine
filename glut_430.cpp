#include "stdafx.h"

#include "glut_430.h"

#if defined (IMAGEMAGICK_SUPPORT)
#if defined (IMAGEMAGICK_IS_GRAPHICSMAGICK)
#include "wand/wand_api.h"
#else
#include "MagickWand/MagickWand.h"
#endif // IMAGEMAGICK_IS_GRAPHICSMAGICK
#endif // IMAGEMAGICK_SUPPORT

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

void
engine_glut_430_reshape (int width_in, int height_in)
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
engine_glut_430_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_430_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
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
engine_glut_430_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
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
engine_glut_430_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_430_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = ENGINE_GLUT_430_DEFAULT_HEIGHT - y;
}

void
engine_glut_430_timer (int v)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_430_timer, v);
}

void
engine_glut_430_draw (void)
{
  struct Engine_OpenGL_GLUT_430_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_430_CBData*> (glutGetWindowData ());
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

  glRotatef ((static_cast<float> (-M_PI) / 8.0f) * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  moveFish();
  drawBubbles();
  drawFish (*cb_data_p);
  drawBowl();
  checkInputs();
  makeBubbles();

  glutSwapBuffers ();
}

void
engine_glut_430_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_430_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_430_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
makeScales (struct Engine_OpenGL_GLUT_430_CBData& CBData_inout)
{
  Common_Image_Resolution_t resolution_s;
  uint8_t* data_p = NULL;
  if (!Common_Image_Tools::load (ACE_TEXT_ALWAYS_CHAR ("tetra2.png"),
                                 ACE_TEXT_ALWAYS_CHAR ("RGBA"),
                                 resolution_s,
                                 data_p))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to load \"tetra2.png\", returning\n")));
    return;
  } // end IF
  ACE_ASSERT (data_p);

  Common_Image_Resolution_t resolution_2 = { ENGINE_GLUT_430_DEFAULT_DETAIL, -1 };
  uint8_t* data_2 = NULL;
  if (!Common_Image_Tools::scale (resolution_s,
                                  ACE_TEXT_ALWAYS_CHAR ("RGBA"),
                                  data_p,
                                  resolution_2,
                                  data_2))
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to scale \"tetra2.png\", returning\n")));
    MagickRelinquishMemory (data_p);
    return;
  } // end IF
  ACE_ASSERT (data_2);
  MagickRelinquishMemory (data_p); data_p = NULL;

  Common_GL_Color_t color;
  for (int y = 0; y < resolution_2.cy; y += 15)
    for (int x = 0; x < resolution_2.cx; x += 15)
    {
      data_p = data_2 + (y * resolution_2.cx + x) * 4;
      color.r = *(data_p + 0);
      color.g = *(data_p + 1);
      color.b = *(data_p + 2);

      if (Common_GL_Tools::brightness (color) > 0.2f)
      {
        float ax =
          Common_GL_Tools::map ((y - resolution_2.cy / 2.0f), -resolution_2.cy / 8.0f, resolution_2.cy / 3.0f, static_cast<float> (-M_PI_2), static_cast<float> (M_PI_2));
        float ay =
          Common_GL_Tools::map ((x - resolution_2.cx / 2.0f), -resolution_2.cx / 2.0f, resolution_2.cx / 3.0f, static_cast<float> (-M_PI_2), static_cast<float> (M_PI_2));
        float tz = std::cos (ax) * (resolution_2.cy / 6.0f) * std::cos (ay);
        float z = std::max (-1.0f, tz);
        if (y < resolution_2.cy / 4.0f || y > 4.0f * resolution_2.cy / 5.0f)
          ax = 0.0f;
        if (x > 5.0f * resolution_2.cx / 6.0f)
          ay = 0.0f;
        glm::vec3 ang (ax, ay, 0.0f);
        glm::vec3 pos (x - resolution_2.cx / 2.0f, y - resolution_2.cy / 2.0f, z);
        CBData_inout.scales.push_back (scale (pos, ang, color));
      } // end IF
    } // end FOR

  delete [] data_2; data_2 = NULL;
}

void
drawFish (struct Engine_OpenGL_GLUT_430_CBData& CBData_in)
{
  glPushMatrix ();
  static float scale_f = 0.15f * ENGINE_GLUT_430_DEFAULT_HEIGHT / 566.0f;
  glScalef (scale_f, scale_f, scale_f);
  glTranslatef (CBData_in.tpos.x, CBData_in.tpos.y, CBData_in.tpos.z);
  glRotatef (CBData_in.orient.y * 180.0f / static_cast<float> (M_PI), 0.0f, 1.0f, 0.0f);
  glRotatef (CBData_in.orient.z * 180.0f / static_cast<float> (M_PI), 0.0f, 0.0f, 1.0f);
  for (std::vector<scale>::iterator iterator = CBData_in.scales.begin ();
       iterator != CBData_in.scales.end ();
       ++iterator)
    (*iterator).draw (CBData_in.twist, CBData_in.wiggle);
  glPopMatrix ();
}
