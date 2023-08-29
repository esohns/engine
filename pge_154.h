#ifndef PGE_154_H
#define PGE_154_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_154
 : public olc::PixelGameEngine
{
 public:
  PGE_154 ();
  inline virtual ~PGE_154 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  float                 t_;

  float mesh (float, float, int = 2);
};

#endif // PGE_154_H
