#ifndef PGE_261_H
#define PGE_261_H

#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

class PGE_261
 : public olc::PixelGameEngine
{
 public:
  PGE_261 ();
  inline virtual ~PGE_261 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void drawNobo (float, float, int32_t, int32_t, float);

  OpenSimplexNoise noise_;
};

#endif // PGE_261_H
