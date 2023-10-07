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
  //olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_195_DEFAULT_CO; i += 1)
  {
    rx_.push_back (Common_Tools::getRandomNumber (0.0f, 1.0f));
    ry_.push_back (Common_Tools::getRandomNumber (0.0f, 1.0f));
  } // end FOR

  restart ();

  return true;
}

void
PGE_195::restart ()
{
  xx_.clear ();
  yy_.clear ();

  for (int i = 0; i < ENGINE_PGE_195_DEFAULT_CO; i += 1)
  {
    xx_.push_back (rx_[i] * i * 1e7f * mw_);
    yy_.push_back (ry_[i] * i * 1e7f * mw_);
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

  int params[8] = {1, 0, 1, 31, 1, 8 + (static_cast<int> (frame_count_i / 50.0f) % 4), 1, 1}; // 6 -> 4
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth () * 2; i++)
    for (int j = 0; j < ENGINE_PGE_195_DEFAULT_CO; j++)
    {
      static float a = 0.5001f;
      static float b = a * 2.0f;
      xx_[j] = xx_[j] - (static_cast<int> (yy_[j] / 300.0f) >> params[0]) - (yy_[j] * a);
      yy_[j] = yy_[j] - (static_cast<int> (xx_[j] / 2.0f) >> params[2]) + (xx_[j] * b - (static_cast<int> (yy_[j]) >> params[3])) + (static_cast<int> (xx_[j] * (4.5f + t_ / 100000.0f / static_cast<float> (t_))) >> params[5]);
      xx_[j] = xx_[j] - (static_cast<int> (yy_[j] / 300.0f) >> params[6]) - (yy_[j] * a);

      float fx = (xx_[j] / 1e7f - fpx_ * olc::PixelGameEngine::ScreenWidth ()  / 4.0f) / 0.75f;
      float fy = (yy_[j] / 1e7f - fpy_ * olc::PixelGameEngine::ScreenHeight () / 4.0f) / 0.75f;

      int px = static_cast<int> ((fx + olc::PixelGameEngine::ScreenWidth ()  / 2));
      int py = static_cast<int> ((fy + olc::PixelGameEngine::ScreenHeight () / 2));
      if (px < 0 || px >= olc::PixelGameEngine::ScreenWidth ()) continue;
      if (py < 0 || py >= olc::PixelGameEngine::ScreenHeight ()) continue;
      int index1 = px + (py * olc::PixelGameEngine::ScreenWidth ());

      float nj = (j / static_cast<float> (ENGINE_PGE_195_DEFAULT_CO)) * mw_;
      int c = static_cast<int> (4.0f * nj); // (i|xx) // (i/xx)

      p[index1].r += c ^ (static_cast<int> ((static_cast<int> (nj * 300) >> 6) + t_ / 2.0f) % 32);
      p[index1].g += c ^ (static_cast<int> (nj * 300) >> 5);
      p[index1].b += c ^ (static_cast<int> ((static_cast<int> (nj * 300) >> 3) + t_ / 2.0f) % 32);
      //p[index1].a = 255;
    } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
