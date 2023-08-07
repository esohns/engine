#ifndef PGE_87_H
#define PGE_87_H

#include "olcPixelGameEngine.h"

class PGE_87
 : public olc::PixelGameEngine
{
 public:
  PGE_87 ();
  inline virtual ~PGE_87 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct point
  {
    olc::vf2d position_;
    olc::vf2d prev_;
    bool      pinned_;
  };
  std::vector<point> points_;

  struct line
  {
    point* p1_;
    point* p2_;
    float  length_;
  };
  std::vector<line> lines_;

  void updatePoints ();
  void cutLines ();
  void updateLines ();
  void render ();
};

#endif // PGE_87_H
