#ifndef PGE_47_H
#define PGE_47_H

#include "olcPixelGameEngine.h"

class PGE_47
 : public olc::PixelGameEngine
{
 public:
  PGE_47 ();
  inline virtual ~PGE_47 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class segment
  {
   public:
    segment (olc::vf2d& a_in, olc::vf2d& b_in)
     : a_ (a_in)
     , b_ (b_in)
    {}

    std::vector<segment> generate ()
    {
      std::vector<segment> result;

      olc::vf2d v = b_ - a_;
      v /= 3.0f;

      // Segment 0
      olc::vf2d b1 = a_ + v;
      result.push_back (segment (a_, b1));

      // Segment 3
      olc::vf2d a1 = b_ - v;
      result.push_back (segment (a1, b_));

      olc::vf2d v1; // rotate v by 60°
      v1.x = static_cast<float> (std::cos (-M_PI / 3.0) * v.x - std::sin (-M_PI / 3.0) * v.y);
      v1.y = static_cast<float> (std::sin (-M_PI / 3.0) * v.x + std::cos (-M_PI / 3.0) * v.y);
      olc::vf2d c = b1 + v1;

      // Segment 2
      result.push_back (segment (b1, c));
      // Segment 3
      result.push_back (segment (c, a1));

      return result;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawLine (a_, b_, olc::WHITE, 0xFFFFFFFF);
    }

    olc::vf2d a_;
    olc::vf2d b_;
  };

  std::vector<segment> segments_;
};

#endif // PGE_47_H
