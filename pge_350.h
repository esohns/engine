#ifndef PGE_350_H
#define PGE_350_H

#include "olcPixelGameEngine.h"

class PGE_350
 : public olc::PixelGameEngine
{
 public:
  PGE_350 ();
  inline virtual ~PGE_350 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_350_H
