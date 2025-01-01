#ifndef PGE_455_H
#define PGE_455_H

#include "olcPixelGameEngine.h"

class PGE_455
 : public olc::PixelGameEngine
{
 public:
  PGE_455 ();
  virtual ~PGE_455 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  uint8_t* grid_;
  float    zoom_;
};

#endif // PGE_455_H
