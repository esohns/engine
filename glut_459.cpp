#include "stdafx.h"

#include "glut_459.h"

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

#include "common_gl_common.h"
#include "common_gl_defines.h"
#include "common_gl_tools.h"

#include "common_ui_defines.h"

#include "common_ui_gtk_manager_common.h"

#include "engine_common.h"

#include "defines.h"
#include "defines_21.h"

gboolean
idle_initialize_UI_459_cb (gpointer userData_in)
{
  // sanity check(s)
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT (iterator != ui_cb_data_p->UIState->builders.end ());

  GtkWidget* dialog_p =
    GTK_WIDGET (gtk_builder_get_object ((*iterator).second.second,
                                        ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_DIALOG_MAIN_NAME)));
  ACE_ASSERT (dialog_p);
 
  // step2: (auto-)connect signals/slots
#if GTK_CHECK_VERSION (4,0,0)
#else
  gtk_builder_connect_signals ((*iterator).second.second,
                               ui_cb_data_p);
#endif // GTK_CHECK_VERSION(4,0,0)

  // step6a: connect default signals
#if GTK_CHECK_VERSION (4,0,0)
#else
  gulong result_2 =
      g_signal_connect (dialog_p,
                        ACE_TEXT_ALWAYS_CHAR ("destroy"),
                        G_CALLBACK (gtk_widget_destroyed),
                        &dialog_p);
  ACE_ASSERT (result_2);
#endif // GTK_CHECK_VERSION(4,0,0)

  // step9: draw main dialog
#if GTK_CHECK_VERSION(4,0,0)
  gtk_widget_show (dialog_p);
#else
  gtk_widget_show_all (dialog_p);
#endif // GTK_CHECK_VERSION(4,0,0)

  return G_SOURCE_REMOVE;
}

gboolean
idle_finalize_UI_459_cb (gpointer userData_in)
{
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_UNUSED_ARG (ui_cb_data_p);

  gtk_main_quit ();

  return G_SOURCE_REMOVE;
}

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void
scale_noise_scale_value_changed_cb (GtkRange* range_in,
                                    gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->scale =
    static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_noise_octaves_value_changed_cb (GtkRange* range_in,
                                      gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->octaves =
    static_cast<int> (gtk_range_get_value (range_in));
}

void
scale_noise_factor_value_changed_cb (GtkRange* range_in,
                                     gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->noiseFactor =
    static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_frequency_value_changed_cb (GtkRange* range_in,
                                  gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->frequency =
    static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_sharpness_value_changed_cb (GtkRange* range_in,
                                  gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->sharpness =
    static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_pattern_value_changed_cb (GtkRange* range_in,
                                gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->pattern =
    static_cast<int> (gtk_range_get_value (range_in));
}

void
button_reset_459_clicked_cb (GtkButton* button_in,
                             gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_459_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_459_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT(iterator != ui_cb_data_p->UIState->builders.end ());

  GtkScale* scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_SCALE_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_NOISE_SCALE);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_OCTAVES_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_NOISE_OCTAVES);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_NOISE_FACTOR_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_NOISE_FACTOR);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_FREQUENCY_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_FREQUENCY);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_SHARPNESS_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_SHARPNESS);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_GLUT_459_UI_GTK_SCALE_PATTERN_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_459_DEFAULT_PATTERN);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

//////////////////////////////////////////

void
engine_glut_459_reshape (int width_in, int height_in)
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
engine_glut_459_key (unsigned char key_in,
                     int x,
                     int y)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (key_in)
  {
    case 27: /* Escape */
      glutLeaveMainLoop ();
      break;
  } // end SWITCH
}

void
engine_glut_459_key_special (int key_in,
                             int x,
                             int y)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
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
engine_glut_459_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
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
engine_glut_459_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
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
  } // end SWITCH
}

void
engine_glut_459_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  cb_data_p->mouseX = x;
  cb_data_p->mouseY = y;
}

void
engine_glut_459_timer (int v)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glutPostRedisplay ();

  glutTimerFunc (1000 / 30,
                 engine_glut_459_timer,
                 v);
}

void
engine_glut_459_draw (void)
{
  struct Engine_OpenGL_GLUT_459_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_459_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Reset transformations
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
                      static_cast<GLfloat> (ENGINE_GLUT_459_DEFAULT_WIDTH),
                      static_cast<GLfloat> (ENGINE_GLUT_459_DEFAULT_HEIGHT));

  static int frame_count_i = 1;
  cb_data_p->phase.x = frame_count_i / 1000.0f;
  glProgramUniform2f (cb_data_p->shader.id_, cb_data_p->phaseLoc,
                      static_cast<GLfloat> (cb_data_p->phase.x),
                      static_cast<GLfloat> (cb_data_p->phase.y));
  ++frame_count_i;

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->scaleLoc,
                      static_cast<GLfloat> (cb_data_p->scale));

  glProgramUniform1i (cb_data_p->shader.id_, cb_data_p->octavesLoc,
                      static_cast<GLint> (cb_data_p->octaves));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->frequencyLoc,
                      static_cast<GLfloat> (cb_data_p->frequency));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->noiseFactorLoc,
                      static_cast<GLfloat> (cb_data_p->noiseFactor));

  glProgramUniform1f (cb_data_p->shader.id_, cb_data_p->sharpnessLoc,
                      static_cast<GLfloat> (std::pow (2, cb_data_p->sharpness)));

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

  glProgramUniform1i (cb_data_p->shader.id_, cb_data_p->patternLoc,
                      static_cast<GLint> (cb_data_p->pattern));

  glTranslatef (static_cast<GLfloat> (-ENGINE_GLUT_459_DEFAULT_WIDTH / 2.0f),
                static_cast<GLfloat> (-ENGINE_GLUT_459_DEFAULT_HEIGHT / 2.0f),
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
engine_glut_459_idle (void)
{
  //glutPostRedisplay ();
}

void
engine_glut_459_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_459_idle);
  else
    glutIdleFunc (NULL);
}
