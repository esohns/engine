#ifndef PGE_212_H
#define PGE_212_H

#include "olcPixelGameEngine.h"

class PGE_212
 : public olc::PixelGameEngine
{
 public:
  PGE_212 ();
  inline virtual ~PGE_212 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  olc::vf2d position1_;
  olc::vf2d position2_;
  olc::vf2d position3_;
  olc::vf2d position4_;
  float     r_;
  float     inc_;
  float     a_;
};

#endif // PGE_212_H
