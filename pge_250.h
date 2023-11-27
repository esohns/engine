#ifndef PGE_250_H
#define PGE_250_H

#include "olcPixelGameEngine.h"

class PGE_250
 : public olc::PixelGameEngine
{
 public:
  PGE_250 ();
  inline virtual ~PGE_250 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_250_H
