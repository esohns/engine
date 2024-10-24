#include "stdafx.h"

#include "pge_174.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_174::PGE_174 ()
 : olc::PixelGameEngine ()
 , particles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 174");
}

bool
PGE_174::OnUserCreate ()
{
  return true;
}

bool
PGE_174::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    particles_.push_back (particle (this));
  if (olc::PixelGameEngine::GetMouse (1).bPressed && !particles_.empty ())
    particles_.pop_back ();

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator).velocity_ += (*iterator).acceleration_;
    (*iterator).position_ += (*iterator).velocity_;
    (*iterator).velocity_ *= 0.98f;
    (*iterator).acceleration_ *= 0.0f;
  } // end FOR

  particle current (this);
  std::vector<particle*> all_particles;
  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
    all_particles.push_back (&*iterator);
  all_particles.push_back (&current);
  collideAll (all_particles);

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    if ((*iterator).position_.x < 0.0f) { (*iterator).position_.x = 0.0f; (*iterator).velocity_.x *= -1.0f; }
    if ((*iterator).position_.x >= static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) { (*iterator).position_.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1); (*iterator).velocity_.x *= -1.0f; }
    if ((*iterator).position_.y < 0) { (*iterator).position_.y = 0.0f; (*iterator).velocity_.y *= -1.0f; }
    if ((*iterator).position_.y >= static_cast<float> (olc::PixelGameEngine::ScreenHeight ())) { (*iterator).position_.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1); (*iterator).velocity_.y *= -1.0f; }
  } // end FOR

  for (int32_t j = ENGINE_PGE_174_DEFAULT_RADIUS; j -= 10;)
  {
    olc::Pixel color (j * 2, j * 2, j * 2);
    for (std::vector<particle*>::iterator iterator = all_particles.begin ();
         iterator != all_particles.end ();
         ++iterator)
      olc::PixelGameEngine::FillCircle ((**iterator).position_, j, color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_174::collideAll (std::vector<particle*>& particles_in)
{
  for (size_t i = particles_in.size (); i--;)
    for (size_t j = i; j--;)
      collide (*particles_in[i], *particles_in[j]);
}

void
PGE_174::collide (particle& current, particle& other)
{
  olc::vf2d dir = current.position_ - other.position_;
  float dis = std::hypot (dir.x, dir.y);
  float sum = static_cast<float> (current.r_ + other.r_);
  if (sum > dis)
  {
    olc::vf2d repel = dir;
    repel /= dis;
    repel *= (sum - dis) / current.d_;
    current.acceleration_ += repel;
    other.acceleration_ -= repel;
  } // end IF
}
