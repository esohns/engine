#ifndef PGE_208_H
#define PGE_208_H

#include "olcPixelGameEngine.h"

class PGE_208
 : public olc::PixelGameEngine
{
 public:
  PGE_208 ();
  inline virtual ~PGE_208 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
};

#endif // PGE_208_H
