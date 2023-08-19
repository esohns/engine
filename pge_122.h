#ifndef PGE_122_H
#define PGE_122_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_122
 : public olc::PixelGameEngine
{
 public:
  PGE_122 ();
  inline virtual ~PGE_122 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct point
  {
    int32_t r_;
    float   theta_;
  };
  std::vector<point> points_;
};

#endif // PGE_122_H
