#ifndef PGE_162_H
#define PGE_162_H

#include "olcPixelGameEngine.h"

class PGE_162
 : public olc::PixelGameEngine
{
 public:
  PGE_162 ();
  inline virtual ~PGE_162 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t t_;
};

#endif // PGE_162_H
