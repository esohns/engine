#ifndef PGE_280_H
#define PGE_280_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_280
 : public olc::PixelGameEngine
{
 public:
  PGE_280 ();
  inline virtual ~PGE_280 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct square 
  {
    int   i;
    int   j;
    int   s;
    float step;
  };

  std::vector<struct square> squares_;

  struct square generateSquare ();
  void drawSquare (int, int, int, float, int);
};

#endif // PGE_280_H
