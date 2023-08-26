#ifndef PGE_143_H
#define PGE_143_H

#include "olcPixelGameEngine.h"

class PGE_143
 : public olc::PixelGameEngine
{
 public:
  PGE_143 ();
  inline virtual ~PGE_143 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float timer_;
};

#endif // PGE_143_H
