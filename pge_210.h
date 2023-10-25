#ifndef PGE_210_H
#define PGE_210_H

#include "olcPixelGameEngine.h"

class PGE_210
 : public olc::PixelGameEngine
{
 public:
  PGE_210 ();
  inline virtual ~PGE_210 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  olc::vf2d position_;
  //olc::vf2d previous_;
  float     s_;
};

#endif // PGE_210_H
