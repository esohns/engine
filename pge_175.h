#ifndef PGE_175_H
#define PGE_175_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_175
 : public olc::PixelGameEngine
{
 public:
  PGE_175 ();
  inline virtual ~PGE_175 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
};

#endif // PGE_175_H
