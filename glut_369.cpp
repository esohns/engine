#include "stdafx.h"

#include "glut_369.h"

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

#include "defines_17.h"
#include "engine_common.h"

void
engine_glut_369_reshape (int width_in, int height_in)
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
engine_glut_369_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_369_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
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
engine_glut_369_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
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
engine_glut_369_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    //case GLUT_LEFT_BUTTON:
    //{
    //  if (state == GLUT_DOWN)
    //  {
    //  } // end IF

    //  break;
    //}
    case GLUT_MIDDLE_BUTTON:
    {
      cb_data_p->mouseCenterPressed = (state == GLUT_DOWN);
      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_369_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->pmouseX = cb_data_p->mouseX;
  cb_data_p->pmouseY = cb_data_p->mouseY;

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_369_timer (int v)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutTimerFunc (1000 / 30,
                 engine_glut_369_timer,
                 v);
}

void
engine_glut_369_draw (void)
{
  struct Engine_OpenGL_GLUT_369_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_369_CBData*> (glutGetWindowData ());
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

  cb_data_p->trail.push_back (std::make_pair (cb_data_p->mouseX, cb_data_p->mouseY));
  int removeCount = 1;
  if (cb_data_p->mouseCenterPressed)
    removeCount++;
  for (int i = 0; i < removeCount; i++)
  {
    if (cb_data_p->trail.empty ())
      break;
    if (cb_data_p->mouseCenterPressed || cb_data_p->trail.size () > ENGINE_GLUT_369_DEFAULT_MAX_TRAIL_COUNT)
      cb_data_p->trail.erase (cb_data_p->trail.begin ());
  } // end FOR

  // create particles
  if (cb_data_p->trail.size () > 1 && cb_data_p->particles.size () < ENGINE_GLUT_369_DEFAULT_MAX_PARTICLE_COUNT)
  {
    glm::vec2 mouse (cb_data_p->mouseX, cb_data_p->mouseY);
    mouse -= glm::vec2 (cb_data_p->pmouseX, cb_data_p->pmouseY);
    if (glm::length (mouse) > 10.0f)
    {
      mouse = glm::normalize (mouse);
      int color_index_i = Common_Tools::getRandomNumber (0, ENGINE_GLUT_369_DEFAULT_MAX_COLOR_COUNT - 1);
      cb_data_p->particles.push_back (particle (static_cast<float> (cb_data_p->pmouseX), static_cast<float> (cb_data_p->pmouseY), mouse.x, mouse.y, color_index_i));
    } // end IF
  } // end IF

  // move and kill particles
  for (int i = cb_data_p->particles.size () - 1; i > -1; i--)
  {
    cb_data_p->particles[i].move ();

    if (glm::length (cb_data_p->particles[i].velocity_) < 0.1f)
    {
      std::vector<particle>::iterator iterator = cb_data_p->particles.begin ();
      std::advance (iterator, i);
      cb_data_p->particles.erase (iterator);
    } // end IF
  } // end FOR

  // update uniforms
  glProgramUniform2f (cb_data_p->programId, cb_data_p->resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_369_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_369_DEFAULT_HEIGHT));
  glProgramUniform1i (cb_data_p->programId, cb_data_p->trailCountLoc,
                      static_cast<int> (cb_data_p->trail.size ()));
  std::vector<float> trails_a;
  for (int i = 0; i < cb_data_p->trail.size (); i++)
  {
    trails_a.push_back (Common_GL_Tools::map (static_cast<float> (cb_data_p->trail[i].first), 0.0f, static_cast<float> (ENGINE_GLUT_369_DEFAULT_WIDTH), 0.0f, 1.0f));
    trails_a.push_back (Common_GL_Tools::map (static_cast<float> (cb_data_p->trail[i].second), 0.0f, static_cast<float> (ENGINE_GLUT_369_DEFAULT_HEIGHT), 1.0f, 0.0f));
  } // end FOR
  glProgramUniform2fv (cb_data_p->programId, cb_data_p->trailLoc,
                       static_cast<GLsizei> (cb_data_p->trail.size ()),
                       trails_a.data ());
  glProgramUniform1i (cb_data_p->programId, cb_data_p->particleCountLoc,
                      static_cast<int> (cb_data_p->particles.size ()));
  std::vector<float> particles_a;
  std::vector<float> colors_a;
  static Common_GL_Color_t palette_a[ENGINE_GLUT_369_DEFAULT_MAX_COLOR_COUNT] =
    {Common_GL_Color_t (0xe6, 0xf6, 0x60), Common_GL_Color_t (0xd2, 0x28, 0x43), Common_GL_Color_t (0xad, 0x86, 0x90), Common_GL_Color_t (0xb0, 0x56, 0xbd), Common_GL_Color_t (0x90, 0x67, 0x46)};
  for (int i = 0; i < cb_data_p->particles.size (); i++)
  {
    particles_a.push_back (Common_GL_Tools::map (static_cast<float> (cb_data_p->particles[i].position_.x), 0.0f, static_cast<float> (ENGINE_GLUT_369_DEFAULT_WIDTH), 0.0f, 1.0f));
    particles_a.push_back (Common_GL_Tools::map (static_cast<float> (cb_data_p->particles[i].position_.y), 0.0f, static_cast<float> (ENGINE_GLUT_369_DEFAULT_HEIGHT), 1.0f, 0.0f));
    particles_a.push_back (cb_data_p->particles[i].mass_ * glm::length (cb_data_p->particles[i].velocity_) / 100.0f);

    Common_GL_Color_t color = palette_a[cb_data_p->particles[i].colorIndex_];
    colors_a.push_back (static_cast<float> (color.r));
    colors_a.push_back (static_cast<float> (color.g));
    colors_a.push_back (static_cast<float> (color.b));
  } // end FOR
  glProgramUniform3fv (cb_data_p->programId, cb_data_p->particlesLoc,
                       static_cast<GLsizei> (cb_data_p->particles.size ()),
                       particles_a.data ());
  glProgramUniform3fv (cb_data_p->programId, cb_data_p->colorsLoc,
                       static_cast<GLsizei> (cb_data_p->particles.size ()),
                       colors_a.data ());

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_369_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_369_DEFAULT_HEIGHT / 2.0f),
                0.0f);

  glColor3f (1.0f, 1.0f, 1.0f);
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> (y * cb_data_p->scaleFactor), 0.0f);
      glVertex3f (static_cast<float> (x * cb_data_p->scaleFactor), static_cast<float> ((y + 1) * cb_data_p->scaleFactor), 0.0f);
    } // end FOR
    glEnd ();
  } // end FOR

  glutSwapBuffers ();
}

void
engine_glut_369_idle (void)
{
  glutPostRedisplay ();
}

void
engine_glut_369_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_369_idle);
  else
    glutIdleFunc (NULL);
}
