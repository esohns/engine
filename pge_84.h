#ifndef PGE_84_H
#define PGE_84_H

#include "olcPixelGameEngine.h"

class PGE_84
 : public olc::PixelGameEngine
{
 public:
  PGE_84 ();
  inline virtual ~PGE_84 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float angle_;
};

#endif // PGE_84_H
