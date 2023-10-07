#ifndef PGE_197_H
#define PGE_197_H

#include "olcPixelGameEngine.h"

class PGE_197
 : public olc::PixelGameEngine
{
 public:
  PGE_197 ();
  inline virtual ~PGE_197 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float wave (float, float, float, float, float, float);
};

#endif // PGE_197_H
