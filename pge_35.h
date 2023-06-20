#ifndef PGE_35_H
#define PGE_35_H

#include "olcPixelGameEngine.h"

class PGE_35
 : public olc::PixelGameEngine
{
 public:
  PGE_35 ();
  virtual ~PGE_35 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int columns_;
  int rows_;
  int* grid_;
  int* next_;

  int countNeighbors (int, int);
  void reset ();
};

#endif // PGE_35_H
