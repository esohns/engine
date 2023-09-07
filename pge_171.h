#ifndef PGE_171_H
#define PGE_171_H

#include "olcPixelGameEngine.h"

class PGE_171
 : public olc::PixelGameEngine
{
 public:
  PGE_171 ();
  inline virtual ~PGE_171 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float x_;
  float y_;
};

#endif // PGE_171_H
