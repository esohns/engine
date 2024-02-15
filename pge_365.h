#ifndef PGE_365_H
#define PGE_365_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_365
 : public olc::PixelGameEngine
{
 public:
  PGE_365 ();
  inline virtual ~PGE_365 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class whirl;
  class particle
  {
   public:
    particle (whirl* whirl)
     : position_ (whirl->position_)
     , angle_ (whirl->angle_)
     , whirl_ (whirl)
     , alpha_ (255)
    {}

    void show (olc::PixelGameEngine* engine)
    {
      olc::vf2d pPos = position_;

      position_.x += std::sin (angle_) * ENGINE_PGE_365_DEFAULT_SPEED;
      position_.y += std::cos (angle_) * ENGINE_PGE_365_DEFAULT_SPEED;

      olc::Pixel color = whirl_->color_;
      color.a = static_cast<uint8_t> (alpha_);
      //strokeWeight(size);
      engine->DrawLine (pPos, position_, color, 0xFFFFFFFF);
      alpha_ -= 7;
    }

    olc::vf2d position_;
    float     angle_;
    whirl*    whirl_;
    int       alpha_;
  };

  class whirl
  {
   public:
    whirl (float x, float y, float a)
     : particles_ ()
     , position_ (x, y)
     , angle_ (a)
     , color_ (static_cast<uint8_t> (x / 3.0f), static_cast<uint8_t> (y / 2.0f), 100, 255)
    {}

    void add ()
    {
      particles_.push_back (particle (this));
      angle_ +=
        (static_cast<float> (M_PI) / 180.0f) * ENGINE_PGE_365_DEFAULT_SPIN_SPEED;
    }

    void show (olc::PixelGameEngine* engine)
    {
      add ();

      for (std::vector<particle>::iterator iterator = particles_.begin ();
           iterator != particles_.end ();
           ++iterator)
        (*iterator).show (engine);

      for (std::vector<particle>::iterator iterator = particles_.begin ();
           iterator != particles_.end ();
           )
        if ((*iterator).alpha_ <= 0)
          iterator = particles_.erase (iterator);
        else
          ++iterator;
    }

    std::vector<particle> particles_;
    olc::vf2d             position_;
    float                 angle_;
    olc::Pixel            color_;
  };

  std::vector<whirl> whirls_;
};

#endif // PGE_365_H
