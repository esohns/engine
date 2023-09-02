#ifndef PGE_164_H
#define PGE_164_H

#include "olcPixelGameEngine.h"

class PGE_164
 : public olc::PixelGameEngine
{
 public:
  PGE_164 ();
  inline virtual ~PGE_164 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
};

#endif // PGE_164_H
