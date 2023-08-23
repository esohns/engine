#ifndef PGE_135_H
#define PGE_135_H

#include "olcPixelGameEngine.h"

class PGE_135
 : public olc::PixelGameEngine
{
 public:
  PGE_135 ();
  virtual ~PGE_135 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  uint8_t* grid_;
  float    zoom_;
};

#endif // PGE_135_H
