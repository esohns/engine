#ifndef PGE_461_H
#define PGE_461_H

#include "olcPixelGameEngine.h"

class PGE_461
 : public olc::PixelGameEngine
{
 public:
  PGE_461 ();
  inline virtual ~PGE_461 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct rectangle
  {
    olc::vi2d        position;
    olc::vi2d        size;
    std::vector<int> contains;
  };

  inline bool pointInRectangle (const olc::vf2d& point, const struct rectangle& rect)
  {
    return point.x > rect.position.x && point.x < rect.position.x + rect.size.x &&
           point.y > rect.position.y && point.y < rect.position.y + rect.size.y;
  }
  void subdivide (const struct rectangle&, bool);
  void createCompo (float);

  std::vector<olc::vf2d> points_;
};

#endif // PGE_461_H
