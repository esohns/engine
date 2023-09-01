#ifndef PGE_163_H
#define PGE_163_H

#include "olcPixelGameEngine.h"

class PGE_163
 : public olc::PixelGameEngine
{
 public:
  PGE_163 ();
  inline virtual ~PGE_163 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
};

#endif // PGE_163_H
