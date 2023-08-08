#ifndef PGE_91_H
#define PGE_91_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_91
 : public olc::PixelGameEngine
{
 public:
  PGE_91 ();
  inline virtual ~PGE_91 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    olc::vf2d  position_;
    olc::Pixel color_;
  };

  float                 fade_;
  float                 fadeAmount_;
  std::vector<particle> particles_;
  noise::module::Perlin noise_;
};

#endif // PGE_91_H
