#include "stdafx.h"

#include "glut_417.h"

#include "glm/gtc/matrix_transform.hpp"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_file_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_19.h"

void
engine_glut_417_reshape (int width_in, int height_in)
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
engine_glut_417_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_417_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 400.0f;
      cb_data_p->camera.position.z = 750.0f;

      cb_data_p->camera.rotation.z = 0.0f;

      break;
  } // end SWITCH
}

void
engine_glut_417_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
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
engine_glut_417_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_417_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = ENGINE_GLUT_417_DEFAULT_HEIGHT - y;
}

void
engine_glut_417_timer (int v)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30, engine_glut_417_timer, v);
}

void
engine_glut_417_draw (void)
{
  struct Engine_OpenGL_GLUT_417_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_417_CBData*> (glutGetWindowData ());
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

  // update the control point
  static int frame_count_i = 1;
  float theta =
    Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (frame_count_i / 150.0 + cb_data_p->xOffset,
                                                                         cb_data_p->yOffset,
                                                                         cb_data_p->zOffset)),
                          -1.0f, 1.0f, 0.0f, 2.0f * static_cast<float> (M_PI));
  float phi =
    Common_GL_Tools::map (static_cast<float> (cb_data_p->noise.GetValue (frame_count_i / 150.0 + cb_data_p->yOffset,
                                                                         cb_data_p->xOffset,
                                                                         cb_data_p->zOffset)),
                          -1.0f, 1.0f, 0.0f, 2.0f * static_cast<float> (M_PI));
  cb_data_p->ctrlPoint.position.x = cb_data_p->r * std::sin (phi) * std::cos (theta);
  cb_data_p->ctrlPoint.position.y = cb_data_p->r * std::sin (phi) * std::sin (theta);
  cb_data_p->ctrlPoint.position.z = cb_data_p->r * std::cos (phi);
  std::pair<float, float> latlon =
    cartesianToSpherical (cb_data_p->ctrlPoint.position.x, cb_data_p->ctrlPoint.position.y, cb_data_p->ctrlPoint.position.y, cb_data_p->r);
  cb_data_p->ctrlPoint.latitude = latlon.first;
  cb_data_p->ctrlPoint.longitude = latlon.second;

  for (std::vector<struct point>::iterator iterator = cb_data_p->points.begin ();
       iterator != cb_data_p->points.end ();
       ++iterator)
  {
    glPushMatrix ();
    glTranslatef ((*iterator).position.x, (*iterator).position.y, (*iterator).position.z);
    float d =
      sphericalDistance (cb_data_p->ctrlPoint.latitude, cb_data_p->ctrlPoint.longitude,
                         (*iterator).latitude, (*iterator).longitude,
                         cb_data_p->r);
    //float growPtR = linearDMap (d, cb_data_p->r);
    float growPtR = decayingSine (d, cb_data_p->r);
    cb_data_p->wireframe ? glutWireSphere (growPtR, 16, 16)
                         : glutSolidSphere (growPtR, 16, 16);

    glPopMatrix ();
  } // end FOR

  // display control point
  glColor3f (1.0f, 1.0f, 1.0f);
  glPushMatrix ();
  glTranslatef (cb_data_p->ctrlPoint.position.x, cb_data_p->ctrlPoint.position.y, cb_data_p->ctrlPoint.position.z);
  cb_data_p->wireframe ? glutWireSphere (30.0, 16, 16)
                       : glutSolidSphere (30.0, 16, 16);
  glPopMatrix ();

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_417_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_417_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_417_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

float
linearDMap (float d, float r)
{
  return Common_GL_Tools::map (d, 0.0f, static_cast<float> (M_PI) * r, 0.0f, r / 20.0f);
}

float
decayingSine (float d, float r)
{
  float dAngle =
    Common_GL_Tools::map (d, 0.0f, static_cast<float> (M_PI) * r, 2.0f * static_cast<float> (M_PI), 0.0f);
  return 0.0005f * r *
         ENGINE_GLUT_417_DEFAULT_DECAY_AMPLITUDE * r *
         std::exp (-ENGINE_GLUT_417_DEFAULT_DECAY_RATE * dAngle) *
         std::sin (ENGINE_GLUT_417_DEFAULT_DECAY_FREQUENCY * dAngle + ENGINE_GLUT_417_DEFAULT_DECAY_PHASE_SHIFT);
}

std::pair<float, float>
cartesianToSpherical (float x, float y, float z, float r)
{
  float lat = std::asin (z / r);
  float lon = std::atan2 (y, x);

  return std::make_pair (lat, lon);
}

float
sphericalDistance (float lat1, float lon1, float lat2, float lon2, float r)
{
  float dLat = lat2 - lat1;
  float dLon = lon2 - lon1;

  float a = std::sin (dLat / 2.0f) * std::sin (dLat / 2.0f) +
            std::cos (lat1)        * std::cos (lat2) *
            std::sin (dLon / 2.0f) * std::sin (dLon / 2.0f);

  float c = 2.0f * std::atan2 (std::sqrt (a), std::sqrt (1.0f - a));

  return r * c;
}

std::vector<struct point>
getPointDistribution (int n, float r)
{
  std::vector<struct point> points;
  
  static float phi = (1.0f + std::sqrt (5.0f)) / 2.0f - 1.0f; // Golden ratio
  static float ga = phi * 2.0f * static_cast<float> (M_PI); // Golden angle in radians

  struct point point_s;
  for (int i = 0; i < n; i++)
  {
    point_s.latitude = std::asin (-1.0f + (2.0f * i) / static_cast<float> (n));
    point_s.longitude = ga * i;

    point_s.position.x = r * std::cos (point_s.latitude) * std::cos (point_s.longitude);
    point_s.position.y = r * std::cos (point_s.latitude) * std::sin (point_s.longitude);
    point_s.position.z = r * std::sin (point_s.latitude);

    points.push_back (point_s);
  } // end FOR

  return points;
}
