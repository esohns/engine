#include "stdafx.h"

#include "pge_225.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_225::PGE_225 ()
 : olc::PixelGameEngine ()
 , blocks_ ()
 , movers_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 225");
}

bool
PGE_225::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_225_DEFAULT_NUMBER_OF_MOVERS; i++)
    movers_.push_back (mover (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                              Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

  float w = olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_225_DEFAULT_N);
  for (int i = 0; i < ENGINE_PGE_225_DEFAULT_N; i++)
    for (int j = 0; j < ENGINE_PGE_225_DEFAULT_N; j++)
    {
      struct block block_s;
      block_s.position.x = i * w + w / 2.0f;
      block_s.position.y = j * w + w / 2.0f;
      block_s.w = w * 0.75f;
      blocks_.push_back (block_s);
    } // end FOR

  return true;
}

bool
PGE_225::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  olc::Pixel color;
  for (std::vector<struct block>::iterator iterator = blocks_.begin ();
       iterator != blocks_.end ();
       ++iterator)
  {
    color = olc::WHITE;
    float off = 0.0f;

    for (std::vector<mover>::iterator iterator_2 = movers_.begin ();
         iterator_2 != movers_.end ();
         ++iterator_2)
    {
      float dst = (*iterator).position.dist ((*iterator_2).position_);
      if (dst < ((*iterator_2).d_ / 2.0f) + 10.0f && dst > ((*iterator_2).d_ / 2.0f) - 10.0f)
      {
        off = (*iterator).w;
        color = (*iterator_2).color_;
      } // end IF
    } // end FOR
    olc::PixelGameEngine::FillRect (static_cast<int32_t> ((*iterator).position.x), static_cast<int32_t> ((*iterator).position.y - off),
                                    static_cast<int32_t> ((*iterator).w), static_cast<int32_t> ((*iterator).w),
                                    color);
  } // end FOR

  for (std::vector<mover>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
    (*iterator).move (this);

  for (int i = static_cast<int> (movers_.size () - 1); i >= 0; i--)
    if (movers_[i].dead_)
    {
      std::vector<mover>::iterator iterator = movers_.begin ();
      std::advance (iterator, i);
      movers_.erase (iterator);
      movers_.push_back (mover (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                                Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));
    } // end IF

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
