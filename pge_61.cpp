#include "stdafx.h"

#include "pge_61.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_61::PGE_61 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = "Example 61";
}

PGE_61::~PGE_61 ()
{
  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_61::OnUserCreate ()
{
  return true;
}

bool
PGE_61::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static int32_t mouse_x = olc::PixelGameEngine::GetMouseX ();
  static int32_t mouse_y = olc::PixelGameEngine::GetMouseY ();
  if (olc::PixelGameEngine::GetMouseX () != mouse_x ||
      olc::PixelGameEngine::GetMouseY () != mouse_y)
  {
    moved ();
    mouse_x = olc::PixelGameEngine::GetMouseX ();
    mouse_y = olc::PixelGameEngine::GetMouseY ();
  } // end IF

  update ();

  for (std::vector<particle*>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    (*iterator)->show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_61::spawn ()
{
  if (particles_.size () >= ENGINE_PGE_61_DEFAULT_MAX_NUMBER_OF_PARTICLES)
  {
    std::vector<particle*>::iterator iterator = particles_.begin ();
    delete* iterator;
    particles_.erase (iterator);
  } // end IF

  particle* particle_p = new particle (olc::PixelGameEngine::GetMouseX (),
                                       olc::PixelGameEngine::GetMouseY (),
                                       Common_Tools::getRandomNumber (ENGINE_PGE_61_DEFAULT_SIZE_MIN, ENGINE_PGE_61_DEFAULT_SIZE_MAX));
  particle_p->wander_ =
    Common_Tools::getRandomNumber (ENGINE_PGE_61_DEFAULT_WANDER_MIN, ENGINE_PGE_61_DEFAULT_WANDER_MAX);
  particle_p->color_.r = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
  particle_p->color_.g = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
  particle_p->color_.b = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255));
  particle_p->drag_ = Common_Tools::getRandomNumber (ENGINE_PGE_61_DEFAULT_DRAG_MIN, ENGINE_PGE_61_DEFAULT_DRAG_MAX);
  float angle_f =
    Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
  float force_f =
    Common_Tools::getRandomNumber (ENGINE_PGE_61_DEFAULT_FORCE_MIN, ENGINE_PGE_61_DEFAULT_FORCE_MAX);
  particle_p->velocity_.x = std::sin (angle_f) * force_f;
  particle_p->velocity_.y = std::cos (angle_f) * force_f;
  particles_.push_back (particle_p);
}

void
PGE_61::update ()
{
  for (int i = static_cast<int> (particles_.size () - 1); i >= 0; i--)
  {
    if (particles_[i]->alive_)
      particles_[i]->update (this);
    else
    {
      std::vector<particle*>::iterator iterator = particles_.begin ();
      delete* iterator;
      particles_.erase (iterator);
    } // end ELSE
  } // end FOR
}

void
PGE_61::moved ()
{
  int max_i = Common_Tools::getRandomNumber (1, 4);
  for (int i = 0; i < max_i; i++)
    spawn ();
}
