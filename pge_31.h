#ifndef PGE_31_H
#define PGE_31_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

class PGE_31
 : public olc::PixelGameEngine
{
 public:
  PGE_31 ();
  inline virtual ~PGE_31 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  // noise
  double                x_;
  double                y_;
  double                z_;
  noise::module::Perlin module_;

  void floodFill (const olc::vi2d&);
};

#endif // PGE_31_H
