#include "stdafx.h"

#include "glut_449.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

void
engine_glut_449_reshape (int width_in, int height_in)
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
engine_glut_449_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_449_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 1500.0f;

      cb_data_p->camera.rotation.y = 0.0f;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_449_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
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
engine_glut_449_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_449_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = y;
}

void
engine_glut_449_timer (int v)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_449_timer, v);
}

void
engine_glut_449_draw (void)
{
  struct Engine_OpenGL_GLUT_449_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_449_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

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

  //glPolygonMode (GL_FRONT_AND_BACK,
  //               cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  static int frame_count_i = 1;

  glTranslatef (ENGINE_GLUT_449_DEFAULT_WIDTH / 128.0f, frame_count_i / 3.0f, 0.0f);
  glScalef (-1.0f, -0.5f, 1.0f);
  glRotatef (static_cast<float> (M_PI_4) * (180.0f / static_cast<float> (M_PI)), 0.0f, 0.0f, 1.0f);

  float vmax = 0.0f;
  float vmin = 1.0f;
  for (int stage = 0; stage <= 1; stage++)
    for (int y = 0; y < 90; y++)
      for (int x = 0; x < 27; x++)
      {
        float cx = (x + (y % 2 == 1 ? 0.0f : 0.5f)) * ENGINE_GLUT_449_DEFAULT_HS * 3.0f;
        float cy = y * ENGINE_GLUT_449_DEFAULT_HS * std::sqrt (3.0f) / 2.0f;
        float v =
          Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (cx * ENGINE_GLUT_449_DEFAULT_NF, cy * ENGINE_GLUT_449_DEFAULT_NF, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
        if (stage == 0)
        {
          vmax = std::max (v, vmax);
          vmin = std::min (v, vmin);
        }  // end IF
        else
        {
          v = Common_GL_Tools::map (v, vmin, vmax, -0.1f, 1.0f);
          Common_GL_Color_t color = grad (v, cb_data_p->gradient);
          glColor4ub (color.r, color.g, color.b, color.a);
          if ((v * v) * (v / std::abs (v)) >= frame_count_i / 512.0f)
            polygon (cx, cy, static_cast<float> (ENGINE_GLUT_449_DEFAULT_HS), 6);
        } // end ELSE
      } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_449_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_449_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_449_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

Common_GL_Color_t
grad (float v, const std::vector<std::vector<float> >& gradient)
{
  Common_GL_Color_t result;

  if (v < 0.0f || v > 1.0f)
  {
    result.r = 0; result.g = 0; result.b = 0; result.a = 0;
    return result;
  } // end IF

  for (int i = 0; i < static_cast<int> (gradient.size ()) - 1; i++)
    if (v >= gradient[i][0] && v <= gradient[i + 1][0])
    {
      glm::vec4 a (gradient[i][1], gradient[i][2], gradient[i][3], gradient[i][4]);
      glm::vec4 b (gradient[i + 1][1], gradient[i + 1][2], gradient[i + 1][3], gradient[i + 1][4]);
      glm::vec4 c = glm::mix (a, b, Common_GL_Tools::map (v, gradient[i][0], gradient[i + 1][0], 0.0f, 1.0f));

      result.r = static_cast<uint8_t> (c.r * 255.0f);
      result.g = static_cast<uint8_t> (c.g * 255.0f);
      result.b = static_cast<uint8_t> (c.b * 255.0f);
      result.a = static_cast<uint8_t> (c.a * 255.0f);
      return result;
    } // end IF

  result.r = 0; result.g = 0; result.b = 0; result.a = 255U;
  return result;
}

void
polygon (float x, float y, float radius, int npoints)
{
  float angle = (2.0f * static_cast<float> (M_PI)) / npoints;

  glBegin (GL_POLYGON);
  for (float a = 0.0f; a < 2.0f * static_cast<float> (M_PI); a += angle)
  {
    float sx = x + std::cos (a) * radius;
    float sy = y + std::sin (a) * radius;
    glVertex3f (sx, sy, 0.0f);
  } // end FOR
  glEnd ();

  // *NOTE*: disable this for non-stroke (looks more organic)
  glColor3ub (0, 0, 0);
  glBegin (GL_LINE_STRIP);
  for (float a = 0.0f; a < 2.0f * static_cast<float> (M_PI); a += angle)
  {
    float sx = x + std::cos (a) * radius;
    float sy = y + std::sin (a) * radius;
    glVertex3f (sx, sy, 0.0f);
  } // end FOR
  glEnd ();
}
