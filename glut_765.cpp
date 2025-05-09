#include "stdafx.h"

#include "glut_765.h"

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

#include "common_time_common.h"
#include "common_tools.h"
#include "common_file_tools.h"

#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "defines_36.h"
#include "engine_common.h"

void
engine_glut_765_reshape (int width_in,
                         int height_in)
{
  glViewport (0, 0, width_in, height_in);

  //glMatrixMode (GL_PROJECTION);

  //glLoadIdentity ();

  //ACE_ASSERT (height_in);
  //gluPerspective (45.0,
  //                width_in / static_cast<GLdouble> (height_in),
  //                0.1, -1000.0);
  //glOrtho (static_cast<GLdouble> (-width_in / 2.0), static_cast<GLdouble> (width_in / 2.0),
  //         static_cast<GLdouble> (height_in / 2.0), static_cast<GLdouble> (-height_in / 2.0), 150.0, -150.0);

  //glMatrixMode (GL_MODELVIEW);
}

void
engine_glut_765_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
    //case 32: /* Space */
    //  cb_data_p->isSpaceKeyPressed = true;
    //  break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_765_key_up (unsigned char key_in,
                        int x,
                        int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    //case 32: /* Space */
    //  cb_data_p->isSpaceKeyPressed = false;
    //  break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_765_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    //case 114: // LCtrl
    //case 115: // RCtrl
    //  cb_data_p->isCtrlKeyPressed = true;
    //  break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_765_key_special_up (int key_in,
                                int x,
                                int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    //case 114: // LCtrl
    //case 115: // RCtrl
    //  cb_data_p->isCtrlKeyPressed = false;
    //  break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_765_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
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
engine_glut_765_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (button)
  {
    case GLUT_LEFT_BUTTON:
    {
      cb_data_p->mouseLMBPressed = (state == GLUT_DOWN);
      break;
    }
    default:
      break;
  } // end IF
}

void
engine_glut_765_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_765_timer (int v)
{
  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 60,
                 engine_glut_765_timer,
                 v);
}

void
engine_glut_765_draw (void)
{
  static int frame_counter_i = 1;

  struct Engine_OpenGL_GLUT_765_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_765_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // reset transformations
  //glMatrixMode (GL_MODELVIEW);
  //glLoadIdentity ();

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

  glBindVertexArray (cb_data_p->VAO);

  // render pass 1
  cb_data_p->shader1.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader1.id_, cb_data_p->S1resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_HEIGHT));

  glProgramUniform1i (cb_data_p->shader1.id_, cb_data_p->S1channel0Loc,
                      static_cast<GLint> (3));

  // draw render pass 1 to framebuffer object (--> textureS1)
  glBindFramebuffer (GL_FRAMEBUFFER, cb_data_p->FBO1);
  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices
  //glDrawArrays (GL_QUADS, 0, 4); // 1 quad --> 4 vertices
  glBindFramebuffer (GL_FRAMEBUFFER, 0);

  // render pass 2
  cb_data_p->shader2.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader2.id_, cb_data_p->S2resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_HEIGHT));

  glProgramUniform1i (cb_data_p->shader2.id_, cb_data_p->S2channel0Loc,
                      static_cast<GLint> (1));

  // draw render pass 2 to framebuffer object (--> textureS2)
  glBindFramebuffer (GL_FRAMEBUFFER, cb_data_p->FBO2);
  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices
  //glDrawArrays (GL_QUADS, 0, 4); // 1 quad --> 4 vertices
  glBindFramebuffer (GL_FRAMEBUFFER, 0);

  // render pass 3
  cb_data_p->shader3.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader3.id_, cb_data_p->S3resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_HEIGHT));

  glProgramUniform1f (cb_data_p->shader3.id_, cb_data_p->S3timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  glProgramUniform1i (cb_data_p->shader3.id_, cb_data_p->S3frameLoc,
                      static_cast<GLint> (frame_counter_i - 1));

  glProgramUniform4f (cb_data_p->shader3.id_, cb_data_p->S3mouseLoc,
                      static_cast<GLfloat> (cb_data_p->mouseX),
                      static_cast<GLfloat> (Common_GL_Tools::map (cb_data_p->mouseY, 0, ENGINE_GLUT_765_DEFAULT_HEIGHT - 1, ENGINE_GLUT_765_DEFAULT_HEIGHT - 1, 0)),
                      static_cast<GLfloat> (cb_data_p->mouseLMBPressed ? 1 : 0),
                      static_cast<GLfloat> (0));

  glProgramUniform1i (cb_data_p->shader3.id_, cb_data_p->S3channel0Loc,
                      static_cast<GLint> (2));
  glProgramUniform1i (cb_data_p->shader3.id_, cb_data_p->S3channel1Loc,
                      static_cast<GLint> (0));

  // draw render pass 3 to framebuffer object (--> textureS3)
  glBindFramebuffer (GL_FRAMEBUFFER, cb_data_p->FBO3);
  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices
  //glDrawArrays (GL_QUADS, 0, 4); // 1 quad --> 4 vertices
  glBindFramebuffer (GL_FRAMEBUFFER, 0);

  // render pass 4
  cb_data_p->shader4.use ();

  // update uniforms
  glProgramUniform2f (cb_data_p->shader4.id_, cb_data_p->S4resolutionLoc,
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_765_DEFAULT_HEIGHT));

  glProgramUniform1f (cb_data_p->shader4.id_, cb_data_p->S4timeLoc,
                      static_cast<GLfloat> (d.count () * 0.001f));

  glProgramUniform1i (cb_data_p->shader4.id_, cb_data_p->S4channel0Loc,
                      static_cast<GLint> (3));
  glProgramUniform1i (cb_data_p->shader4.id_, cb_data_p->S4channel1Loc,
                      static_cast<GLint> (0));

  // draw render pass 4 to screen
  glDrawArrays (GL_TRIANGLES, 0, 6); // 2 triangles --> 6 vertices
  //glDrawArrays (GL_QUADS, 0, 4); // 1 quad --> 4 vertices

  glBindVertexArray (0);

  glutSwapBuffers ();

  ++frame_counter_i;
}

void
engine_glut_765_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_765_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_765_idle);
  else
    glutIdleFunc (NULL);
}
