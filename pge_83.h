#ifndef PGE_83_H
#define PGE_83_H

#include "olcPixelGameEngine.h"

class PGE_83
 : public olc::PixelGameEngine
{
 public:
  PGE_83 ();
  inline virtual ~PGE_83 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t count_;
  int32_t size_;
};

#endif // PGE_83_H
