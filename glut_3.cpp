#include "stdafx.h"

#include "glut_3.h"

#if defined (ACE_WIN32) || defined (ACE_WIN64)
#include "gl/GL.h"
#else
#include "GL/gl.h"
#endif // ACE_WIN32 || ACE_WIN64
#include "GL/freeglut.h"

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

void
engine_glut_3_reshape (int width_in, int height_in)
{
  glViewport (0, 0,
              static_cast<GLsizei> (width_in), static_cast<GLsizei> (height_in));
  COMMON_GL_ASSERT;

  glMatrixMode (GL_PROJECTION);
  COMMON_GL_ASSERT;

  glLoadIdentity ();
  COMMON_GL_ASSERT;

  ACE_ASSERT (height_in);
  gluPerspective (45.0,
                  width_in / static_cast<GLdouble> (height_in),
                  -1.0, 1.0);
  COMMON_GL_ASSERT;

  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
}

void
engine_glut_3_key (unsigned char k, int x, int y)
{
  switch (k) {
  case 27:  /* Escape */
    //exit (0);
    glutLeaveMainLoop ();

    Common_UI_GTK_Manager_t* gtk_manager_p =
      COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
    ACE_ASSERT (gtk_manager_p);
    gtk_manager_p->stop (false,  // wait ?
                         false);

    break;
  }
}

void
engine_glut_3_menu (int entry_in)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
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
engine_glut_3_mouse_button (int button, int state, int x, int y)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
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
engine_glut_3_mouse_move (int x, int y)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
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
engine_glut_3_timer (int v)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData());
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
  glutTimerFunc (1000 / 30, engine_glut_3_timer, v);
}

void
engine_glut_3_draw (void)
{
  struct Engine_OpenGL_GLUT_3_CBData* cb_data_p =
    static_cast<struct Engine_OpenGL_GLUT_3_CBData*> (glutGetWindowData ());
  ACE_ASSERT (cb_data_p);

  // compute terrain
  cb_data_p->yOffset += 0.03;
  double xoff;
  double yoff = cb_data_p->yOffset;
  for (int y = 0; y < cb_data_p->rows; ++y)
  {
    xoff = 0.0;
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      //cb_data_s.terrain[y * cb_data_s.rows + x] = Common_Tools::getRandomNumber (-10, 10);
      cb_data_p->terrain[y * cb_data_p->rows + x] =
        static_cast<float> ((cb_data_p->module.GetValue (cb_data_p->x + xoff, cb_data_p->y + yoff, cb_data_p->z) * cb_data_p->level));
      xoff += cb_data_p->step;
    } // end FOR
    yoff += cb_data_p->step;
  } // end FOR

  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  COMMON_GL_ASSERT;

  // Reset transformations
  glMatrixMode (GL_MODELVIEW);
  COMMON_GL_ASSERT;
  glLoadIdentity ();
  COMMON_GL_ASSERT;

  //glTranslatef (cb_data_p->size.cx / 2.0F, cb_data_p->size.cy / 2.0F, 0);
  //glRotatef (static_cast<float> (M_PI) / 3.0F, 1.0F, 0.0F, 0.0F);

  // Set the camera
  gluLookAt (cb_data_p->camera.position.x, cb_data_p->camera.position.y, cb_data_p->camera.position.z,
             cb_data_p->camera.looking_at.x, cb_data_p->camera.looking_at.y, cb_data_p->camera.looking_at.z,
             cb_data_p->camera.up.x, cb_data_p->camera.up.y, cb_data_p->camera.up.z);

  glPolygonMode (GL_FRONT_AND_BACK,
                 cb_data_p->wireframe ? GL_LINE : GL_FILL);
  COMMON_GL_ASSERT;

  // Draw a red x-axis, a green y-axis, and a blue z-axis.  Each of the
  // axes are ten units long.
  glBegin (GL_LINES);
  glColor3f (1.0F, 0.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (100, 0, 0);
  glColor3f (0.0F, 1.0F, 0.0F); glVertex3i (0, 0, 0); glVertex3i (0, 100, 0);
  glColor3f (0.0F, 0.0F, 1.0F); glVertex3i (0, 0, 0); glVertex3i (0, 0, 100);
  glEnd ();
  COMMON_GL_ASSERT;

  if (!cb_data_p->color)
    glColor3ub (255, 255, 255);

  Common_GL_Color_t color_s;
  for (int y = 0; y < cb_data_p->rows - 1; ++y)
  {
    glBegin (GL_TRIANGLE_STRIP);
    for (int x = 0; x < cb_data_p->columns; ++x)
    {
      if (cb_data_p->color)
      {
        color_s =
          Common_GL_Tools::toRGBColor (((cb_data_p->terrain[y * cb_data_p->rows + x] / static_cast<float> (cb_data_p->level)) + 1.0F) / 2.0);
        glColor3ub (color_s.r, color_s.g, color_s.b);
      } // end IF
      glVertex3f (x * cb_data_p->scaleFactor, y * cb_data_p->scaleFactor, cb_data_p->terrain[y * cb_data_p->rows + x]);
      if (cb_data_p->color)
      {
        color_s =
          Common_GL_Tools::toRGBColor (((cb_data_p->terrain[(y + 1) * cb_data_p->rows + x] / static_cast<float> (cb_data_p->level)) + 1.0F) / 2.0);
        glColor3ub (color_s.r, color_s.g, color_s.b);
      } // end IF
      glVertex3f (x * cb_data_p->scaleFactor, (y + 1) * cb_data_p->scaleFactor, cb_data_p->terrain[(y + 1) * cb_data_p->rows + x]);
    } // end FOR
    glEnd ();
  } // end FOR
  COMMON_GL_ASSERT;

  glFlush ();
  COMMON_GL_ASSERT;

  glutSwapBuffers ();
}

void
engine_glut_3_idle (void)
{
  //static float vel0 = -100.0;
  //static double t0 = -1.;
  //double t, dt;
  //t = glutGet (GLUT_ELAPSED_TIME) / 1000.;
  //if (t0 < 0.)
  //  t0 = t;
  //dt = t - t0;
  //t0 = t;

  //Zrot += Zstep * dt;

  //Xpos += Xvel * dt;
  //if (Xpos >= Xmax) {
  //  Xpos = Xmax;
  //  Xvel = -Xvel;
  //  Zstep = -Zstep;
  //}
  //if (Xpos <= Xmin) {
  //  Xpos = Xmin;
  //  Xvel = -Xvel;
  //  Zstep = -Zstep;
  //}
  //Ypos += Yvel * dt;
  //Yvel += G * dt;
  //if (Ypos < Ymin) {
  //  Ypos = Ymin;
  //  if (vel0 == -100.0)
  //    vel0 = fabs (Yvel);
  //  Yvel = vel0;
  //}
  glutPostRedisplay ();
}

void
engine_glut_3_visible (int vis)
{
  if (vis == GLUT_VISIBLE)
    glutIdleFunc (engine_glut_3_idle);
  else
    glutIdleFunc (NULL);
}

//////////////////////////////////////////

gboolean
idle_initialize_UI_cb (gpointer userData_in)
{
  // sanity check(s)
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_CBData*> (userData_in);
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
idle_finalize_UI_cb (gpointer userData_in)
{
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_UNUSED_ARG (ui_cb_data_p);

  return G_SOURCE_REMOVE;
}

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void
scale_frequency_value_changed_cb (GtkRange* range_in,
                                  gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->module.SetFrequency (gtk_range_get_value (range_in));
}

void
scale_octaves_value_changed_cb (GtkRange* range_in,
                                gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->module.SetOctaveCount (static_cast<int> (gtk_range_get_value (range_in)));
}

void
scale_persistence_value_changed_cb (GtkRange* range_in,
                                    gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->module.SetPersistence (gtk_range_get_value (range_in));
}

void
scale_lacunarity_value_changed_cb (GtkRange* range_in,
                                   gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->module.SetLacunarity (gtk_range_get_value (range_in));
}

void
scale_step_value_changed_cb (GtkRange* range_in,
                             gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->step = gtk_range_get_value (range_in);
}

void
scale_level_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->level =
    static_cast<int> (gtk_range_get_value (range_in));
}

void
scale_speed_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_ASSERT (ui_cb_data_p->GLUT_CBData);

  ui_cb_data_p->GLUT_CBData->yOffset = gtk_range_get_value (range_in);
}

void
button_reset_clicked_cb (GtkButton* button_in,
                         gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT(iterator != ui_cb_data_p->UIState->builders.end ());

  GtkScale* scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_FREQUENCY_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 1.0);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_OCTAVES_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 6.0);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_PERSISTENCE_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 0.5);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_LACUNARITY_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 2.5);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_STEP_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 0.05);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_LEVEL_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 50.0);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_SPEED_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), 0.03);
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
