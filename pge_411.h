#ifndef PGE_411_H
#define PGE_411_H

#include "olcPixelGameEngine.h"

class PGE_411
 : public olc::PixelGameEngine
{
 public:
  PGE_411 ();
  inline virtual ~PGE_411 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float f_;
};

#endif // PGE_411_H
  