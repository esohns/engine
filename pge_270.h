#ifndef PGE_270_H
#define PGE_270_H

#include "olcPixelGameEngine.h"

class PGE_270
 : public olc::PixelGameEngine
{
 public:
  PGE_270 ();
  inline virtual ~PGE_270 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float f_;
};

#endif // PGE_270_H
