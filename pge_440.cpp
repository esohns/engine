#include "stdafx.h"

#include "pge_440.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_math_tools.h"

#include "common_ui_defines.h"

#include "engine_common.h"

#include "defines_20.h"

//////////////////////////////////////////

gboolean
idle_initialize_UI_440_cb (gpointer userData_in)
{
  // sanity check(s)
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT (iterator != ui_cb_data_p->UIState->builders.end ());

  GtkWidget* dialog_p =
    GTK_WIDGET (gtk_builder_get_object ((*iterator).second.second,
                                        ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_440_GTK_DIALOG_MAIN_NAME)));
  ACE_ASSERT (dialog_p);
 
  // step2: (auto-)connect signals/slots
#if GTK_CHECK_VERSION (4,0,0)
#else
  gtk_builder_connect_signals ((*iterator).second.second,
                               ui_cb_data_p);
#endif // GTK_CHECK_VERSION (4,0,0)

  // step6a: connect default signals
#if GTK_CHECK_VERSION (4,0,0)
#else
  gulong result_2 =
      g_signal_connect (dialog_p,
                        ACE_TEXT_ALWAYS_CHAR ("destroy"),
                        G_CALLBACK (gtk_widget_destroyed),
                        &dialog_p);
  ACE_ASSERT (result_2);
#endif // GTK_CHECK_VERSION (4,0,0)

  // step9: draw main dialog
#if GTK_CHECK_VERSION (4,0,0)
  gtk_widget_show (dialog_p);
#else
  gtk_widget_show_all (dialog_p);
#endif // GTK_CHECK_VERSION (4,0,0)

  return G_SOURCE_REMOVE;
}

gboolean
idle_finalize_UI_440_cb (gpointer userData_in)
{
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    static_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
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
scale_alpha_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  *ui_cb_data_p->alpha = static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_beta_value_changed_cb (GtkRange* range_in,
                             gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  *ui_cb_data_p->beta = static_cast<float> (gtk_range_get_value (range_in));
}

void
scale_gamma_value_changed_cb (GtkRange* range_in,
                              gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (range_in);
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  *ui_cb_data_p->gamma = static_cast<float> (gtk_range_get_value (range_in));
}

void
button_reset_clicked_cb (GtkButton* button_in,
                         gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);
  Common_UI_GTK_BuildersIterator_t iterator =
    ui_cb_data_p->UIState->builders.find (ACE_TEXT_ALWAYS_CHAR (COMMON_UI_DEFINITION_DESCRIPTOR_MAIN));
  ACE_ASSERT(iterator != ui_cb_data_p->UIState->builders.end ());

  GtkScale* scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_440_GTK_SCALE_ALPHA_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_PGE_440_DEFAULT_ALPHA);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_440_GTK_SCALE_BETA_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_PGE_440_DEFAULT_BETA);

  scale_p =
    GTK_SCALE (gtk_builder_get_object ((*iterator).second.second,
                                       ACE_TEXT_ALWAYS_CHAR (ENGINE_PGE_440_GTK_SCALE_GAMMA_NAME)));
  ACE_ASSERT (scale_p);
  gtk_range_set_value (GTK_RANGE (scale_p), ENGINE_PGE_440_DEFAULT_GAMMA);
}

void
button_clear_clicked_cb (GtkButton* button_in,
                         gpointer userData_in)
{
  // sanity check(s)
  ACE_ASSERT (button_in);
  struct Engine_UI_GTK_440_CBData* ui_cb_data_p =
    reinterpret_cast<struct Engine_UI_GTK_440_CBData*> (userData_in);
  ACE_ASSERT (ui_cb_data_p);

  ui_cb_data_p->clearScreen = true;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */

//////////////////////////////////////////

PGE_440::PGE_440 (struct Engine_UI_GTK_440_CBData& CBData_in)
 : olc::PixelGameEngine ()
 , a_ (), b_ (), c_ ()
 , columns_ (0), rows_ (0)
 , size_ (ENGINE_PGE_440_DEFAULT_SIZE)
 , p_ (0), q_ (1)
 , alpha_ (ENGINE_PGE_440_DEFAULT_ALPHA), beta_ (ENGINE_PGE_440_DEFAULT_BETA), gamma_ (ENGINE_PGE_440_DEFAULT_GAMMA)
 , CBData_ (&CBData_in)
{
  sAppName = "Example 440";

  CBData_->alpha = &alpha_;
  CBData_->beta = &beta_;
  CBData_->gamma = &gamma_;
}

bool
PGE_440::OnUserCreate ()
{
  columns_ = olc::PixelGameEngine::ScreenWidth () / size_;
  rows_ = olc::PixelGameEngine::ScreenHeight () / size_;

  std::vector<float> values_a (2, 0.0f);
  std::vector<std::vector<float> > row_a (rows_, values_a);
  a_.assign (columns_, row_a);
  b_.assign (columns_, row_a);
  c_.assign (columns_, row_a);
  reset ();

  return true;
}

bool
PGE_440::OnUserUpdate (float fElapsedTime)
{
  if (CBData_->clearScreen)
  {
    CBData_->clearScreen = false;
    olc::PixelGameEngine::Clear (olc::BLACK);
    reset ();
  } // end IF

  for (int x = 0; x < columns_; x++)
    for (int y = 0; y < rows_; y++)
    {
      float c_a = 0.0f;
      float c_b = 0.0f;
      float c_c = 0.0f;

      for (int i = x - 1; i <= x + 1; i++)
        for (int j = y - 1; j <= y + 1; j++)
        {
          int xIndex = (i + columns_) % columns_;
          int yIndex = (j + rows_) % rows_;
          c_a += a_[xIndex][yIndex][p_];
          c_b += b_[xIndex][yIndex][p_];
          c_c += c_[xIndex][yIndex][p_];
        } // end FOR

      c_a /= 9.0f;
      c_b /= 9.0f;
      c_c /= 9.0f;

      a_[x][y][q_] = std::max (std::min (c_a + c_a * (alpha_ * c_b - gamma_ * c_c), 1.0f), 0.0f);
      b_[x][y][q_] = std::max (std::min (c_b + c_b * (beta_  * c_c - alpha_ * c_a), 1.0f), 0.0f);
      c_[x][y][q_] = std::max (std::min (c_c + c_c * (gamma_ * c_a - beta_  * c_b), 1.0f), 0.0f);
    } // end FOR

  int temp = p_; p_ = q_; q_ = temp;

  for (int x = 0; x < columns_; x++)
    for (int y = 0; y < rows_; y++)
    {
      float val = a_[x][y][p_];
      uint8_t r = static_cast<uint8_t> (Common_Math_Tools::lerp (255.0f, 150.0f, val));
      uint8_t g = static_cast<uint8_t> (Common_Math_Tools::lerp (100.0f, 210.0f, val));
      uint8_t b = static_cast<uint8_t> (Common_Math_Tools::lerp (0.0f, 255.0f, val));
      olc::Pixel color (r, g, b, 255U);
      olc::PixelGameEngine::FillRect (x * size_, y * size_, size_, size_, color);
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_440::reset ()
{
  for (int x = 0; x < columns_; x++)
    for (int y = 0; y < rows_; y++)
    {
      a_[x][y] = {Common_Tools::getRandomNumber (0.0f, 1.0f), Common_Tools::getRandomNumber (0.0f, 1.0f)};
      b_[x][y] = {Common_Tools::getRandomNumber (0.0f, 1.0f), Common_Tools::getRandomNumber (0.0f, 1.0f)};
      c_[x][y] = {Common_Tools::getRandomNumber (0.0f, 1.0f), Common_Tools::getRandomNumber (0.0f, 1.0f)};
    } // end FOR
}
