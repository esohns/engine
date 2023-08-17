#ifndef PGE_120_H
#define PGE_120_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE_120
 : public olc::PixelGameEngine
{
 public:
  PGE_120 ();
  virtual ~PGE_120 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d position_;
    olc::vf2d prev_;
    float     direction_;
    float     magnitude_;
    olc::vf2d velocity_;
    float     size_;
    float     friction_;
    float     gravity_;

    particle (olc::PixelGameEngine* engine_in)
     : position_ (static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ()))
     , prev_ (position_)
     , direction_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , magnitude_ (5.0f * (((static_cast<float> (engine_in->ScreenWidth ()) / 1920.0f) * Common_Tools::getRandomNumber (0.0f, 5.0f)) + 0.1f))
     , velocity_ (magnitude_ * std::cos (direction_), magnitude_ * std::sin (direction_))
     , size_ (Common_Tools::getRandomNumber (1.0f, 10.0f))
     , friction_ (0.5f)
     , gravity_ (0.2f)
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      prev_ = position_;
      position_ += velocity_;
      velocity_.y += gravity_;

      // right
      if (position_.x > engine_in->ScreenWidth () - (size_ / 2.0f))
      {
        position_.x -= velocity_.x;
        velocity_.x = -friction_ * std::abs (velocity_.x);
        velocity_.y *= friction_;
        while (position_.x > engine_in->ScreenWidth () - (size_ / 2.0f))
          position_.x -= 0.1f;
      } // end IF

      // left
      if (position_.x < 0 + (size_ / 2.0f))
      {
        position_.x += velocity_.x;
        velocity_.x = friction_ * std::abs (velocity_.x);
        velocity_.y *= friction_;
        while (position_.x < 0 + (size_ / 2.0f))
          position_.x += 0.1f;
      } // end IF

      // bottom
      if (position_.y > engine_in->ScreenHeight () - (size_ / 2.0f))
      {
        position_.y -= velocity_.y;
        velocity_.y = -friction_ * std::abs (velocity_.y);
        velocity_.x *= friction_;
        while (position_.y > engine_in->ScreenHeight () - (size_ / 2.0f))
          position_.y -= 0.1f;
      } // end IF

      // top
      if (position_.y < 0 + (size_ / 2.0f))
      {
        position_.y += velocity_.y;
        velocity_.y = friction_ * std::abs (velocity_.y);
        velocity_.x *= friction_;
        while (position_.y < 0 + (size_ / 2.0f))
          position_.y += 0.1f;
      } // end IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      //engine_in->DrawLine (prev_, position_, olc::WHITE, 0xFFFFFFFF);
      engine_in->FillCircle (position_, static_cast<int32_t> (size_ / 3.0f), olc::WHITE);
    }
  };

  std::vector<particle*> particles_;
};

#endif // PGE_120_H
