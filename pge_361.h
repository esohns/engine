#ifndef PGE_361_H
#define PGE_361_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_361
 : public olc::PixelGameEngine
{
 public:
  PGE_361 ();
  inline virtual ~PGE_361 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class star
  {
   public:
    star (float x, float y)
     : position_ (x, y)
    {}

    olc::vf2d position_;
  };
   
  class hole
  {
   public:
    hole (float x, float y)
     : position_ (x, y)
    {}

    olc::vf2d position_;
  };

  std::vector<star> stars_;
  std::vector<hole> holes_;
  int               color_;
};

#endif // PGE_361_H
