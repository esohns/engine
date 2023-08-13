#ifndef PGE_102_H
#define PGE_102_H

#include "olcPixelGameEngine.h"

class PGE_102
 : public olc::PixelGameEngine
{
 public:
  PGE_102 ();
  virtual ~PGE_102 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  uint8_t* cells_;

  void brush ();
};

#endif // PGE_102_H
