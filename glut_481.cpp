#include "stdafx.h"

#include "glut_481.h"

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
#include "common_file_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines_22.h"
#include "engine_common.h"

void
engine_glut_481_reshape (int width_in, int height_in)
{
  glViewport (0, 0, width_in, height_in);

  glMatrixMode (GL_PROJECTION);

  glLoadIdentity ();

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  0.1, -1000.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_481_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_481_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case GLUT_KEY_LEFT:
      cb_data_p->camera.left (200.0f);
      break;
    case GLUT_KEY_RIGHT:
      cb_data_p->camera.right (200.0f);
      break;
    case GLUT_KEY_UP:
      cb_data_p->camera.position.x = 0.0f;
      cb_data_p->camera.position.y = 0.0f;
      cb_data_p->camera.position.z = 1500.0f;
      break;
  } // end SWITCH
}

void
engine_glut_481_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
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
engine_glut_481_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      if (state == GLUT_DOWN)
      {
      } // end IF

      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_481_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_481_timer (int v)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 60,
                 engine_glut_481_timer,
                 v);
}

void
engine_glut_481_draw (void)
{
  struct Engine_OpenGL_GLUT_481_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_481_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);

  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 = std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 = std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - cb_data_p->tp1;
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);

  // render pass 1
  glClampColor (GL_CLAMP_READ_COLOR, GL_FALSE);
  glClampColor (GL_CLAMP_VERTEX_COLOR, GL_FALSE);
  glClampColor (GL_CLAMP_FRAGMENT_COLOR, GL_FALSE);

  glBindVertexArray (cb_data_p->VAO);

  cb_data_p->shader1.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader1.id_, cb_data_p->S1resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_481_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_481_DEFAULT_HEIGHT));

  glProgramUniform1f (cb_data_p->shader1.id_, cb_data_p->S1timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  glProgramUniform2f (cb_data_p->shader1.id_, cb_data_p->S1mouseLoc,
                      static_cast<GLfloat> (cb_data_p->mouseX),
                      static_cast<GLfloat> (cb_data_p->mouseY));

  glActiveTexture (GL_TEXTURE0);
  cb_data_p->texture0.bind ();
  glProgramUniform1i (cb_data_p->shader1.id_, cb_data_p->S1channel0Loc,
                      static_cast<GLint> (0));

  glActiveTexture (GL_TEXTURE1);
  cb_data_p->textureS1.bind ();
  glProgramUniform1i (cb_data_p->shader1.id_, cb_data_p->S1channel1Loc,
                      static_cast<GLint> (1));

  // draw render pass 1 to framebuffer object (--> textureS1)
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, cb_data_p->FBO);
  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices
  glBindFramebufferEXT (GL_FRAMEBUFFER_EXT, 0);

  glBindVertexArray (0);

  cb_data_p->texture0.unbind ();
  cb_data_p->textureS1.unbind ();

  // render pass 2
  glBindVertexArray (cb_data_p->VAO_2);

  cb_data_p->shader2.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader2.id_, cb_data_p->S2resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_481_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_481_DEFAULT_HEIGHT));

  glActiveTexture (GL_TEXTURE1);
  cb_data_p->textureS1.bind ();
  glProgramUniform1i (cb_data_p->shader2.id_, cb_data_p->S2channel0Loc,
                      static_cast<GLint> (1));

  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices (see also: above)

  glBindVertexArray (0);

  cb_data_p->textureS1.unbind ();

  glutSwapBuffers ();
}

void
engine_glut_481_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_481_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_481_idle);
  else
    glutIdleFunc (NULL);
}
