#ifndef PGE_359_H
#define PGE_359_H

#include "olcPixelGameEngine.h"

class PGE_359
 : public olc::PixelGameEngine
{
 public:
  PGE_359 ();
  inline virtual ~PGE_359 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float X1 (float, float);
  float Y1 (float, float);
};

#endif // PGE_359_H
