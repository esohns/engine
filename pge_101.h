#ifndef PGE_101_H
#define PGE_101_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_4.h"

class PGE_101
 : public olc::PixelGameEngine
{
 public:
  PGE_101 ();
  virtual ~PGE_101 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mover
  {
   public:
    mover (float x, float y, float r)
     : position_ (x, y)
     , velocity_ (Common_Tools::getRandomNumber (-1.0f, 1.0f) * 2.0f,
                  Common_Tools::getRandomNumber (-1.0f, 1.0f) * 2.0f)
     , radius_ (r)
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      position_ += velocity_;

      if (position_.x < radius_ / 2.0f || engine_in->ScreenWidth () - radius_ / 2.0f < position_.x)
        velocity_.x *= -1.0f;
      if (position_.y < radius_ / 2.0f || engine_in->ScreenHeight () - radius_ / 2.0f < position_.y)
        velocity_.y *= -1.0f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), olc::BLACK);
      olc::vf2d position_2 = position_;
      position_2.x += radius_ * 0.2f;
      position_2.y -= radius_ * 0.2f;
      engine_in->FillCircle (position_2, static_cast<int32_t> (radius_ * 0.2f), olc::WHITE);
    }

    void run (olc::PixelGameEngine* engine_in)
    {
      show (engine_in);
      update (engine_in);
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    float radius_;
  };

  std::vector<mover*> movers_;
};

#endif // PGE_101_H
