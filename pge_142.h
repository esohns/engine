#ifndef PGE_142_H
#define PGE_142_H

#include "olcPixelGameEngine.h"

class PGE_142
 : public olc::PixelGameEngine
{
 public:
  PGE_142 ();
  inline virtual ~PGE_142 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int timer_;
};

#endif // PGE_142_H
