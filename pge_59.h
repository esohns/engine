#ifndef PGE_59_H
#define PGE_59_H

#include "olcPixelGameEngine.h"

#include "defines_2.h"

class PGE_59
 : public olc::PixelGameEngine
{
 public:
  PGE_59 ();
  inline virtual ~PGE_59 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  float m[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  float x[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  float y[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  float vx[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  float vy[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  uint8_t red[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  uint8_t green[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  uint8_t blue[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];
  int shape[ENGINE_PGE_59_DEFAULT_NUMBER_OF_OBJECTS];

  void reset ();
};

#endif // PGE_59_H
