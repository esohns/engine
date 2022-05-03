#ifndef PGE_H
#define PGE_H

#include "olcPixelGameEngine.h"

class PGE
 : public olc::PixelGameEngine
{
 public:
  PGE ();

 public:
  bool OnUserCreate ();
  bool OnUserUpdate(float);
};

#endif // PGE_H
