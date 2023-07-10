#ifndef PGE_48_H
#define PGE_48_H

#include "ace/Assert.h"

#include "olcPixelGameEngine.h"

#include "defines_2.h"

class PGE_48
 : public olc::PixelGameEngine
{
 public:
  PGE_48 ();
  virtual ~PGE_48 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class orbit
  {
   public:
    orbit (int32_t x_in, int32_t y_in, int32_t radius_in, int32_t level_in, orbit* parent_in)
     : position_ ({x_in, y_in})
     , radius_ (radius_in)
     , child_(NULL)
     , parent_ (parent_in)
     , level_ (level_in)
     , speed_ (static_cast<float> (std::pow (ENGINE_PGE_48_DEFAULT_K, level_in - 1) * (M_PI / 180.0)) / static_cast<float> (ENGINE_PGE_48_DEFAULT_RESOLUTION))
     , angle_ (static_cast<float> (-M_PI / 2.0))
    {}

    orbit* addChild ()
    { ACE_ASSERT (!child_);
      int32_t newr = static_cast<int32_t> (radius_ / 3.0f);
      int32_t newx = position_.x + radius_ + newr;
      int32_t newy = position_.y;
      child_ = new orbit (newx, newy, newr, level_ + 1, this);
      return child_;
    }

    void update ()
    {
      if (!parent_) return;

      angle_ += speed_;
      int32_t rsum = radius_ + parent_->radius_;
      position_.x = static_cast<int32_t> (parent_->position_.x + rsum * std::cos (angle_));
      position_.y = static_cast<int32_t> (parent_->position_.y + rsum * std::sin (angle_));
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, radius_, olc::WHITE, 0xFF);
    }

    olc::vi2d position_;
    int32_t   radius_;
    orbit*    child_;
    orbit*    parent_;

   private:
    int32_t   level_;
    float     speed_;
    float     angle_;
  };

  orbit* sun_;
  orbit* end_;
};

#endif // PGE_48_H
