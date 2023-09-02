#include "stdafx.h"

#include "pge_167.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_167::PGE_167 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 167");
}

bool
PGE_167::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_167_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (particle (this));

  return true;
}

bool
PGE_167::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  int32_t f = 0;
  for (int32_t d = ENGINE_PGE_167_DEFAULT_RANGE; d > 6; d -= ENGINE_PGE_167_DEFAULT_SIZE * 2, f = 1 - f)
  {
    olc::Pixel color (f * 255, f * 255, f * 255, 255);
    for (std::vector<particle>::iterator iterator = particles_.begin ();
         iterator != particles_.end ();
         ++iterator)
    {
      (*iterator).position_.x += (*iterator).velocity_.x;
      (*iterator).position_.y += (*iterator).velocity_.y;
      if ((*iterator).position_.x < 0.0f || (*iterator).position_.x > ENGINE_PGE_167_DEFAULT_W) (*iterator).velocity_.x = -(*iterator).velocity_.x;
      if ((*iterator).position_.y < 0.0f || (*iterator).position_.y > ENGINE_PGE_167_DEFAULT_W) (*iterator).velocity_.y = -(*iterator).velocity_.y;
      olc::PixelGameEngine::FillCircle ((*iterator).position_, d / 2, color);
    } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
