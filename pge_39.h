#ifndef PGE_39_H
#define PGE_39_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_39
 : public olc::PixelGameEngine
{
 public:
  PGE_39 ();
  virtual ~PGE_39 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int                   columns_;
  int                   rows_;
  float*                field_;
  noise::module::Perlin noise_;
  double                zOffset_;

  int getState (float, float, float, float);
};

#endif // PGE_39_H
