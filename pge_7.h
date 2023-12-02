#ifndef PGE_7_H
#define PGE_7_H

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_7
 : public olc::PixelGameEngine
{
 public:
  PGE_7 ();
  ~PGE_7 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void spawn (int);
  void spawn (int, int);
  void leftToRight ();
  void rightToLeft ();
  void moveDrop (int, int);
  void move ();
  void remove (int, int);
  void reset ();

  int drops[ENGINE_PGE_7_DEFAULT_HEIGHT][ENGINE_PGE_7_DEFAULT_WIDTH] = { 0 };
  int water_fall_rate = 0;
  int max_water_fall_rate = 50;
  float animate_elapsed = 0.0f;
  float animate_rate = 0.001f;  // 1.0f is 1 second.  0.03125f is  1/32 of a second
  bool left = true;
};

#endif // PGE_5_H
