#ifndef PGE_77_H
#define PGE_77_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_77
 : public olc::PixelGameEngine
{
 public:
  PGE_77 ();
  inline virtual ~PGE_77 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct particle
  {
    olc::vf2d position_;
    float     t_;
  };

  noise::module::Perlin noise_;
  std::vector<particle> particles_;
};

#endif // PGE_77_H
