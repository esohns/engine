#ifndef PGE_2_H
#define PGE_2_H

#include "olcPixelGameEngine.h"

#include "noise/noise.h"

class PGE_2
 : public olc::PixelGameEngine
{
 public:
  PGE_2 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int                   columns_;
  int                   rows_;
  int                   scaleFactor_;

  // noise
  double                x_;
  double                y_;
  double                z_;
  double                step_;
  noise::module::Perlin module_;
};

#endif // PGE_2_H
