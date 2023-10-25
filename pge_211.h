#ifndef PGE_211_H
#define PGE_211_H

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_211
 : public olc::PixelGameEngine
{
 public:
  PGE_211 ();
  inline virtual ~PGE_211 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  double                rcOffset_;
  double                rmOffset_;
  double                rdOffset_;
  //noise::module::Perlin noise_;
  OpenSimplexNoise      noise_;

  void reset ();
};

#endif // PGE_211_H
