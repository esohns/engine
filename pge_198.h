#ifndef PGE_198_H
#define PGE_198_H

#include "olcPixelGameEngine.h"

class PGE_198
 : public olc::PixelGameEngine
{
 public:
  PGE_198 ();
  inline virtual ~PGE_198 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
};

#endif // PGE_198_H
