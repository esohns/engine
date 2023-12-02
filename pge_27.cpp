#include "stdafx.h"

#include "pge_27.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_ui_gtk_manager_common.h"

#include "engine_common.h"

//////////////////////////////////////////

gboolean
idle_initialize_UI_27_cb (gpointer userData_in)
{
  // sanity check(s)
  struct Engine_UI_GTK_27_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_27_CBData*> (userData_in);
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
#if GTK_CHECK_VERSION (4,0,0)
  gtk_widget_show (dialog_p);
#else
  gtk_widget_show_all (dialog_p);
#endif // GTK_CHECK_VERSION(4,0,0)

  return G_SOURCE_REMOVE;
}

gboolean
idle_finalize_UI_27_cb (gpointer userData_in)
{
  struct Engine_UI_GTK_27_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_27_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  ACE_UNUSED_ARG (ui_cb_data_p);

  return G_SOURCE_REMOVE;
}

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void
scale_delta_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_27_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_27_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  ui_cb_data_p->delta =
    static_cast<int> (gtk_range_get_value (range_in));
}

void
scale_angle_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_27_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_27_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  ui_cb_data_p->angle =
    static_cast<int> (gtk_range_get_value (range_in));
}

void
button_reset_27_clicked_cb (GtkButton* button_in,
                            gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_27_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_27_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT(iterator != ui_cb_data_p->UIState->builders.end ());

  GtkScale* scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_DELTA_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_PGE_27_DEFAULT_DELTA);
  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_UI_GTK_SCALE_ANGLE_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_PGE_27_DEFAULT_ANGLE);
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

//////////////////////////////////////////

PGE_27::PGE_27 (const std::string& UIDefinitionFilePath_in)
 : olc::PixelGameEngine()
 , polygons_ ()
 , CBData_ ()
 , GTKConfiguration_ ()
 , GTKBuilderDefinition_ ()
{
  sAppName = "Example 27";

  GTKConfiguration_.argc = 0;
  GTKConfiguration_.argv = NULL;
  GTKConfiguration_.CBData = &CBData_;
  GTKConfiguration_.eventHooks.finiHook = idle_finalize_UI_27_cb;
  GTKConfiguration_.eventHooks.initHook = idle_initialize_UI_27_cb;
  GTKConfiguration_.definition = &GTKBuilderDefinition_;

  Common_UI_GTK_Manager_t* gtk_manager_p =
    COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
  ACE_ASSERT (gtk_manager_p);
  Common_UI_GTK_State_t& state_r =
    const_cast<Common_UI_GTK_State_t&> (gtk_manager_p->getR ());
  state_r.builders[ACE_TEXT_ALWAYS_CHAR(COMMON_UI_DEFINITION_DESCRIPTOR_MAIN)] =
    std::make_pair (UIDefinitionFilePath_in, static_cast<GtkBuilder*> (NULL));

  CBData_.UIState = &state_r;
  CBData_.progressData.state = &state_r;
  CBData_.delta = ENGINE_PGE_27_DEFAULT_DELTA;
  CBData_.angle = ENGINE_PGE_27_DEFAULT_ANGLE;

  bool result_2 = gtk_manager_p->initialize (GTKConfiguration_);
  if (!result_2)
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to Common_UI_GTK_Manager_T::initialize(), returning\n")));
    return;
  } // end IF
}

bool
PGE_27::OnUserCreate ()
{
  Common_UI_GTK_Manager_t* gtk_manager_p =
    COMMON_UI_GTK_MANAGER_SINGLETON::instance ();
  ACE_ASSERT (gtk_manager_p);
  gtk_manager_p->start ();
  if (!gtk_manager_p->isRunning ())
  {
    ACE_DEBUG ((LM_ERROR,
                ACE_TEXT ("failed to start GTK event dispatch, aborting\n")));
    return false;
  } // end IF

  int inc = 100;
  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += inc)
    for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y += inc)
    {
      polygon* polygon_p = new polygon (4);
      polygon_p->addVertex (x, y);
      polygon_p->addVertex (x + inc, y);
      polygon_p->addVertex (x + inc, y + inc);
      polygon_p->addVertex (x, y + inc);
      polygon_p->close ();
      polygons_.push_back (polygon_p);
    } // end FOR

  return true;
}

bool
PGE_27::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<polygon*>::iterator iterator = polygons_.begin ();
       iterator != polygons_.end ();
       ++iterator)
  {
    (*iterator)->hankin (CBData_.delta,
                         CBData_.angle);
    (*iterator)->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
