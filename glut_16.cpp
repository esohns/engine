#include "stdafx.h"

#include "glut_16.h"

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

#include "defines.h"
#include "engine_common.h"

gboolean
idle_initialize_UI_16_cb (gpointer userData_in)
{
  // sanity check(s)
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT (iterator != ui_cb_data_p->UIState->builders.end ());

  GtkWidget* dialog_p =
    GTK_WIDGET (gtk_builder_get_object ((*iterator).second.second,
                                        ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_DIALOG_MAIN_NAME)));
  ACE_ASSERT (dialog_p);
 
  // step2: (auto-)connect signals/slots
#if GTK_CHECK_VERSION(4,0,0)
#else
  gtk_builder_connect_signals ((*iterator).second.second,
                               ui_cb_data_p);
#endif // GTK_CHECK_VERSION(4,0,0)

  // step6a: connect default signals
#if GTK_CHECK_VERSION(4,0,0)
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
idle_finalize_UI_16_cb (gpointer userData_in)
{
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_UNUSED_ARG (ui_cb_data_p);

  return G_SOURCE_REMOVE;
}

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void
scale_m_value_changed_cb (GtkRange* range_in,
                          gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->m = gtk_range_get_value (range_in);
}

void
scale_n1_value_changed_cb (GtkRange* range_in,
                           gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->n1 = gtk_range_get_value (range_in);
}

void
scale_n2_value_changed_cb (GtkRange* range_in,
                           gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->n2 = gtk_range_get_value (range_in);
}

void
scale_n3_value_changed_cb (GtkRange* range_in,
                           gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->n3 = gtk_range_get_value (range_in);
}

void
button_reset_16_clicked_cb (GtkButton* button_in,
                            gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_16_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_16_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT(iterator != ui_cb_data_p->UIState->builders.end ());

  GtkScale* scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_M_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_16_DEFAULT_M);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_N1_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_16_DEFAULT_N1);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_N2_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_16_DEFAULT_N2);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_N3_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_GLUT_16_DEFAULT_N3);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

//////////////////////////////////////////

float
super_shape (float theta, float a, float b, float m, float n1, float n2, float n3)
{
  float t1 = std::pow (std::abs ((1 / a) * std::cos (m * theta / 4.0F)), n2);
  float t2 = std::pow (std::abs ((1 / b) * std::sin (m * theta / 4.0F)), n3);
  float t3 = t1 + t2;
  float r = std::pow (t3, - 1.0F / n1);
  return r;
}

void
engine_glut_16_key_special (int key_in,
                            int x,
                            int y)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
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
      cb_data_p->camera.position.x = 0.0F;
      cb_data_p->camera.position.y = -600.0f;
      cb_data_p->camera.position.z = 0.0f;
      cb_data_p->camera.rotation.z = 0.0f;
      break;
  } // end SWITCH
}

enum Engine_GLUTMode
{
  ENGINE_GLUT_MODE_WIREFRAME = 0,
  ENGINE_GLUT_MODE_COLOR,
  //////////////////////
  ENGINE_GLUT_MODE_INVALID,
  ENGINE_GLUT_MODE_MAX,
};

void
engine_glut_16_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  switch (entry_in)
  {
    case ENGINE_GLUT_MODE_WIREFRAME:
      cb_data_p->wireframe = !cb_data_p->wireframe; break;
    case ENGINE_GLUT_MODE_COLOR:
      cb_data_p->color = !cb_data_p->color; break;
    default:
      break;
  }
}

void
engine_glut_16_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
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
engine_glut_16_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
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
engine_glut_16_timer (int v)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  //if (cb_data_p->spinning)
  //{
    cb_data_p->angle += 1.0;
    if (cb_data_p->angle > 360.0)
    {
      cb_data_p->angle -= 360.0;
    }
  //  glutPostRedisplay();
  //} // end IF
  glutTimerFunc (1000 / 30, engine_glut_16_timer, v);
}

void
engine_glut_16_draw (void)
{
  struct Engine_OpenGL_GLUT_16_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_16_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
  glLoadIdentity ();
  COMMON_GL_ASSERT;

  // rotate the camera
  glm::mat4 rotation_matrix = glm::rotate (glm::mat4 (1.0f),
                                           glm::radians (cb_data_p->camera.rotation.z),
                                           glm::vec3 (0.0f, 0.0f, 1.0f));
  glm::vec3 rotation_center (0.0f, 0.0f, 0.0f);
  glm::vec4 pos_rot_h =
    rotation_matrix * glm::vec4 (cb_data_p->camera.position - rotation_center,
                                 1.0f);
  glm::vec3 pos_rot = glm::vec3 (pos_rot_h) + rotation_center;
  cb_data_p->camera.position = pos_rot;

  // set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);
  COMMON_GL_ASSERT;

  Common_GL_Color_t color_s = { 255, 255, 255 };
  if (!cb_data_p->color)
    glColor3ub (color_s.r, color_s.g, color_s.b);

  for (int i = 0; i < cb_data_p->total + 1; ++i)
  {
    float lat = Common_GL_Tools::map (static_cast<float> (i),
                                      0.0F, static_cast<float> (cb_data_p->total + 1),
                                      static_cast<float> (-M_PI / 2.0), static_cast<float> (M_PI / 2.0));
    float r2 = super_shape (lat,
                            cb_data_p->a,
                            cb_data_p->b,
                            cb_data_p->m,
                            cb_data_p->n1,
                            cb_data_p->n2,
                            cb_data_p->n3);
    for (int j = 0; j < cb_data_p->total + 1; ++j)
    {
      float lon = Common_GL_Tools::map (static_cast<float> (j),
                                        0.0F, static_cast<float> (cb_data_p->total + 1),
                                        static_cast<float> (-M_PI), static_cast<float> (M_PI));
      float r1 = super_shape (lat,
                              cb_data_p->a,
                              cb_data_p->b,
                              cb_data_p->m,
                              cb_data_p->n1,
                              cb_data_p->n2,
                              cb_data_p->n3);
      float x = cb_data_p->radius * r1 * std::cos (lon) * r2 * std::cos (lat);
      float y = cb_data_p->radius * r1 * std::sin (lon) * r2 * std::cos (lat);
      float z = cb_data_p->radius * r2 * std::sin (lat);
      cb_data_p->globe[i * (cb_data_p->total + 1) + j] = glm::vec3 (x, y, z);
    } // end FOR
  } // end FOR

  for (int i = 0; i < cb_data_p->total; ++i)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int j = 0; j < cb_data_p->total + 1; ++j)
    {
      glm::vec3 v1 = cb_data_p->globe[i * (cb_data_p->total + 1) + j];
      glVertex3f (v1.x, v1.y, v1.z);
      glm::vec3 v2 = cb_data_p->globe[(i + 1) * (cb_data_p->total + 1) + j];
      glVertex3f (v2.x, v2.y, v2.z);
    } // end FOR
    glEnd ();
  } // end FOR
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}
