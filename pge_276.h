#ifndef PGE_276_H
#define PGE_276_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_276
 : public olc::PixelGameEngine
{
 public:
  PGE_276 ();
  inline virtual ~PGE_276 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class traveler
  {
   public:
    traveler (float x, float y, float r, float a, float speed, float d)
     : position_ (x, y)
     , position1_ (x, y)
     , r_ (r)
     , center_ ()
     , a_ (a)
     , t_ (0.0f)
     , speed_ (speed)
     , d_ (d)
    {}

    olc::vf2d position_;
    olc::vf2d position1_;
    float     r_;
    olc::vf2d center_;
    float     a_;
    float     t_;
    float     speed_;
    float     d_;
  };

  std::vector<olc::vf2d> points_;
  std::vector<traveler>  travelers_;
};

#endif // PGE_276_H
