#include "stdafx.h"

#include "pge_210.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_210::PGE_210 ()
 : olc::PixelGameEngine ()
 , position_ (0.0f, 0.0f)
 //, previous_ (0.0f, 0.0f)
 , s_ (0.5f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 210");
}

bool
PGE_210::OnUserCreate ()
{
  return true;
}

bool
PGE_210::OnUserUpdate (float fElapsedTime)
{
  olc::Pixel color = olc::BLACK;
  for (int i = 0; i < 10000; i++)
  {
    color.r = static_cast<uint8_t> (std::max (std::min (s_ + 0.2f, 1.0f), 0.0f) * 255.0f);
    color.g = color.r;

    olc::PixelGameEngine::Draw (static_cast<int32_t> (Common_GL_Tools::map (position_.x, -0.26f, 1.1f, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1))),
                                (olc::PixelGameEngine::ScreenHeight () - 1) - static_cast<int32_t> (Common_GL_Tools::map (position_.y, -0.25f, 0.6f, 0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))),
                                color);

    //previous_ = position_;
    if (Common_Tools::testRandomProbability (0.5f))
    {
      s_ = 1.0f - s_ / 2.0f;
      float tx = position_.x;
      position_.x = 0.62367f * position_.x - 0.40337f * position_.y;
      position_.y = 0.40337f * tx + 0.62367f * position_.y;
    } // end IF
    else
    {
      s_ = s_ / 2.0f;
      float tx = position_.x;
      position_.x = -0.37633f * position_.x - 0.40337f * position_.y + 1.0f;
      position_.y = 0.40337f * tx + -0.37633f * position_.y;
    } // end ELSE
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
