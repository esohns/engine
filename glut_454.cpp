#include "stdafx.h"

#include "glut_454.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

void
engine_glut_454_reshape (int width_in, int height_in)
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
engine_glut_454_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
     glutLeaveMainLoop ();
     break;
  } // end SWITCH
}

void
engine_glut_454_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.z = 1000.0f;

      cb_data_p->camera.rotation.y = 0.0f;
      break;
    default:
      break;
  } // end SWITCH
}

void
engine_glut_454_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
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
engine_glut_454_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  if (button == GLUT_LEFT_BUTTON)
  {
    cb_data_p->mouse_0_ButtonDown = (state == GLUT_DOWN);
  } // end IF
}

void
engine_glut_454_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mousex = x;
  cb_data_p->mousey = y;
}

void
engine_glut_454_timer (int v)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 30, engine_glut_454_timer, v);
}

void
engine_glut_454_draw (void)
{
  struct Engine_OpenGL_GLUT_454_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_454_CBData*> (glutGetWindowData ());
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

  static int frame_count_i = 1;
  glRotatef (-0.2f * frame_count_i, 0.0f, 1.0f, 0.0f);
  glRotatef (-0.1f * frame_count_i, 0.0f, 0.0f, 1.0f);
  glRotatef (-0.1f * frame_count_i, 1.0f, 0.0f, 0.0f);

  for (std::vector<agent>::iterator iterator = cb_data_p->agents.begin ();
       iterator != cb_data_p->agents.end ();
       ++iterator)
  {
    (*iterator).update ();
    (*iterator).show ();
  } // end FOR

  glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
  glColor3ub (255, 255, 255);
  for (int i = 0; i < 6; i++)
  {
    enum directions direction_e = static_cast<enum directions> (i);
    glPushMatrix ();
    switch (direction_e)
    {
      case S_FRONT:
        glTranslatef (0.0f, 0.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f);
        break;
      case S_BACK:
        glTranslatef (0.0f, 0.0f, -ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f);
        glRotatef (180.0f, 0.0f, 1.0f, 0.0f);
        break;
      case S_TOP:
        glTranslatef (0.0f, -ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f);
        glRotatef (90.0f, 1.0f, 0.0f, 0.0f);
        break;
      case S_BOTTOM:
        glTranslatef (0.0f, ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f);
        glRotatef (-90.0f, 1.0f, 0.0f, 0.0f);
        break;
      case S_RIGHT:
        glTranslatef (ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f, 0.0f);
        glRotatef (90.0f, 0.0f, 1.0f, 0.0f);
        break;
      case S_LEFT:
        glTranslatef (-ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f, 0.0f);
        glRotatef (-90.0f, 0.0f, 1.0f, 0.0f);
        break;
      default:
        break;
    } // end SWITCH
    glTranslatef (-ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, -ENGINE_GLUT_454_DEFAULT_CUBE_SIZE / 2.0f, 0.0f);
    glRectf (0.0f, 0.0f,
             static_cast<float> (ENGINE_GLUT_454_DEFAULT_CUBE_SIZE), static_cast<float> (ENGINE_GLUT_454_DEFAULT_CUBE_SIZE));
    glPopMatrix ();
  } // end FOR

  glutSwapBuffers ();

  ++frame_count_i;
}

void
engine_glut_454_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_454_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_454_idle);
  else
    glutIdleFunc (NULL);
}
