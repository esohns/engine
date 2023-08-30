#include "stdafx.h"

#include "pge_159.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_6.h"
#include "engine_common.h"

PGE_159::PGE_159 ()
 : olc::PixelGameEngine ()
 , index_ (0)
 , noise_ ()
 , particles_ ()
{
  sAppName = "Example 159";
}

bool
PGE_159::OnUserCreate ()
{
  for (int i = ENGINE_PGE_159_DEFAULT_NUMBER_OF_PARTICLES; i--;)
    particles_.push_back ({Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                           Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))});

  for (std::vector<olc::vf2d>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    for (int i = 50; i--;)
    {
      float d =
        Common_GL_Tools::map (static_cast<float> (noise_.GetValue ((*iterator).x / olc::PixelGameEngine::ScreenWidth () * 3.0, (*iterator).y / olc::PixelGameEngine::ScreenHeight () * 2.0, 0.0)),
                              -1.0f, 1.0f, 0.0f, 1.0f) * 20.0f;
      olc::vf2d v (3.0f, 0.0f);
      olc::vf2d rotated_v;
      rotated_v.x = std::cos (d) * v.x - std::sin (d) * v.y;
      rotated_v.y = std::sin (d) * v.x + std::cos (d) * v.y;
      (*iterator) += rotated_v;
      if ((*iterator).x < 0.0f) (*iterator).x += static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1);
      else if ((*iterator).x >= static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)) (*iterator).x -= static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1);
      if ((*iterator).y < 0.0f) (*iterator).y += static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1);
      else if ((*iterator).y >= static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1)) (*iterator).y -= static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1);
    } // end FOR

  olc::PixelGameEngine::Clear (olc::WHITE);

  return true;
}

bool
PGE_159::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    particles_.clear ();
    OnUserCreate ();
    index_ = 0;
    olc::PixelGameEngine::Clear (olc::WHITE);
  } // end IF

  if (index_ > static_cast<int32_t> (particles_.size ()))
    goto continue_;

  for (int32_t i = index_; i < index_ + ENGINE_PGE_159_DEFAULT_STEP; i++)
    for (int j = i + 1; j < ENGINE_PGE_159_DEFAULT_NUMBER_OF_PARTICLES; j++)
    {
      float d = (particles_[i].x - particles_[j].x) * (particles_[i].x - particles_[j].x) + (particles_[i].y - particles_[j].y) * (particles_[i].y - particles_[j].y);
      bool condition_b = true;
      for (std::vector<olc::vf2d>::iterator iterator = particles_.begin ();
           iterator != particles_.end ();
           ++iterator)
        if (d > (particles_[i].x - (*iterator).x) * (particles_[i].x - (*iterator).x) + (particles_[i].y - (*iterator).y) * (particles_[i].y - (*iterator).y) &&
            d > (particles_[j].x - (*iterator).x) * (particles_[j].x - (*iterator).x) + (particles_[j].y - (*iterator).y) * (particles_[j].y - (*iterator).y))
        {
          condition_b = false;
          break;
        } // end IF
      if (condition_b) //strokeWeight(log(A.dist(B)))
        olc::PixelGameEngine::DrawLine (particles_[i], particles_[j], olc::BLACK, 0xFFFFFFFF);
    } // end FOR

  index_ += ENGINE_PGE_159_DEFAULT_STEP;

continue_:
  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
