#ifndef PGE_75_H
#define PGE_75_H

#include "olcPixelGameEngine.h"

class PGE_75
 : public olc::PixelGameEngine
{
 public:
  PGE_75 ();
  inline virtual ~PGE_75 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float a_;

  void conchoid (float);
};

#endif // PGE_75_H
