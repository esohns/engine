#ifndef PGE_397_H
#define PGE_397_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_397
 : public olc::PixelGameEngine
{
 public:
  PGE_397 ();
  inline virtual ~PGE_397 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> points_;
  int                    t_;

  inline float F (float p, float d, float m = 1.0f) { return std::sin (p / d) * m + std::sin (p / d * 2.0f) + std::sin (p / d * 4.0f); }
};

#endif // PGE_397_H
