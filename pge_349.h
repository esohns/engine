#ifndef PGE_349_H
#define PGE_349_H

#include "olcPixelGameEngine.h"

class PGE_349
 : public olc::PixelGameEngine
{
 public:
  PGE_349 ();
  inline virtual ~PGE_349 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void crown (float, float, olc::Pixel&);
};

#endif // PGE_349_H
