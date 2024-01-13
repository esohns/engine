#ifndef PGE_306_H
#define PGE_306_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_14.h"

class PGE_306
 : public olc::PixelGameEngine
{
 public:
  PGE_306 ();
  inline virtual ~PGE_306 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class circle
  {
   public:
    circle (olc::vf2d& pos, float r)
     : position_ (pos)
     , radius_ (r)
     , direction_ (1.0f)
     , time_ (0.0f)
    {}

    void update (olc::PixelGameEngine* engine)
    {
      if (time_ > radius_)
        direction_ = -1.0f;
      else if (time_ < -radius_)
        direction_ = 1.0f;

      time_ += direction_ * radius_ * 0.001f;
      position_.x = (engine->ScreenWidth () / 2) + ENGINE_PGE_306_DEFAULT_R * std::sin (time_);
      position_.y = (engine->ScreenHeight () / 2) + ENGINE_PGE_306_DEFAULT_R * std::cos (time_);
    }

    void display (olc::PixelGameEngine* engine,
                  olc::Pixel& color)
    {
      engine->DrawCircle (position_, static_cast<int32_t> (radius_), color, 0xFF);
    }

    olc::vf2d position_;
    float     radius_;
    float     direction_;
    float     time_;
  };
   
  std::vector<circle> circles_;
};

#endif // PGE_306_H
