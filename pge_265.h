#ifndef PGE_265_H
#define PGE_265_H

#include <vector>

//#include "noise/noise.h"
#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_265
 : public olc::PixelGameEngine
{
 public:
  PGE_265 ();
  inline virtual ~PGE_265 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct cache_node
  {
    olc::Pixel color;
    float      nx;
    float      ny;
  };

  std::vector<struct cache_node> cache_;
  //noise::module::Perlin          noise_;
  OpenSimplexNoise               noise_;
};

#endif // PGE_265_H
