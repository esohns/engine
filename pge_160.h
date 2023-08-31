#ifndef PGE_160_H
#define PGE_160_H

#include "olcPixelGameEngine.h"

class PGE_160
 : public olc::PixelGameEngine
{
 public:
  PGE_160 ();
  inline virtual ~PGE_160 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t rows_;
};

#endif // PGE_160_H
