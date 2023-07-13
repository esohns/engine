#ifndef PGE_50_H
#define PGE_50_H

#include "olcPixelGameEngine.h"

class PGE_50
 : public olc::PixelGameEngine
{
 public:
  PGE_50 ();
  inline virtual ~PGE_50 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_50_H
