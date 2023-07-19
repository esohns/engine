#ifndef PGE_61_H
#define PGE_61_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_3.h"

class PGE_61
 : public olc::PixelGameEngine
{
 public:
  PGE_61 ();
  virtual ~PGE_61 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (int32_t x_in, int32_t y_in, float size_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , velocity_ (0.0f, 0.0f)
     , size_ (size_in)
     , alive_ (true)
     , wander_ (0.15f)
     , angle_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , drag_ (0.92f)
     , color_ (olc::WHITE)
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      position_ += velocity_;
      velocity_ *= drag_;
      angle_ +=
        Common_Tools::getRandomNumber (ENGINE_PGE_61_DEFAULT_THETA_MIN, ENGINE_PGE_61_DEFAULT_THETA_MAX) * wander_;
      velocity_.x += std::sin (angle_) * 0.1f;
      velocity_.y += std::cos (angle_) * 0.1f;
      size_ *= ENGINE_PGE_61_DEFAULT_SIZE_SCALE;
      alive_ = size_ > 0.5f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (size_), color_);
    }

    olc::vf2d  position_;
    olc::vf2d  velocity_;
    float      size_;
    bool       alive_;
    float      wander_;
    float      angle_;
    float      drag_;
    olc::Pixel color_;
  };

  std::vector<particle*> particles_;

  void spawn ();
  void update ();
  void moved ();
};

#endif // PGE_61_H
