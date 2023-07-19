#ifndef PGE_60_H
#define PGE_60_H

#include "olcPixelGameEngine.h"

class PGE_60
 : public olc::PixelGameEngine
{
 public:
  PGE_60 ();
  virtual ~PGE_60 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (int32_t x_in, int32_t y_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , velocity_ (0.0f, 0.0f)
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      float magnitude_f = velocity_.mag ();
      if (magnitude_f > 2.0f)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= 2.0f;
      } // end IF
      position_ += velocity_;

      if (position_.x < 0)
        position_.x = static_cast<float> (engine_in->ScreenWidth () - 1);
      else if (position_.x > engine_in->ScreenWidth () - 1)
        position_.x = 0;
      if (position_.y < 0)
        position_.y = static_cast<float> (engine_in->ScreenHeight () - 1);
      else if (position_.y > engine_in->ScreenHeight () - 1)
        position_.y = 0;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, 3, olc::WHITE, 0xFF);
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
  };

  std::vector<agent*> agents_;
};

#endif // PGE_60_H
