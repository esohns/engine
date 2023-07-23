#ifndef PGE_65_H
#define PGE_65_H

#include "olcPixelGameEngine.h"

class PGE_65
 : public olc::PixelGameEngine
{
 public:
  PGE_65 ();
  virtual ~PGE_65 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  bool* cells_;
  float zoom_;

  bool getNextState (bool, int, int);
};

#endif // PGE_65_H
