#ifndef PGE_424_H
#define PGE_424_H

#include "olcPixelGameEngine.h"

class PGE_424
 : public olc::PixelGameEngine
{
 public:
  PGE_424 ();
  inline virtual ~PGE_424 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int32_t u_;
};

#endif // PGE_424_H
