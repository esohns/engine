#ifndef PGE_307_H
#define PGE_307_H

#include "olcPixelGameEngine.h"

class PGE_307
 : public olc::PixelGameEngine
{
 public:
  PGE_307 ();
  inline virtual ~PGE_307 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float x (float);
  float y (float);
  float z (float);
};

#endif // PGE_307_H
