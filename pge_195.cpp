#include "stdafx.h"

#include "pge_195.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_195::PGE_195 ()
 : olc::PixelGameEngine ()
 , rx_ ()
 , ry_ ()
 , xx_ ()
 , yy_ ()
 , mw_ (ENGINE_PGE_195_DEFAULT_MW)
 , t_ (0)
 , fpx_ (0.0f)
 , fpy_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 195");
}

bool
PGE_195::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_195_DEFAULT_CO; i += 1)
  {
    rx_.push_back (Common_Tools::getRandomNumber (0.0f, 1.0f));
    ry_.push_back (Common_Tools::getRandomNumber (0.0f, 1.0f));
  } // end FOR

  return true;
}

void
PGE_195::restart ()
{
  xx_.clear ();
  yy_.clear ();

  for (int i = 1; i <= ENGINE_PGE_195_DEFAULT_CO; i += 1)
  {
    xx_.push_back (rx_[i - 1] * i * 1e7f * mw_);
    yy_.push_back (ry_[i - 1] * i * 1e7f * mw_);
  } // end FOR

  t_ = 0;
}

bool
PGE_195::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;
  //static int32_t prev_mouse_x = 0, prev_mouse_y = 0;

  //if (prev_mouse_x != olc::PixelGameEngine::GetMouseX ())
  //{
  //  prev_mouse_x = olc::PixelGameEngine::GetMouseX ();
  //  fpx_ = (0.5f - olc::PixelGameEngine::GetMouseX () / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) * 2;
  //  restart ();
  //} // end IF
  //if (prev_mouse_y != olc::PixelGameEngine::GetMouseY ())
  //{
  //  prev_mouse_y = olc::PixelGameEngine::GetMouseY ();
  //  fpy_ = (0.5f - olc::PixelGameEngine::GetMouseY () / static_cast<float> (olc::PixelGameEngine::ScreenHeight ())) * 2;
  //  restart ();
  //} // end IF

  //if (olc::PixelGameEngine::GetMouse (0).bPressed)
    restart ();

  mw_ += 0.01f;

  t_++;

  olc::PixelGameEngine::Clear (olc::BLACK);

  int params[8] = {1, 0, 1, 31, 1, 8 + (static_cast<int> (frame_count_i / 50) % 4), 1, 1}; // 6 -> 4
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth () * 2; i++)
    for (int j = 0; j < ENGINE_PGE_195_DEFAULT_CO; j++)
    {
      static float a = 0.5001f;
      static float b = a * 2.0f;
      xx_[j] = xx_[j] - (yy_[j] / 300.0f / 2.0f) - (yy_[j] * a);
      yy_[j] = yy_[j] - (xx_[j] / 2.0f / 2.0f) + (xx_[j] * b - (static_cast<int> (yy_[j]) >> params[3])) + (static_cast<int> (xx_[j] * (4.5f + static_cast<float> (t_) / 100000.0f / static_cast<float> (t_))) >> params[5]);
      xx_[j] = xx_[j] - (yy_[j] / 300.0f / 2.0f) - (yy_[j] * a);

      float fx = (xx_[j] / 1e7f - fpx_ * olc::PixelGameEngine::ScreenWidth ()  / 4.0f) / 0.75f;
      float fy = (yy_[j] / 1e7f - fpy_ * olc::PixelGameEngine::ScreenHeight () / 4.0f) / 0.75f;

      int px = static_cast<int> ((fx + olc::PixelGameEngine::ScreenWidth ()  / 2));
      int py = static_cast<int> ((fy + olc::PixelGameEngine::ScreenHeight () / 2));
      if (px < 0 || px >= olc::PixelGameEngine::ScreenWidth ()) continue;
      if (py < 0 || py >= olc::PixelGameEngine::ScreenHeight ()) continue;
      int index1 = px + (py * olc::PixelGameEngine::ScreenWidth ());

      float nj = (j / static_cast<float> (ENGINE_PGE_195_DEFAULT_CO)) * mw_;
      unsigned int c = static_cast<unsigned int> (4.0f * nj); // (i|xx) // (i/xx)

      unsigned int r   = p[index1].r + (c ^ (static_cast<int> (nj * 300) >> 6) + (static_cast<int> (t_ / 2.0f) % 32));
      unsigned int g   = p[index1].g + (c ^ (static_cast<int> (nj * 300) >> 5));
      unsigned int b_2 = p[index1].b + (c ^ (static_cast<int> (nj * 300) >> 3) + (static_cast<int> (t_ / 2.0f) % 32));
      p[index1].r = r   > 255 ? 255 : static_cast<uint8_t> (r);
      p[index1].g = g   > 255 ? 255 : static_cast<uint8_t> (g);
      p[index1].b = b_2 > 255 ? 255 : static_cast<uint8_t> (b_2);
      //p[index1].a = 255;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
