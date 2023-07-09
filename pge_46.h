#ifndef PGE_46_H
#define PGE_46_H

#include "olcPixelGameEngine.h"

class PGE_46
 : public olc::PixelGameEngine
{
 public:
  PGE_46 ();
  inline virtual ~PGE_46 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float d_;
  float n_;
};

#endif // PGE_46_H
