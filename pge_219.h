#ifndef PGE_219_H
#define PGE_219_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_219
 : public olc::PixelGameEngine
{
 public:
  PGE_219 ();
  virtual ~PGE_219 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class curve
  {
   public:
    curve ()
     : current_ ()
     , path_ ()
    {}

    void setX (float x)
    {
      current_.x = x;
    }

    void setY (float y)
    {
      current_.y = y;
    }

    void addPoint ()
    {
      path_.push_back (current_);
    }

    void reset ()
    {
      path_.clear ();
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d prev_s;
      bool is_first_b = true;
      for (std::vector<olc::vf2d>::iterator iterator = path_.begin ();
           iterator != path_.end ();
           ++iterator)
      {
        if (is_first_b)
        {
          is_first_b = false;
          prev_s = *iterator;
        } // end IF

        engine_in->DrawLine (prev_s, *iterator, olc::WHITE, 0xFFFFFFFF);
        prev_s = *iterator;
      } // end FOR

      engine_in->FillCircle (current_, 4, olc::WHITE);
    }

    olc::vf2d              current_;
    std::vector<olc::vf2d> path_;
  };

  curve** curves_;
  int     columns_;
  int     rows_;
  float   angle_;
};

#endif // PGE_219_H
