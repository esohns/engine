#ifndef PGE_93_H
#define PGE_93_H

#include "olcPixelGameEngine.h"

class PGE_93
 : public olc::PixelGameEngine
{
 public:
  PGE_93 ();
  inline virtual ~PGE_93 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float     angle_;
  olc::vf2d mouse_;
  float     rad_;
};

#endif // PGE_93_H
