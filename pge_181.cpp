#include "stdafx.h"

#include "pge_181.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_181::PGE_181 ()
 : olc::PixelGameEngine ()
 , blackHoles_ (3, olc::vf2d ())
 , stars_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 181");
}

bool
PGE_181::OnUserCreate ()
{
  blackHoles_[0] = {0.0f, 0.0f};

  for (int i = ENGINE_PGE_181_DEFAULT_NUMBER_OF_STARS; i--;)
    stars_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                       Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});

  return true;
}

bool
PGE_181::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (int i = 3; i--;)
  {
    float degree_f = frame_count_i / 300.0f * (i + 1);
    blackHoles_[i] = {std::cos (degree_f) * 200.0f + 400.0f,
                      std::sin (degree_f) * 200.0f + 300.0f};
  } // end FOR

  for (std::vector<olc::vf2d>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
  {
    float direction_f = 0.0f;
    for (std::vector<olc::vf2d>::iterator iterator_2 = blackHoles_.begin ();
         iterator_2 != blackHoles_.end ();
         ++iterator_2)
    {
      float dx = (*iterator).x - (*iterator_2).x;
      float dy = (*iterator).y - (*iterator_2).y;
      if (dx * dx + dy * dy > 1.0f)
        direction_f += std::atan2 (dy, dx) * 4.0f;
      else
      {
        (*iterator).x =
          Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1));
        (*iterator).y =
          static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1) - 100.0f;
        break;
      } // end ELSE
    } // end FOR
    float offset_x = std::cos (direction_f) * ENGINE_PGE_181_DEFAULT_SPEED;
    float offset_y = std::sin (direction_f) * ENGINE_PGE_181_DEFAULT_SPEED;
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> ((*iterator).x), static_cast<int32_t> ((*iterator).y),
                                    static_cast<int32_t> ((*iterator).x + offset_x), static_cast<int32_t> ((*iterator).y + offset_y),
                                    olc::WHITE, 0xFFFFFFFF);
    (*iterator).x += offset_x;
    (*iterator).y += offset_y;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
