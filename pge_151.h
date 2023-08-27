#ifndef PGE_151_H
#define PGE_151_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_151
 : public olc::PixelGameEngine
{
 public:
  PGE_151 ();
  inline virtual ~PGE_151 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<olc::vf2d> grid_;

  void drawLine (float, float, olc::Pixel&);
  float getAngle (float, float);
};

#endif // PGE_151_H
