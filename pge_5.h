#ifndef PGE_5_H
#define PGE_5_H

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_5
 : public olc::PixelGameEngine
{
 public:
  PGE_5 ();
  ~PGE_5 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void spawn (int);
  void spawn (int, int);
  void move ();
  void remove (int, int);
  void reset ();

  int flakes[ENGINE_PGE_5_DEFAULT_WIDTH][ENGINE_PGE_5_DEFAULT_HEIGHT] = { 0 };
  int snow_fall_rate = 0;
  int max_snow_fall_rate = 50;
  float animate_elapsed = 0.0f;
  float animate_rate = 0.03125f;  // 1.0f is 1 second.  0.03125f is  1/32 of a second
};

#endif // PGE_5_H
