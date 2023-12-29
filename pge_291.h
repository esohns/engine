#ifndef PGE_291_H
#define PGE_291_H

#include "olcPixelGameEngine.h"

class PGE_291
 : public olc::PixelGameEngine
{
 public:
  PGE_291 ();
  inline virtual ~PGE_291 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_291_H
