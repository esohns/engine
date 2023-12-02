#ifndef PGE_22_H
#define PGE_22_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "ace/Basic_Types.h"

#include "defines.h"

class PGE_22
 : public olc::PixelGameEngine
{
 private:
  class circle
  {
   public:
    circle (const olc::vi2d& position_in)
     : position_ (position_in)
     , radius_ (1)
     , growing_ (true)
    {}

    void grow ()
    {
      if (growing_)
        ++radius_;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (position_, radius_, olc::WHITE, 0xFF);
    }

    bool edges ()
    {
      return (position_.x + radius_ > ENGINE_PGE_22_DEFAULT_WIDTH - 1  ||
              position_.x - radius_ < 0                                ||
              position_.y + radius_ > ENGINE_PGE_22_DEFAULT_HEIGHT - 1 ||
              position_.y - radius_ < 0);
    }

   public:
    olc::vi2d position_;
    ACE_INT32 radius_;
    bool      growing_;
  };

 public:
  PGE_22 ();
  inline virtual ~PGE_22 () {}

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<circle>    circles_;
  std::vector<olc::vi2d> spots_;

  bool makeCircle ();
};

#endif // PGE_22_H
