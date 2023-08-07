#ifndef PGE_86_H
#define PGE_86_H

#include "olcPixelGameEngine.h"

class PGE_86
 : public olc::PixelGameEngine
{
 public:
  PGE_86 ();
  inline virtual ~PGE_86 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float val_;

  void draw (int32_t, int);
};

#endif // PGE_86_H
