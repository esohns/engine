#ifndef PGE_309_H
#define PGE_309_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_309
 : public olc::PixelGameEngine
{
 public:
  PGE_309 ();
  inline virtual ~PGE_309 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 angle_;
  noise::module::Perlin noise_;
  double                nx_;
  double                ny_;
  double                nz_;
  int                   c_;
};

#endif // PGE_309_H
