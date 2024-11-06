#include "stdafx.h"

#include "pge_397.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_18.h"

PGE_397::PGE_397 ()
 : olc::PixelGameEngine ()
 , points_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 397");
}

bool
PGE_397::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_397_DEFAULT_NUMBER_OF_POINTS; i++)
    points_.push_back ({0, 0});

  return true;
}

bool
PGE_397::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_397_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_397_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);

    points_.clear ();
    for (int i = 0; i < ENGINE_PGE_397_DEFAULT_NUMBER_OF_POINTS; i++)
      points_.push_back ({0, 0});
  } // end IF

  static int W = olc::PixelGameEngine::ScreenWidth ();
  static int H = olc::PixelGameEngine::ScreenHeight ();
  static float half_width_f = olc::PixelGameEngine::ScreenWidth () / 2.0f;
  static float half_height_f = olc::PixelGameEngine::ScreenHeight () / 2.0f;
  static olc::vf2d center_s (half_width_f, half_height_f);
  for (int i = 99; i--;)
    points_[t_++ % ENGINE_PGE_397_DEFAULT_NUMBER_OF_POINTS] = { static_cast<float> (t_ % W), static_cast<float> (Common_Tools::getRandomNumber (0, H - 1)) };

  for (std::vector<olc::vf2d>::iterator iterator = points_.begin ();
       iterator != points_.end ();
       ++iterator)
  {
    (*iterator).x += F ((*iterator).y, 29.0f);
    (*iterator).y +=
      F ((*iterator).x, 29.0f, std::pow (std::sin (center_s.dist (*iterator) / 99.0f - t_ / 2e4f), 8.0f) * static_cast<float> (H));

    olc::PixelGameEngine::Draw (*iterator, olc::WHITE);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
