#include "stdafx.h"

#include "pge_97.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_97::PGE_97 ()
 : olc::PixelGameEngine ()
 , neighbours_ ()
 , particles_ ()
 , springs_ ()
 , columns_ (0)
 , rows_ (0)
 , spacing_ (0.0f)
 , baseR_ (0.0f)
 , maxR_ (0.0f)
{
  sAppName = "Example 97";
}

bool
PGE_97::OnUserCreate ()
{
  if (olc::PixelGameEngine::ScreenWidth () < olc::PixelGameEngine::ScreenHeight ())
  {
    spacing_ = olc::PixelGameEngine::ScreenWidth () * 0.08f;
    columns_ =
      static_cast<int32_t> (std::floor (olc::PixelGameEngine::ScreenWidth () / spacing_)) + 1;
    rows_ =
      static_cast<int32_t> (std::floor (olc::PixelGameEngine::ScreenHeight () / spacing_)) + 2;
  } // end IF
  else
  {
    spacing_ = olc::PixelGameEngine::ScreenHeight () * 0.08f;
    columns_ =
      static_cast<int32_t> (std::floor (olc::PixelGameEngine::ScreenWidth () / spacing_)) + 2;
    rows_ =
      static_cast<int32_t> (std::floor (olc::PixelGameEngine::ScreenHeight () / spacing_)) + 1;
  } // end ELSE
  baseR_ = spacing_ * 0.2f;
  maxR_ = spacing_ * 0.8f;

  neighbours_.push_back ({1, 0});
  neighbours_.push_back({1, 1});
  neighbours_.push_back({0, 1});
  neighbours_.push_back({-1, 0});
  neighbours_.push_back({-1, -1});
  neighbours_.push_back({0, -1});
  neighbours_.push_back({1, -1});
  neighbours_.push_back({-1, 1});

  for (int32_t j = 0; j < rows_; j++)
    for (int32_t i = 0; i < columns_; i++)
    {
      particles_.push_back (new particle (i * spacing_, j * spacing_, i, j, baseR_));

      if (i == 0 || j == 0 || i == columns_ - 1 || j == rows_ - 1)
        particles_[i + j * columns_]->locked_ = true;

      if (i != 0)
      {
        particle* a = particles_[i + j * columns_];
        particle* b = particles_[i - 1 + j * columns_];
        springs_.push_back (new spring (ENGINE_PGE_97_DEFAULT_K, static_cast<int32_t> (spacing_), *a, *b));
        if (j != 0)
        {
          particle* c = particles_[i + j * columns_];
          particle* d = particles_[i + (j - 1) * columns_];
          springs_.push_back (new spring (ENGINE_PGE_97_DEFAULT_K, static_cast<int32_t> (spacing_), *c, *d));
        } // end IF
      } // end IF
    } // end FOR

  return true;
}

bool
PGE_97::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<spring*>::iterator iterator = springs_.begin ();
       iterator != springs_.end ();
       ++iterator)
  {
    (*iterator)->update ();
    // (*iterator)->show ();
  } // end FOR

  for (std::vector<particle*>::iterator iterator = particles_.begin();
       iterator != particles_.end();
       ++iterator)
  {
    if (!(*iterator)->locked_)
    {
      (*iterator)->show (this);
      (*iterator)->update (baseR_, maxR_);

      (*iterator)->applyAttRep (neighbours_, particles_, columns_, baseR_, maxR_, spacing_);
    } // end IF
  } // end FOR

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    particle* active = NULL;
    olc::vf2d mouse_point =
      {static_cast<float> (olc::PixelGameEngine::GetMouseX ()), static_cast<float> (olc::PixelGameEngine::GetMouseY ())};
    for (std::vector<particle*>::iterator iterator = particles_.begin();
         iterator != particles_.end();
         ++iterator)
    {
      float distance_f = (*iterator)->position_.dist (mouse_point);
      if (distance_f < static_cast<float> ((*iterator)->radius_))
      {
        active = *iterator;
        (*iterator)->active_ = true;
        break;
      } // end IF
    } // end FOR
    if (active)
    {
      active->velocity_ *= 0.0f;
      active->targetR_ = maxR_;
    } // end IF
  } // end IF
  else
  {
    for (std::vector<particle*>::iterator iterator = particles_.begin();
         iterator != particles_.end();
         ++iterator)
    {
      (*iterator)->active_ = false;
      (*iterator)->targetR_ = baseR_;
    } // end FOR
  } // end ELSE
  
  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
