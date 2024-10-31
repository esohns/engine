#ifndef PGE_380_H
#define PGE_380_H

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_380
 : public olc::PixelGameEngine
{
 public:
  PGE_380 ();
  inline virtual ~PGE_380 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float f_;
  float R_;
};

#endif // PGE_380_H
