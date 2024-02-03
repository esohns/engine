#include "stdafx.h"

#include "pge_339.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_15.h"

#include "engine_common.h"

PGE_339::PGE_339 ()
 : olc::PixelGameEngine ()
 , prev_ ()
 , t_ (0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 339");
}

bool
PGE_339::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_339_DEFAULT_PALETTE_SIZE; i++)
    prev_.push_back (olc::vf2d (0.0f, 0.0f));

  return true;
}

bool
PGE_339::OnUserUpdate (float fElapsedTime)
{
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  //olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a >= ENGINE_PGE_339_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_339_DEFAULT_ALPHA_DECAY : 0);

  static olc::Pixel pallet_a[ENGINE_PGE_339_DEFAULT_PALETTE_SIZE] =
    {olc::Pixel ( 23,  22,  20),
     olc::Pixel ( 25, 103, 116),
     olc::Pixel (144, 161, 157),
     olc::Pixel ( 38, 240,  31)};

  olc::vf2d coordinate (0.0f, 0.0f);
  for (int i = 1; i < ENGINE_PGE_339_DEFAULT_PALETTE_SIZE; i++)
  {
    float radius = ENGINE_PGE_339_DEFAULT_OUTER_RADIUS / static_cast<float> (i);
    float period =
      10.0f * (ENGINE_PGE_339_DEFAULT_PALETTE_SIZE - i) * (ENGINE_PGE_339_DEFAULT_PALETTE_SIZE - i);
    olc::vf2d inner = move (coordinate, radius, t_ * static_cast<float> (M_PI) / period);

    olc::Pixel color = pallet_a[i];
    color.a = 120;
    olc::PixelGameEngine::DrawLine (center_s + coordinate, center_s + inner, color, 0xFFFFFFFF);
    coordinate = inner;

    if (t_ != 0)
    {
      color.a = 152;
      olc::PixelGameEngine::DrawLine (center_s + prev_[i], center_s + coordinate, color, 0xFFFFFFFF);
    } // end IF

    prev_[i] = coordinate;
  } // end FOR

  ++t_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

olc::vf2d
PGE_339::move (olc::vf2d& origin, float radius, float angle)
{
  return olc::vf2d (std::cos (angle) * radius + origin.x, std::sin (angle) * radius + origin.y);
}
