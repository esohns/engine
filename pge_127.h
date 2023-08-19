#ifndef PGE_127_H
#define PGE_127_H

#include "olcPixelGameEngine.h"

class PGE_127
 : public olc::PixelGameEngine
{
 public:
  PGE_127 ();
  inline virtual ~PGE_127 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t step_;
  float   theta_;
};

#endif // PGE_127_H
