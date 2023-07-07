#ifndef PGE_42_H
#define PGE_42_H

#include "olcPixelGameEngine.h"

class PGE_42
 : public olc::PixelGameEngine
{
 public:
  PGE_42 ();
  virtual ~PGE_42 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  enum direction
  {
    UP = 0,
    RIGHT,
    DOWN,
    LEFT
  };

  int* grid_;
  int  x_;
  int  y_;
  int  direction_;

  void right ();
  void left ();
  void forwardAndEdges ();
};

#endif // PGE_42_H
