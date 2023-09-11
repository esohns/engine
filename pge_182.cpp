#include "stdafx.h"

#include "pge_182.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_8.h"
#include "engine_common.h"

PGE_182::PGE_182 ()
 : olc::PixelGameEngine ()
 , blackHoles_ ()
 , stars_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 182");
}

bool
PGE_182::OnUserCreate ()
{
  for (int i = ENGINE_PGE_182_DEFAULT_NUMBER_OF_HOLES; i--;)
    blackHoles_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                            Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});
  for (int i = ENGINE_PGE_182_DEFAULT_NUMBER_OF_STARS; i--;)
    stars_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                       Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});

  return true;
}

bool
PGE_182::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  if (frame_count_i % ENGINE_PGE_182_DEFAULT_SHIFT_INTERVAL == 0)
  {
    blackHoles_.pop_front ();
    blackHoles_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                            Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});
  } // end IF

  for (std::vector<olc::vf2d>::iterator iterator = stars_.begin ();
       iterator != stars_.end ();
       ++iterator)
  {
    float direction_f = 0.0f;
    for (std::list<olc::vf2d>::iterator iterator_2 = blackHoles_.begin();
         iterator_2 != blackHoles_.end();
         ++iterator_2)
    {
      float dx = (*iterator).x - (*iterator_2).x;
      float dy = (*iterator).y - (*iterator_2).y;
      if (dx * dx + dy * dy > 1.0f)
        direction_f += std::atan2 (dy, dx);
      else
      {
        float angle_f = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
        (*iterator).x = olc::PixelGameEngine::ScreenWidth ()  / 2.0f + std::cos (angle_f) *  olc::PixelGameEngine::ScreenWidth () / 3.0f;
        (*iterator).y = olc::PixelGameEngine::ScreenHeight () / 2.0f + std::sin (angle_f) * olc::PixelGameEngine::ScreenHeight () / 3.0f;
        break;
      } // end ELSE
    } // end FOR
    float offset_x = std::cos (direction_f) * ENGINE_PGE_182_DEFAULT_SPEED;
    float offset_y = std::sin (direction_f) * ENGINE_PGE_182_DEFAULT_SPEED;
    olc::PixelGameEngine::DrawLine (static_cast<int32_t> ((*iterator).x), static_cast<int32_t> ((*iterator).y),
                                    static_cast<int32_t> ((*iterator).x + offset_x), static_cast<int32_t> ((*iterator).y + offset_y),
                                    olc::WHITE, 0xFFFFFFFF);
    (*iterator).x += offset_x;
    (*iterator).y += offset_y;
  } // end FOR

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
