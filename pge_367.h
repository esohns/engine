#ifndef PGE_367_H
#define PGE_367_H

#include <list>
#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_17.h"

class PGE_367
 : public olc::PixelGameEngine
{
 public:
  PGE_367 ();
  inline virtual ~PGE_367 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct active_position
  {
   public:
     active_position ()
     : position ()
     , hasBranched (false)
     , thetaPossibilities ()
    {}

    olc::vf2d        position;
    bool             hasBranched;
    std::vector<int> thetaPossibilities;
  };

  inline float distToSquared (const olc::vf2d& p1, const olc::vf2d& p2) { return ((p1.x - p2.x) * (p1.x - p2.x)) + ((p1.y - p2.y) * (p1.y - p2.y)); }

  std::list<active_position>           active_;
  float                                w_;
  int                                  columns_;
  int                                  rows_;
  std::vector<std::vector<olc::vf2d> > grid_;
  int                                  thetaDivs_;
  float                                theta0_;
};

#endif // PGE_367_H
