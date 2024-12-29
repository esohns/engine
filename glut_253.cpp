#include "stdafx.h"

#include "glut_253.h"

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

#include "defines_11.h"
#include "engine_common.h"

void
engine_glut_253_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  1.0, -1.0);

  //glOrtho (width_in / 2.0, -width_in / 2.0, -height_in / 2.0, height_in / 2.0, 3000.0, -3000.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_253_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_253_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.y = 1000.0f;
      cb_data_p->camera.position.z = 3000.0f;

      cb_data_p->camera.rotation.z = 0.0f;
      break;
    case GLUT_KEY_DOWN:
      break;
  } // end SWITCH
}

void
engine_glut_253_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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
engine_glut_253_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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
engine_glut_253_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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
engine_glut_253_timer (int v)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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
  glutTimerFunc (1000 / 30, engine_glut_253_timer, v);
}

void
engine_glut_253_draw (void)
{
  struct Engine_OpenGL_GLUT_253_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_253_CBData*> (glutGetWindowData ());
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

  if (cb_data_p->flip)
  {
    cb_data_p->a0 += 0.025f;
    if (cb_data_p->a0 > 5.1f)
      cb_data_p->flip = !cb_data_p->flip;
  } // end IF
  else
  {
    cb_data_p->a0 -= 0.025f;
    if (cb_data_p->a0 < -5.1f)
    {
      cb_data_p->flip = !cb_data_p->flip;
      createVoxWorld (*cb_data_p);
      irregularSubDivide (*cb_data_p);
    } // end IF
  } // end ELSE

  static glm::i32vec3 d (ENGINE_GLUT_253_DEFAULT_DIMENSION_X, ENGINE_GLUT_253_DEFAULT_DIMENSION_Y, ENGINE_GLUT_253_DEFAULT_DIMENSION_Z);
  static glm::i32vec3 vsize = d * ENGINE_GLUT_253_DEFAULT_VOX_WIDTH;
  glPushMatrix ();
  glTranslatef (-vsize.x / 2.0f, -vsize.y / 2.0f, vsize.z / 2.0f);
  glTranslatef (0.0f, vsize.y * 0.3f, 0.0f);

  for (int x = 0; x < ENGINE_GLUT_253_DEFAULT_DIMENSION_X; x++)
    for (int y = 0; y < ENGINE_GLUT_253_DEFAULT_DIMENSION_Y; y++)
      for (int z = 0; z < ENGINE_GLUT_253_DEFAULT_DIMENSION_Z; z++)
      {
        float t =
          std::min (std::max (cb_data_p->a0 + ((1.0f - (y / static_cast<float> (ENGINE_GLUT_253_DEFAULT_DIMENSION_Y))) * 5.0f), 0.0f), 1.0f);
        float at = easeInOutQuint (t);
        float offset = -at * ENGINE_GLUT_253_DEFAULT_VOX_WIDTH * ENGINE_GLUT_253_DEFAULT_DIMENSION_X * 0.25f;

        glPushMatrix ();
        glTranslatef (offset, offset, 0.0f);
        glRotatef (-static_cast<float> (M_PI_4) * 0.5f * at * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
        glRotatef ( static_cast<float> (M_PI_4)        * at * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);

        drawVox (x, y, z, at * 1.5f, *cb_data_p);
        glPopMatrix ();
      } // end FOR
  glPopMatrix ();

  glutSwapBuffers ();
}

void
engine_glut_253_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_253_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_253_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

int
index (int x, int y, int z)
{
  return ENGINE_GLUT_253_DEFAULT_DIMENSION_Y * ENGINE_GLUT_253_DEFAULT_DIMENSION_X * z + ENGINE_GLUT_253_DEFAULT_DIMENSION_X * y + x;
}

int
maxIndex ()
{
  return ENGINE_GLUT_253_DEFAULT_DIMENSION_X * ENGINE_GLUT_253_DEFAULT_DIMENSION_Y * ENGINE_GLUT_253_DEFAULT_DIMENSION_Z;
}

bool
validIndex (int x, int y, int z)
{
  return x >= 0 && y >= 0 && z >= 0 && x < ENGINE_GLUT_253_DEFAULT_DIMENSION_X && y < ENGINE_GLUT_253_DEFAULT_DIMENSION_Y && z < ENGINE_GLUT_253_DEFAULT_DIMENSION_Z;
}

float
easeInOutQuint (float x)
{
  return x < 0.5f ? 16.0f * x * x * x * x * x
                  : 1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 5)) / 2.0f;
}

void
createVoxWorld (struct Engine_OpenGL_GLUT_253_CBData& CBData_in)
{
  for (int x = 0; x < ENGINE_GLUT_253_DEFAULT_DIMENSION_X; x++)
    for (int y = 0; y < ENGINE_GLUT_253_DEFAULT_DIMENSION_Y; y++)
      for (int z = 0; z < ENGINE_GLUT_253_DEFAULT_DIMENSION_Z; z++)
        CBData_in.voxGrid[index(x, y, z)] = vox (glm::i32vec3 (x, y, z));
}

void
drawFaceBox (int boxWidth, int boxHeight, int boxDepth, Common_GL_Color_t& top, Common_GL_Color_t& right, Common_GL_Color_t& front)
{
  glPushMatrix ();
  glColor3ub (top.r, top.g, top.b);
  glRotatef (-static_cast<float> (M_PI) * 0.5f * (180.0f / static_cast<float> (M_PI)), 1.0f, 0.0f, 0.0f);
  glTranslatef (0.0f, 0.0f, -boxHeight * 0.5f);
  //plane(boxWidth, boxDepth);
  glBegin (GL_QUADS);
  glVertex3f (0.0f, 0.0f, 0.0f);
  glVertex3f (0.0f, static_cast<float> (boxDepth), 0.0f);
  glVertex3f (static_cast<float> (boxWidth), static_cast<float> (boxDepth), 0.0f);
  glVertex3f (static_cast<float> (boxWidth), 0.0f, 0.0f);
  glEnd ();
  glPopMatrix();

  glPushMatrix ();
  glColor3ub (right.r, right.g, right.b);
  glRotatef (static_cast<float> (M_PI) * 0.5f * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);
  glTranslatef (0.0f, 0.0f, -boxWidth * 0.5f);
  //plane (boxDepth, boxHeight);
  glBegin (GL_QUADS);
  glVertex3f (0.0f, 0.0f, 0.0f);
  glVertex3f (0.0f, static_cast<float> (boxHeight), 0.0f);
  glVertex3f (static_cast<float> (boxDepth), static_cast<float> (boxHeight), 0.0f);
  glVertex3f (static_cast<float> (boxDepth), 0.0f, 0.0f);
  glEnd ();
  glPopMatrix ();

  glPushMatrix ();
  glColor3ub (front.r, front.g, front.b);
  glRotatef (static_cast<float> (M_PI) * (180.0f / static_cast<float> (M_PI)), 0.0f, 1.0f, 0.0f);
  glTranslatef (0.0f, 0.0f, -boxDepth * 0.5f);
  //plane (boxWidth, boxHeight);
  glBegin (GL_QUADS);
  glVertex3f (0.0f, 0.0f, 0.0f);
  glVertex3f (0.0f, static_cast<float> (boxHeight), 0.0f);
  glVertex3f (static_cast<float> (boxWidth), static_cast<float> (boxHeight), 0.0f);
  glVertex3f (static_cast<float> (boxWidth), 0.0f, 0.0f);
  glEnd ();
  glPopMatrix ();
}

void
drawVox (int x, int y, int z, float explode, struct Engine_OpenGL_GLUT_253_CBData& CBData_in)
{
  int i = index (x, y, z);
  if (i >= maxIndex ())
    return;
  vox v = CBData_in.voxGrid[i];
  //only draw masters
  if (v.slave)
    return;

  int w = ENGINE_GLUT_253_DEFAULT_VOX_WIDTH;
  glm::i32vec3 d (ENGINE_GLUT_253_DEFAULT_DIMENSION_X, ENGINE_GLUT_253_DEFAULT_DIMENSION_Y, ENGINE_GLUT_253_DEFAULT_DIMENSION_Z);

  //normalize in the range -half dim to dim
  glm::i32vec3 temp = d; temp *= 0.5f;
  glm::i32vec3 normalV = v.position - temp;
  explode *= w;

  glPushMatrix ();
  glTranslatef (normalV.x * explode, normalV.y * explode, normalV.z * explode);
  glTranslatef (static_cast<float> (v.dimension.x) * 0.5f * explode, static_cast<float> (v.dimension.y) * 0.5f * explode, static_cast<float> (v.dimension.z) * 0.5f * explode);
  glTranslatef (static_cast<float> (v.position.x) * w, static_cast<float> (v.position.y) * w, static_cast<float> (v.position.z) * w);
  glTranslatef (w * static_cast<float> (v.dimension.x) * 0.5f, w * static_cast<float> (v.dimension.y) * 0.5f, w * static_cast<float> (v.dimension.z) * 0.5f);

  static Common_GL_Color_t internalColors[4] =
    {Common_GL_Color_t (0x23, 0x93, 0xd9, 0xff), Common_GL_Color_t (0x3a, 0xc4, 0x35, 0xff), Common_GL_Color_t (0xed, 0x53, 0x53, 0xff), Common_GL_Color_t (0xd9, 0xd9, 0x3d, 0xff)};
  static Common_GL_Color_t internalColorsDrk[4] =
    {Common_GL_Color_t (0x1b, 0x70, 0xa6, 0xff), Common_GL_Color_t (0x33, 0xa6, 0x2d, 0xff), Common_GL_Color_t (0xa6, 0x3a, 0x3a, 0xff), Common_GL_Color_t (0xa6, 0xa6, 0x2e, 0xff)};

  Common_GL_Color_t rightface = x > 0 ? internalColorsDrk[(i + 1) % 4] : Common_GL_Color_t (0x00, 0xDD, 0x00, 0xff);
  Common_GL_Color_t topface   = y > 0 ? internalColors[(i + 2) % 4] : Common_GL_Color_t (0xEE, 0x99, 0x00, 0xff);
  Common_GL_Color_t frontColour = internalColors[(i + 3) % 4];
  bool frontface = z + v.dimension.z >= d.z;
  if (frontface)
  {
    if (x < d.x * 0.5f && y < d.y * 0.5f)
      frontColour = Common_GL_Color_t (0xFF, 0x22, 0x22, 0xff); // top right
    if (x < d.x * 0.5f && y >= d.y * 0.5f)
      frontColour = Common_GL_Color_t (0x44, 0x44, 0xFF, 0xff); // bottom right
    if (x >= d.y * 0.5f && y >= d.y * 0.5f)
      frontColour = Common_GL_Color_t (0xFF, 0x22, 0x22, 0xff); // bottom left
    if (x >= d.y * 0.5f && y < d.y * 0.5f)
      frontColour = Common_GL_Color_t (0x44, 0x44, 0xFF, 0xff); // top left
  } // end IF

  drawFaceBox (w * v.dimension.x, w * v.dimension.y, w * v.dimension.z,
               topface, rightface, frontColour);

  /*
  //Debug the faces
  frontface = z + v.dimension.z < d.z  ? "#575366" : "#0000FF";
  if(x ==0 || y == 0 || z ==0 || (x + v.dimension.x) >= d.x || (y + v.dimension.y >= d.y) || (z + v.dimension.z >= d.z) )
    drawFaceBox(w*v.dimension.x,w*v.dimension.y,w*v.dimension.z,"#E3DFFF","#222222","#575366");
  else
    drawFaceBox(w*v.dimension.x,w*v.dimension.y,w*v.dimension.z,"#FF0000","#00FF00","#0000FF");
  */

  glPopMatrix ();
}

bool
areAllFree (glm::i32vec3 pos, glm::i32vec3 dim, struct Engine_OpenGL_GLUT_253_CBData& CBData_in)
{
  glm::vec3 vsize (ENGINE_GLUT_253_DEFAULT_DIMENSION_X, ENGINE_GLUT_253_DEFAULT_DIMENSION_Y, ENGINE_GLUT_253_DEFAULT_DIMENSION_Z);
  glm::vec3 hsize = vsize * 0.5f;

  if (pos.z + dim.z >= vsize.z)
  {
    int x = dim.x;
    int y = dim.y;

    if (pos.x + dim.x > hsize.x)
      x = std::max (1, static_cast<int> ((pos.x + dim.x - 1) - hsize.x));
    if (pos.y + dim.y > hsize.y)
      y = std::max (1, static_cast<int> ((pos.y + dim.y - 1) - hsize.y));

    dim.z = 1;
    dim.x = std::min (x, y);
    dim.y = std::min (x, y);
  } // end IF

  for (int ax = 0; ax < dim.x; ax++)
    for (int by = 0; by < dim.y; by++)
      for (int cz = 0; cz < dim.z; cz++)
      {
        int tx = pos.x + ax;
        int ty = pos.y + by;
        int tz = pos.z + cz;

        if (!validIndex (tx, ty, tz)) //|| tz >= vsize.z-1)
          return false;

        int i = index (tx, ty, tz);

        //check that the dimensions aren't out of scope
        if (!CBData_in.voxGrid[i].free)
          return 0;
      }

  return true;
}

void
claim (glm::i32vec3 pos, glm::i32vec3 dim, struct Engine_OpenGL_GLUT_253_CBData& CBData_in)
{
  for (int ax = 0; ax < dim.x; ax++)
    for (int by = 0; by < dim.y; by++)
      for (int cz = 0; cz < dim.z; cz++)
      {
        int tx = pos.x + ax;
        int ty = pos.y + by;
        int tz = pos.z + cz;

        if (!validIndex (tx, ty, tz))
          continue;

        int i = index (tx, ty, tz);

        CBData_in.voxGrid[i].free = false;
        CBData_in.voxGrid[i].slave = true;
      } // end FOR

  //top left - is the master
  int i = index (pos.x, pos.y, pos.z);
  CBData_in.voxGrid[i].slave = false;
  CBData_in.voxGrid[i].dimension = dim;
}

void
irregularSubDivide (struct Engine_OpenGL_GLUT_253_CBData& CBData_in)
{
  glm::vec3 vsize (ENGINE_GLUT_253_DEFAULT_DIMENSION_X, ENGINE_GLUT_253_DEFAULT_DIMENSION_Y, ENGINE_GLUT_253_DEFAULT_DIMENSION_Z);
  float sdb = ENGINE_GLUT_253_DEFAULT_SUBDIVIDEBIAS;

  for (int32_t x = 0; x < vsize.x; x++)
    for (int32_t y = 0; y < vsize.y; y++)
      for (int32_t z = 0; z < vsize.z; z++)
      {
        if (!CBData_in.voxGrid[index(x, y, z)].free)
          continue;

        for (int tries = 0; tries < 20; tries++)
        {
          //Get me a random potential size
          int32_t n0 =
            static_cast<int32_t> (std::floor (Common_Tools::getRandomNumber (0.0f, (vsize.x * sdb - 1.0f)) + 1.0f));
          int32_t n1 =
            static_cast<int32_t> (std::floor (Common_Tools::getRandomNumber (0.0f, (vsize.y * sdb - 1.0f)) + 1.0f));
          int32_t n2 =
            static_cast<int32_t> (std::floor (Common_Tools::getRandomNumber (0.0f, (vsize.z * sdb - 1.0f)) + 1.0f));

          //check all directions and if all free use them up
          std::vector<int32_t> abc = {n0, n1, n2};
          static std::random_device rd;
          static std::default_random_engine rng (rd ());
          std::shuffle (abc.begin (), abc.end (), rng);

          glm::i32vec3 pos (x, y, z);
          glm::i32vec3 dim (abc[0], abc[1], abc[2]);
          if (areAllFree (pos, dim, CBData_in))
          {
            claim (pos, dim, CBData_in);
            break;
          } // end IF
        } // end FOR
      } // end FOR
}
