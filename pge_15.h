#ifndef PGE_15_H
#define PGE_15_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines.h"

class PGE_15
 : public olc::PixelGameEngine
{
 private:
  class particle
  {
   public:
    particle (ACE_UINT32 scale_in)
     : scale_ (scale_in)
    {
      position_ = olc::vd2d (static_cast<double> (Common_Tools::getRandomNumber (0, ENGINE_PGE_15_DEFAULT_WIDTH - 1)),
                             static_cast<double> (Common_Tools::getRandomNumber (0, ENGINE_PGE_15_DEFAULT_HEIGHT - 1)));
      velocity_ = olc::vd2d (0.0, 0.0);
      acceleration_ = olc::vd2d (0.0, 0.0);
      maxSpeed_ = 4;
      prevPosition_ = position_;
    }

    void update ()
    {
      velocity_ += acceleration_;
      if (velocity_.mag () > static_cast<double> (maxSpeed_))
      {
        olc::vd2d upper_bound = { static_cast<double> (maxSpeed_), static_cast<double> (maxSpeed_) };
        olc::vd2d lower_bound = { -1.0 * static_cast<double> (maxSpeed_), -1.0 * static_cast<double> (maxSpeed_) };
        velocity_ = velocity_.clamp (lower_bound, upper_bound);
      } // end IF
      position_ += velocity_;
      acceleration_ = {0.0, 0.0};
    }

    void follow (olc::vd2d* vectors_in)
    {
      ACE_INT32 x = static_cast<ACE_INT32> (std::floor (position_.x / static_cast<double> (scale_)));
      ACE_INT32 y = static_cast<ACE_INT32> (std::floor (position_.y / static_cast<double> (scale_)));
      ACE_UINT32 index = x + y * (ENGINE_PGE_15_DEFAULT_WIDTH / scale_);
      applyForce (vectors_in[index]);
    }

    void applyForce (const olc::vd2d& force_in)
    {
      acceleration_ += force_in;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (position_, prevPosition_, olc::Pixel (0, 0, 0, 10), 0xFFFFFFFF);
      updatePrevious ();
    }

    void updatePrevious ()
    {
      prevPosition_ = position_;
    }

    void edges ()
    {
      if (position_.x > ENGINE_PGE_15_DEFAULT_WIDTH - 1)
      {
        position_.x = 0;
        updatePrevious ();
      }
      if (position_.x < 0)
      {
        position_.x = ENGINE_PGE_15_DEFAULT_WIDTH - 1;
        updatePrevious ();
      }

      if (position_.y > ENGINE_PGE_15_DEFAULT_HEIGHT - 1)
      {
        position_.y = 0;
        updatePrevious ();
      }
      if (position_.y < 0)
      {
        position_.y = ENGINE_PGE_15_DEFAULT_HEIGHT - 1;
        updatePrevious ();
      }
    }

   private:
    ACE_UINT32 scale_;
    olc::vd2d  position_;
    olc::vd2d  velocity_;
    olc::vd2d  acceleration_;
    ACE_UINT32 maxSpeed_;
    olc::vd2d  prevPosition_;
  };

 public:
  PGE_15 ();
  ~PGE_15 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  ACE_UINT32             columns_;
  ACE_UINT32             rows_;
  olc::vd2d*             flowField_;
  std::vector<particle*> particles_;

  // noise
  double                 z_ = 0.0;
  double                 step_ = 0.01;
  noise::module::Perlin  module;
};

#endif // PGE_15_H
