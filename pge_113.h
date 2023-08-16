#ifndef PGE_113_H
#define PGE_113_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_113
 : public olc::PixelGameEngine
{
 public:
  PGE_113 ();
  virtual ~PGE_113 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class ball
  {
   public:
    olc::vf2d  center_;
    olc::vf2d  direction_;
    olc::vf2d  position_;
    float      radius_;

    ball (float radius, float angle, olc::PixelGameEngine* engine_in)
     : center_ (engine_in->ScreenWidth () / 2.0f, engine_in->ScreenHeight () / 2.0f)
     , direction_ (std::cos (angle), std::sin (angle))
     , position_ (center_)
     , radius_ (radius)
    {}

    void update (size_t id, std::vector<ball*>& balls)
    {
      for (size_t i = id + 1; i < balls.size () - 1; i++)
        if (position_.dist(balls[i]->position_) < 12.0f)
          position_ += direction_;
    }

    bool isDead (olc::PixelGameEngine* engine_in)
    {
      return (position_.dist (center_) > engine_in->ScreenWidth () / 3.0f);
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (radius_), {255, 200, 0, 255});
    }
  };

  std::vector<ball*> balls_;
};

#endif // PGE_113_H
