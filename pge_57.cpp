#include "stdafx.h"

#include "pge_57.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_57::PGE_57 ()
 : olc::PixelGameEngine ()
 , color_ ()
 , particles_ ()
 , viscosity_ (ENGINE_PGE_57_DEFAULT_VISCOSITY)
{
  sAppName = "Example 57";
}

PGE_57::~PGE_57 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_57::OnUserCreate ()
{
  return true;
}

bool
PGE_57::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  color_ = {static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
            static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
            static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
            255};

  if (olc::PixelGameEngine::GetMouse (0).bPressed ||
      olc::PixelGameEngine::GetMouse (0).bHeld)
    particles_.push_back (new particle (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY (), color_));
  if ((olc::PixelGameEngine::GetMouse (1).bPressed || olc::PixelGameEngine::GetMouse (1).bHeld) &&
      !particles_.empty ())
  {
    std::random_device rd;
    std::mt19937 g (rd ());
    std::shuffle (particles_.begin (), particles_.end (), g);
    delete *particles_.begin ();
    particles_.erase (particles_.begin ());
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::V).bPressed)
  {
    if (viscosity_ >= 0.9f) viscosity_ = Common_Tools::getRandomNumber (0.3f, 0.6f);
    else if (viscosity_ <= 0.6f) viscosity_ = Common_Tools::getRandomNumber (0.7f, 0.8f);
    else viscosity_ = Common_Tools::getRandomNumber (0.9f, 1.0f);
  } // end IF

  handleInteractions ();

  for (int i = 0; i < particles_.size (); i++)
  {
    particles_[i]->update ();
    particles_[i]->show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_57::handleInteractions ()
{
  for (int i = 0; i < particles_.size (); i++)
  {
    olc::vf2d acceleration (0.0f, 0.0f);

    for (int j = 0; j < particles_.size (); j++)
    {
      if (i != j)
      {
        float x = particles_[j]->position_.x - particles_[i]->position_.x;
        float y = particles_[j]->position_.y - particles_[i]->position_.y;
        float distance_f = std::sqrt (x * x + y * y);
        if (distance_f < 1.0f) distance_f = 1.0f;

        float force = (distance_f - 320.0f) * particles_[j]->mass_ / distance_f;
        acceleration.x += force * x;
        acceleration.y += force * y;
      } // end IF

      // mouse interaction
      float x = olc::PixelGameEngine::GetMouseX () - particles_[i]->position_.x;
      float y = olc::PixelGameEngine::GetMouseY () - particles_[i]->position_.y;
      float distance_f = std::sqrt (x * x + y * y);

      // adds a dampening effect
      if (distance_f < 40.0f) distance_f = 40.0f;
      if (distance_f > 50.0f) distance_f = 50.0f;

      float force = (distance_f - 50.0f) / (5.0f * distance_f);
      acceleration.x += force * x;
      acceleration.y += force * y;
    } // end FOR

    particles_[i]->velocity_.x = particles_[i]->velocity_.x * viscosity_ + acceleration.x * particles_[i]->mass_;
    particles_[i]->velocity_.y = particles_[i]->velocity_.y * viscosity_ + acceleration.y * particles_[i]->mass_;
  } // end FOR
}
