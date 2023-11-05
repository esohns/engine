#ifndef PGE_223_H
#define PGE_223_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_223
 : public olc::PixelGameEngine
{
 public:
  PGE_223 ();
  inline virtual ~PGE_223 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float                 s_;
  float                 u_;
  float                 dDiff_;
  noise::module::Perlin noise_;

  float d (float);
  olc::Pixel rainbow (float);
  void draw_circle_arc (olc::vf2d&, float, float, float, olc::Pixel&);
  void makeTile (int, int, float, std::pair<olc::Pixel, olc::Pixel>&);
};

#endif // PGE_223_H
