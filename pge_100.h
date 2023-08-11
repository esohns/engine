#ifndef PGE_100_H
#define PGE_100_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_4.h"

class PGE_100
 : public olc::PixelGameEngine
{
 public:
  PGE_100 ();
  virtual ~PGE_100 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class star
  {
   public:
    star (olc::PixelGameEngine* engine_in)
     : position_ (static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ()))
     , velocity_ (0.0f, 0.0f)
     , acceleration_ (0.0f, 0.0f)
     , G_ (0.6f)
     , mass_ (Common_Tools::getRandomNumber (2.0f, 5.0f))
     , size_ (mass_)
    {}

    void applyForce (olc::vf2d& force)
    {
      olc::vf2d f = force;
      f /= mass_;
      acceleration_ += f;
    }

    void update ()
    {
      velocity_ += acceleration_;
      position_ += velocity_;
      velocity_ *= 0.95f; // damping
      acceleration_ *= 0.0f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, static_cast<int32_t> (size_), olc::WHITE, 0xFF);
    }

    void link (star& c, olc::PixelGameEngine* engine_in)
    {
      olc::vf2d dist = c.position_ - position_;
      if (dist.mag () < 150.0f)
        engine_in->DrawLine (position_, c.position_, {255, 255, 255, 75}, 0xFFFFFFFF);
    }

    olc::vf2d attract (star& c)
    {
      olc::vf2d force = position_ - c.position_;
      float distance = force.mag ();
      if (distance > 0.0f) force = force.norm (); // prevent -nan when force is 0
      distance = std::min (std::max (distance, 20.0f), 25.0f);
      float strength = (G_ * mass_ * c.mass_) / (distance * distance);
      force *= strength;
      return force;
    }

    void checkEdges ()
    {
      if (position_.x < size_ / 2)
      {
        position_.x = size_ / 2;
        velocity_.x *= -1.0f;
      } // end IF
      else if (position_.x > ENGINE_PGE_100_DEFAULT_WIDTH - size_ / 2.0f)
      {
        position_.x = ENGINE_PGE_100_DEFAULT_WIDTH - size_ / 2.0f;
        velocity_.x *= -1.0f;
      } // end ELSE IF

      if (position_.y < size_ / 2.0f)
      {
        position_.y = size_ / 2.0f;
        velocity_.y *= -1.0f;
      } // end IF
      else if (position_.y > ENGINE_PGE_100_DEFAULT_HEIGHT - size_ / 2.0f)
      {
        position_.y = ENGINE_PGE_100_DEFAULT_HEIGHT - size_ / 2.0f;
        velocity_.y *= -1.0f;
      } // end ELSE IF
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
    float G_;
    float mass_;
    float size_;
  };

  std::vector<star*> stars_;
};

#endif // PGE_100_H
