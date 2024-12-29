#include "stdafx.h"

#include "glut_264.h"

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

#include "defines_12.h"
#include "engine_common.h"

void
engine_glut_264_reshape (int width_in, int height_in)
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
engine_glut_264_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_264_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 500.0f;
      cb_data_p->camera.position.z = 1000.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_264_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
engine_glut_264_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
engine_glut_264_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
engine_glut_264_timer (int v)
{
  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_264_timer, v);
}

void
engine_glut_264_draw (void)
{
  static int frame_count_i = 1;

  struct Engine_OpenGL_GLUT_264_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_264_CBData*> (glutGetWindowData ());
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
  glRotatef ((static_cast<float> (-M_PI) / 3.0f) * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  glScalef (0.5f, 0.5f, 0.5f);
  static int hf = 50;
  for (int ribbon = 5; ribbon > 0; --ribbon)
  {
    std::vector<glm::vec3> verts =
      createVertLoop (cb_data_p->time, cb_data_p->time - 5.8f, ENGINE_GLUT_264_DEFAULT_WIDTH, 35 * (ribbon + 2), 100, 20);

    static Common_GL_Color_t colors_a[5] =
      {Common_GL_Color_t (0x9b, 0x5d, 0xe5, 0xff), Common_GL_Color_t (0xf1, 0x5b, 0xb5, 0xff), Common_GL_Color_t (0xfe, 0xe4, 0x40, 0xff),
       Common_GL_Color_t (0x00, 0xbb, 0xf9, 0xff), Common_GL_Color_t (0x00, 0xf5, 0xd4, 0xff)};
    Common_GL_Color_t color = colors_a[ribbon % 5];
    glBegin (GL_POLYGON);
    glColor4f (color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, 1.0f);
    for (int i = 0; i < static_cast<int> (verts.size ()); i++)
      glVertex3f (verts[i].x, std::sin (frame_count_i * 0.01f) * (ribbon + 1) * -hf, verts[i].y);
    glEnd ();
  } // end FOR
  glPopMatrix ();

  cb_data_p->time += ENGINE_GLUT_264_DEFAULT_TIME_SCALE;

  ++frame_count_i;

  glutSwapBuffers ();
}

void
engine_glut_264_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_264_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_264_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

glm::vec3
figureEight (float time, int halfWidth)
{
  float denom = 1.0f + (std::sin (time) * std::sin (time));

  float x = halfWidth * std::cos (time) / denom;
  float y = halfWidth * std::sin (time) * std::cos (time) / denom;

  return glm::vec3 (x, y, 0.0f);
}

glm::vec3
rotatePoint (float cx, float cy, float angle, float x1, float y1)
{
  float s = std::sin (angle);
  float c = std::cos (angle);

  // translate point back to origin:
  x1 -= cx;
  y1 -= cy;

  // rotate point
  float xnew = x1 * c - y1 * s;
  float ynew = x1 * s + y1 * c;

  // translate point back:
  x1 = xnew + cx;
  y1 = ynew + cy;

  return glm::vec3 (x1, y1, 0.0f);
}

std::vector<glm::vec3>
createCapVerts (glm::vec3& p0, glm::vec3& p1, int numCapPoints)
{
  std::vector<glm::vec3> capVerts;
  
  float mx = (p0.x + p1.x) / 2.0f;
  float my = (p0.y + p1.y) / 2.0f;

  float capIncrement = static_cast<float> (M_PI) / static_cast<float> (numCapPoints);
  for (int i = 0; i < numCapPoints + 1; i++)
  {
    glm::vec3 vert = rotatePoint (mx, my, capIncrement * i, p0.x, p0.y);
    capVerts.push_back (vert);
  } // end FOR

  return capVerts;
}

std::vector<glm::vec3>
createVertLoop (float currentTime, float previousTime, int halfWidth, int halfloopWidth, int numEdgePoints, int numCapPoints)
{
  glm::vec3 up (0.0f, 0.0f, 1.0f);
  std::vector<glm::vec3> verts;
  int vertCount = 0;
  float timeDiff = currentTime - previousTime;
  float timeInc = timeDiff / static_cast<float> (numEdgePoints);

  //Top edge
  std::vector<glm::vec3> TopEdgeVerts;
  std::vector<glm::vec3> BottomEdgeVerts;
  for (int i = 0; i < numEdgePoints; i++)
  {
    TopEdgeVerts.push_back (glm::vec3 (0.0f, 0.0f, 0.0f));
    BottomEdgeVerts.push_back (glm::vec3 (0.0f, 0.0f, 0.0f));

    glm::vec3 t1 = figureEight (currentTime, halfWidth);
    glm::vec3 t0 = figureEight (currentTime - 0.001f, halfWidth);

    glm::vec3 direction = t1 - t0;
    direction = glm::normalize (direction);

    glm::vec3 side = glm::cross (direction, up);
    side *= halfloopWidth;

    glm::vec3 topEdge = t0 + side;
    glm::vec3 botEdge = t0 - side;

    TopEdgeVerts[vertCount].x = topEdge.x;
    TopEdgeVerts[vertCount].y = topEdge.y;

    BottomEdgeVerts[vertCount].x = botEdge.x;
    BottomEdgeVerts[vertCount].y = botEdge.y;

    currentTime += timeInc;
    vertCount++;
  } // end FOR

  std::reverse (BottomEdgeVerts.begin (), BottomEdgeVerts.end ());

  glm::vec3 end0 = TopEdgeVerts[numEdgePoints - 1];
  glm::vec3 end1 = BottomEdgeVerts[0];
  std::vector<glm::vec3> TopCapVerts = createCapVerts (end0, end1, numCapPoints);

  glm::vec3 start0 = BottomEdgeVerts[numEdgePoints - 1];
  glm::vec3 start1 = TopEdgeVerts[0];
  std::vector<glm::vec3> BottomCapVerts = createCapVerts (start0, start1, numCapPoints);


  verts.insert (verts.end (), TopEdgeVerts.begin (), TopEdgeVerts.end ());
  verts.insert (verts.end (), TopCapVerts.begin (), TopCapVerts.end ());
  verts.insert (verts.end (), BottomEdgeVerts.begin (), BottomEdgeVerts.end ());
  verts.insert (verts.end (), BottomCapVerts.begin (), BottomCapVerts.end ());

  return verts;
}
