#include "stdafx.h"

#include "pge_266.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_266::PGE_266 ()
 : olc::PixelGameEngine ()
 , x_ ()
 , increasing_ (false)
 , count_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 266");
}

bool
PGE_266::OnUserCreate ()
{
  increasing_ = true;
  count_ = ENGINE_PGE_266_DEFAULT_MIN;
  x_.push_back (1.0f);
  for (int i = 1; i < ENGINE_PGE_266_DEFAULT_STEPS / 2; i++)
    x_.push_back (0.0f);

  return true;
}

bool
PGE_266::OnUserUpdate (float fElapsedTime)
{
  static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  olc::PixelGameEngine::Clear (olc::BLACK);

  float factor = createCounter (ENGINE_PGE_266_DEFAULT_MIN, ENGINE_PGE_266_DEFAULT_MAX);
  for (int i = 1; i < ENGINE_PGE_266_DEFAULT_STEPS / 2; i++)
    x_[i] = x_[i - 1] * factor;

  std::vector<int32_t> x_coordinates_a;
  for (std::vector<float>::iterator iterator = x_.begin ();
       iterator != x_.end ();
       ++iterator)
    x_coordinates_a.push_back (static_cast<int32_t> (Common_GL_Tools::map (*iterator, x_[0], x_[ENGINE_PGE_266_DEFAULT_STEPS / 2 - 1], olc::PixelGameEngine::ScreenWidth () / 2.0f, 0.1f)));
  std::vector<int32_t> x_coordinates_reverse_a = x_coordinates_a;
  std::reverse (x_coordinates_reverse_a.begin (), x_coordinates_reverse_a.end ());
  for (int i = 0; i < static_cast<int> (x_coordinates_a.size ()); i++)
  {
    olc::PixelGameEngine::DrawLine (x_coordinates_a[i], 0, half_width_i - x_coordinates_reverse_a[i], half_height_i, olc::WHITE, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (x_coordinates_a[i], olc::PixelGameEngine::ScreenHeight (), half_width_i - x_coordinates_reverse_a[i], half_height_i, olc::WHITE, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (olc::PixelGameEngine::ScreenWidth () - x_coordinates_reverse_a[i], 0, x_coordinates_a[i] + half_width_i, half_height_i, olc::WHITE, 0xFFFFFFFF);
    olc::PixelGameEngine::DrawLine (olc::PixelGameEngine::ScreenWidth () - x_coordinates_reverse_a[i], olc::PixelGameEngine::ScreenHeight (), x_coordinates_a[i] + half_width_i, half_height_i, olc::WHITE, 0xFFFFFFFF);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

float
PGE_266::createCounter (float min, float max)
{
  if (increasing_)
  {
    count_ += 0.0004f;
    if (count_ >= max)
      increasing_ = false;
  } // end IF
  else
  {
    count_ -= 0.0004f;
    if (count_ <= min)
      increasing_ = true;
  } // end ELSE

  return count_;
}
