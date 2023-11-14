#ifndef PGE_234_H
#define PGE_234_H

#include "olcPixelGameEngine.h"

class PGE_234
 : public olc::PixelGameEngine
{
 public:
  PGE_234 ();
  inline virtual ~PGE_234 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float harmonograph (float, float, float, float, float);
};

#endif // PGE_234_H
