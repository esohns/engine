#ifndef PGE_165_H
#define PGE_165_H

#include "olcPixelGameEngine.h"

class PGE_165
 : public olc::PixelGameEngine
{
 public:
  PGE_165 ();
  inline virtual ~PGE_165 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t t_;
};

#endif // PGE_165_H
