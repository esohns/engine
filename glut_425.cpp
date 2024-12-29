#include "stdafx.h"

#include "glut_425.h"

#include "GL/glew.h"
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

#include "engine_common.h"

#include "defines_19.h"

void
engine_glut_425_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  150.0, -150.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_425_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_425_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 500.0f;

      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

void
engine_glut_425_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
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
engine_glut_425_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      cb_data_p->mouseLMBPressed = (state == GLUT_DOWN);
      if (cb_data_p->mouseLMBPressed)
        randomizeParameters (*cb_data_p);
      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_425_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_425_timer (int v)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_425_timer,
                 v);
}

void
engine_glut_425_draw (void)
{
  struct Engine_OpenGL_GLUT_425_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_425_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // draw a red x-axis, a green y-axis, and a blue z-axis. Each of the
  // axes are 100 units long
  glBegin (GL_LINES);
  glColor3f (1.0f, 0.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0f, 1.0f, 0.0f); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0f, 0.0f, 1.0f); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader.id_, cb_data_p->resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_425_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_425_DEFAULT_HEIGHT));

  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 =
    std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 =
    std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - cb_data_p->tp1;
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);
  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->skewFactorLoc,
                      static_cast<GLfloat> (cb_data_p->skewFactor));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->rotationSpeedLoc,
                      static_cast<GLfloat> (cb_data_p->rotationSpeed));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->wobblynessLoc,
                      static_cast<GLfloat> (cb_data_p->wobblyness));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->subDivisionsLoc,
                      static_cast<GLfloat> (cb_data_p->subDivisions));

  std::vector<float> colors_a;
  for (int i = 0; i < 5; i++)
  {
    Common_GL_Color_t color = cb_data_p->palette[i];
    colors_a.push_back (static_cast<float> (color.r) / 255.0f);
    colors_a.push_back (static_cast<float> (color.g) / 255.0f);
    colors_a.push_back (static_cast<float> (color.b) / 255.0f);
  } // end FOR
  glProgramUniform3fv (cb_data_p->shader.id_, cb_data_p->paletteLoc,
                       static_cast<GLsizei> (5),
                       colors_a.data ());

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->spiralFactorLoc,
                      static_cast<GLfloat> (cb_data_p->spiralFactor));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->spiralSpeedLoc,
                      static_cast<GLfloat> (cb_data_p->spiralSpeed));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->xOffLoc,
                      static_cast<GLfloat> (Common_GL_Tools::map (static_cast<float> (cb_data_p->mouseX), ENGINE_GLUT_425_DEFAULT_WIDTH / 4.0f, ENGINE_GLUT_425_DEFAULT_WIDTH * 3.0f / 4.0f, -0.2f, 0.2f)));
  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->yOffLoc,
                      static_cast<GLfloat> (Common_GL_Tools::map (static_cast<float> (cb_data_p->mouseY), ENGINE_GLUT_425_DEFAULT_HEIGHT / 4.0f, ENGINE_GLUT_425_DEFAULT_HEIGHT * 3.0f / 4.0f, -0.2f, 0.2f)));

  glProgramUniform1i (cb_data_p->shader.id_, cb_data_p->numColorsLoc,
                      static_cast<GLint> (cb_data_p->numColors));

  glProgramUniform1i (cb_data_p->shader.id_, cb_data_p->blendColorsLoc,
                      static_cast<GLint> (cb_data_p->blendColors ? 1 : 0));
  glProgramUniform1i (cb_data_p->shader.id_, cb_data_p->blendSameLoc,
                      static_cast<GLint> (cb_data_p->blendSame ? 1 : 0));

  //glBegin (GL_QUADS);
  //glVertex3f (-1.0f, -1.0f, 0.0f);
  //glVertex3f ( 1.0f, -1.0f, 0.0f);
  //glVertex3f ( 1.0f,  1.0f, 0.0f);
  //glVertex3f (-1.0f,  1.0f, 0.0f);
  //glEnd ();

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_425_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_425_DEFAULT_HEIGHT / 2.0f),
                0.0f);

  glColor3f (1.0f, 1.0f, 1.0f);
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ( y      * cb_data_p->scaleFactor), 0.0f);
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ((y + 1) * cb_data_p->scaleFactor), 0.0f);
    } // end FOR
    glEnd ();
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_425_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_425_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_425_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

void
randomizeParameters (struct Engine_OpenGL_GLUT_425_CBData& CBData_inout)
{
  CBData_inout.paletteIndex =
    Common_Tools::getRandomNumber (0, ENGINE_GLUT_425_DEFAULT_NUMBER_OF_PALETTES - 1);
  static Common_GL_Color_t palettes_a[ENGINE_GLUT_425_DEFAULT_NUMBER_OF_PALETTES][5] = {
    {Common_GL_Color_t (0xed, 0x22, 0x5d, 0xff), Common_GL_Color_t (0x3c, 0xaf, 0x65, 0xff), Common_GL_Color_t (0x0d, 0x40, 0xbf, 0xff), Common_GL_Color_t (0xf5, 0xb8, 0x00, 0xff), Common_GL_Color_t (0x2a, 0x2a, 0x2a, 0xff)},
    {Common_GL_Color_t (0xb3, 0x00, 0x00, 0xff), Common_GL_Color_t (0xe6, 0xcf, 0x00, 0xff), Common_GL_Color_t (0x12, 0x83, 0xb3, 0xff), Common_GL_Color_t (0xfa, 0xfa, 0xfa, 0xff), Common_GL_Color_t (0x0a, 0x0a, 0x0a, 0xff)},
    {Common_GL_Color_t (0x60, 0x6c, 0x38, 0xff), Common_GL_Color_t (0x28, 0x36, 0x18, 0xff), Common_GL_Color_t (0xfe, 0xfa, 0xe0, 0xff), Common_GL_Color_t (0xdd, 0xa1, 0x5e, 0xff), Common_GL_Color_t (0xbc, 0x6c, 0x25, 0xff)},
    {Common_GL_Color_t (0x00, 0x14, 0x27, 0xff), Common_GL_Color_t (0x70, 0x8d, 0x81, 0xff), Common_GL_Color_t (0xf4, 0xd5, 0x8d, 0xff), Common_GL_Color_t (0xbf, 0x06, 0x03, 0xff), Common_GL_Color_t (0x8d, 0x08, 0x01, 0xff)},
    {Common_GL_Color_t (0x3d, 0x5a, 0x80, 0xff), Common_GL_Color_t (0x98, 0xc1, 0xd9, 0xff), Common_GL_Color_t (0xe0, 0xfb, 0xfc, 0xff), Common_GL_Color_t (0xee, 0x6c, 0x4d, 0xff), Common_GL_Color_t (0x29, 0x32, 0x41, 0xff)},
    {Common_GL_Color_t (0x44, 0x64, 0xa1, 0xff), Common_GL_Color_t (0x62, 0xb6, 0xde, 0xff), Common_GL_Color_t (0xb3, 0xdc, 0xe0, 0xff), Common_GL_Color_t (0xfa, 0xfa, 0xfa, 0xff), Common_GL_Color_t (0xff, 0xc5, 0xc7, 0xff)},
    {Common_GL_Color_t (0x35, 0x35, 0x35, 0xff), Common_GL_Color_t (0x3c, 0x6e, 0x71, 0xff), Common_GL_Color_t (0xff, 0xff, 0xff, 0xff), Common_GL_Color_t (0xd9, 0xd9, 0xd9, 0xff), Common_GL_Color_t (0x28, 0x4b, 0x63, 0xff)},
    {Common_GL_Color_t (0x00, 0x81, 0xa7, 0xff), Common_GL_Color_t (0x00, 0xaf, 0xb9, 0xff), Common_GL_Color_t (0xfd, 0xfc, 0xdc, 0xff), Common_GL_Color_t (0xfe, 0xd9, 0xb7, 0xff), Common_GL_Color_t (0xf0, 0x71, 0x67, 0xff)},
    {Common_GL_Color_t (0x00, 0x4b, 0x23, 0xff), Common_GL_Color_t (0x00, 0x64, 0x00, 0xff), Common_GL_Color_t (0x00, 0x72, 0x00, 0xff), Common_GL_Color_t (0x00, 0x80, 0x00, 0xff), Common_GL_Color_t (0x38, 0xb0, 0x00, 0xff)},
    {Common_GL_Color_t (0x0a, 0x11, 0x28, 0xff), Common_GL_Color_t (0x00, 0x1f, 0x54, 0xff), Common_GL_Color_t (0x03, 0x40, 0x78, 0xff), Common_GL_Color_t (0x12, 0x82, 0xa2, 0xff), Common_GL_Color_t (0xfe, 0xfc, 0xfb, 0xff)},
    {Common_GL_Color_t (0x2d, 0x31, 0x42, 0xff), Common_GL_Color_t (0xbf, 0xc0, 0xc0, 0xff), Common_GL_Color_t (0xff, 0xff, 0xff, 0xff), Common_GL_Color_t (0xef, 0x83, 0x54, 0xff), Common_GL_Color_t (0x4f, 0x5d, 0x75, 0xff)},
    {Common_GL_Color_t (0x04, 0x15, 0x1f, 0xff), Common_GL_Color_t (0x18, 0x3a, 0x37, 0xff), Common_GL_Color_t (0xef, 0xd6, 0xac, 0xff), Common_GL_Color_t (0xc4, 0x49, 0x00, 0xff), Common_GL_Color_t (0x43, 0x25, 0x34, 0xff)},
    {Common_GL_Color_t (0xca, 0x9c, 0xe1, 0xff), Common_GL_Color_t (0x54, 0xde, 0xfd, 0xff), Common_GL_Color_t (0xf1, 0xff, 0xc4, 0xff), Common_GL_Color_t (0xff, 0xca, 0xaf, 0xff), Common_GL_Color_t (0x28, 0x58, 0x7b, 0xff)}
  };
  CBData_inout.palette.clear ();
  for (int i = 0; i < 5; ++i)
    CBData_inout.palette.push_back (palettes_a[CBData_inout.paletteIndex][i]);
  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (CBData_inout.palette.begin (), CBData_inout.palette.end (), rng);

  CBData_inout.rotationSpeed = Common_Tools::getRandomNumber (0.0f, 1.0f);
  CBData_inout.rotationSpeed *= CBData_inout.rotationSpeed;
  if (Common_Tools::testRandomProbability (0.5f))
    CBData_inout.rotationSpeed *= -1.0f;
  CBData_inout.rotationSpeed *= 0.5f;

  CBData_inout.skewFactor = Common_Tools::getRandomNumber (0.0f, 1.0f);
  CBData_inout.skewFactor *= CBData_inout.skewFactor;
  if (Common_Tools::testRandomProbability (0.5f))
    CBData_inout.skewFactor *= -1.0f;
  CBData_inout.skewFactor *= 0.7f;

  CBData_inout.wobblyness = Common_Tools::getRandomNumber (-0.4f, 0.0f);

  float temp_f = Common_Tools::getRandomNumber (0.0f, 1.0f);
  temp_f *= temp_f;
  CBData_inout.subDivisions = std::floor (2.0f + (temp_f * 12.0f));

  temp_f = Common_Tools::getRandomNumber (0.0f, 1.0f);
  temp_f *= temp_f;
  CBData_inout.spiralFactor = 0.5f + (temp_f * 1.5f);

  temp_f = Common_Tools::getRandomNumber (0.0f, 1.0f);
  temp_f *= temp_f;
  CBData_inout.spiralSpeed = -(1.0f + (temp_f * 2.0f));

  CBData_inout.xOff = Common_Tools::getRandomNumber (-0.2f, 0.2f);
  CBData_inout.yOff = Common_Tools::getRandomNumber (-0.2f, 0.2f);

  CBData_inout.numColors = Common_Tools::getRandomNumber (3, 5);

  CBData_inout.blendColors = Common_Tools::testRandomProbability (0.3f);
  CBData_inout.blendSame =
    CBData_inout.blendColors && Common_Tools::testRandomProbability (0.2f);
}
