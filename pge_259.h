#ifndef PGE_259_H
#define PGE_259_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_11.h"

class PGE_259
 : public olc::PixelGameEngine
{
 public:
  PGE_259 ();
  inline virtual ~PGE_259 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class point
  {
   public:
    point (float x, float y)
     : origin_ (x, y)
     , position_ (x, y)
     , velocity_ ()
     , acceleration_ ()
    {}

    void seek (olc::vf2d& target)
    {
      olc::vf2d diff = position_ - target;
      float distance = diff.mag ();
      olc::vf2d f = diff * distance;
      f *= ENGINE_PGE_259_DEFAULT_RESILIENCY;
      acceleration_ += f;
    }

    void update ()
    {
      seek (origin_);
      velocity_ += acceleration_;
      position_ += velocity_;
      velocity_ *= ENGINE_PGE_259_DEFAULT_FRICTION;
      acceleration_ *= 0.0f;
    }

    olc::vf2d origin_;
    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
  };

  std::vector<std::vector<point*> > columns_;
  std::vector<std::vector<point*> > rows_;
  std::vector<point>                main_;

  void burst ();
};

#endif // PGE_259_H
