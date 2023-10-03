#ifndef PGE_192_H
#define PGE_192_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_192
 : public olc::PixelGameEngine
{
 public:
  PGE_192 ();
  inline virtual ~PGE_192 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t               m_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;

  int noise (int32_t, int32_t, int32_t&);
};

#endif // PGE_192_H
