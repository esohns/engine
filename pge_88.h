#ifndef PGE_88_H
#define PGE_88_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_88
 : public olc::PixelGameEngine
{
 public:
  PGE_88 ();
  inline virtual ~PGE_88 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;

  void blob (int32_t, int32_t, int32_t, float, float, float, const olc::Pixel&);
};

#endif // PGE_88_H
