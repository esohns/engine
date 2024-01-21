#ifndef PGE_324_H
#define PGE_324_H

#include "olcPixelGameEngine.h"

class PGE_324
 : public olc::PixelGameEngine
{
 public:
  PGE_324 ();
  inline virtual ~PGE_324 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
};

#endif // PGE_324_H
