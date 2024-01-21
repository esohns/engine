#ifndef PGE_323_H
#define PGE_323_H

#include "olcPixelGameEngine.h"

class PGE_323
 : public olc::PixelGameEngine
{
 public:
  PGE_323 ();
  inline virtual ~PGE_323 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_323_H
