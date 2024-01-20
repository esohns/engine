#ifndef PGE_320_H
#define PGE_320_H

#include "olcPixelGameEngine.h"

class PGE_320
 : public olc::PixelGameEngine
{
 public:
  PGE_320 ();
  inline virtual ~PGE_320 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int n_;
  int rev_;
};

#endif // PGE_320_H
