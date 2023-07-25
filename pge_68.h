#ifndef PGE_68_H
#define PGE_68_H

#include "olcPixelGameEngine.h"

class PGE_68
 : public olc::PixelGameEngine
{
 public:
  PGE_68 ();
  inline virtual ~PGE_68 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  double w_;
};

#endif // PGE_68_H
