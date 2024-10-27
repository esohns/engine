#ifndef PGE_368_H
#define PGE_368_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_368
 : public olc::PixelGameEngine
{
 public:
  PGE_368 ();
  inline virtual ~PGE_368 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<std::vector<olc::Pixel> > colors_;
  std::vector<float>                    weights1_;
  std::vector<float>                    weights2_;

  inline float easeInOutQuint (float x) { return x < 0.5f ? 16.0f * x * x * x * x * x : 1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 5)) / 2.0f; }
};

#endif // PGE_368_H
