#ifndef PGE_346_H
#define PGE_346_H

#include "olcPixelGameEngine.h"

class PGE_346
 : public olc::PixelGameEngine
{
 public:
  PGE_346 ();
  inline virtual ~PGE_346 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float t_;
  float hue_;
};

#endif // PGE_346_H
