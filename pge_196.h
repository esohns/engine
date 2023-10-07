#ifndef PGE_196_H
#define PGE_196_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_196
 : public olc::PixelGameEngine
{
 public:
  PGE_196 ();
  inline virtual ~PGE_196 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
  float                 t_;

  float mesh (float, float, int = 2);
};

#endif // PGE_196_H
