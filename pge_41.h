#ifndef PGE_41_H
#define PGE_41_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_41
 : public olc::PixelGameEngine
{
 public:
  PGE_41 ();
  virtual ~PGE_41 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mover
  {
   public:
    mover (int x_in, int y_in, float vx_in, float vy_in, int32_t mass_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , velocity_ (vx_in, vy_in)
     , acceleration_ (0.0f, 0.0f)
     , mass_ (mass_in)
     , radius_ (static_cast<int32_t> (std::sqrt (mass_in) * 2.0))
    {
    }

    void applyForce (const olc::vf2d& force_in)
    {
      olc::vf2d force = force_in / static_cast<float> (mass_);
      acceleration_ += force;
    }


    void attract (mover& mover_in)
    {
      olc::vf2d force = position_ - mover_in.position_;
      float distanceSq = force.mag2 ();
      distanceSq =
        (distanceSq <= 100.0f ? 100.0f : (distanceSq >= 1000.0f ? 1000.0f : distanceSq));
      float G = 1.0f;
      float strength = (G * (mass_ * mover_in.mass_)) / distanceSq;
      force = force.norm ();
      force *= strength;
      mover_in.applyForce (force);
    }

    void update ()
    {
      velocity_ += acceleration_;
      position_ += velocity_;
      
      acceleration_ = {0.0f, 0.0f};
    }

    void show (olc::PixelGameEngine* engine_in) const
    {
      engine_in->FillCircle (static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y), radius_, {255, 255, 255, 100});
    }

    olc::vf2d position_;
    int32_t   mass_;

   private:
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
    int32_t   radius_;
  };

  std::vector<mover*> movers_;
  mover*              sun_;
};

#endif // PGE_41_H
