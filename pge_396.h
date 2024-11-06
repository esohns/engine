#ifndef PGE_396_H
#define PGE_396_H

#include <deque>
#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_18.h"

class PGE_396
 : public olc::PixelGameEngine
{
 public:
  PGE_396 ();
  inline virtual ~PGE_396 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (std::vector<std::vector<bool> >& world,
           olc::PixelGameEngine* engine)
     : position_ (Common_Tools::getRandomNumber (0, engine->ScreenWidth () - 1),
                  Common_Tools::getRandomNumber (0, engine->ScreenHeight () - 1))
     , color_ (static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
               static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
               static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 255)),
               255)
     , stack_ ()
    {
      stack_.push_front (position_);
      world[position_.x][position_.y] = true;
      engine->Draw (position_, color_);
    }

    void update (std::vector<std::vector<bool> >& world,
                 olc::PixelGameEngine* engine)
    {
      static std::vector<olc::vi2d> std_directions_a = { { 1, 1 }, { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 } };
      static std::vector<olc::vi2d> alt_directions_a = {           { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 }, { 1, 1 } };
      std::vector<olc::vi2d> directions_a = std_directions_a;
      if (Common_Tools::testRandomProbability (0.5f))
        directions_a = alt_directions_a;
      //static std::vector<olc::vi2d> std_directions_a = { { 0, 1 }, { -1, 0 }, { 0, -1 }, { 1, 0 } };
      //std::vector<olc::vi2d> directions_a = std_directions_a;
      //static std::random_device rd;
      //static std::default_random_engine rng (rd ());
      //std::shuffle (directions_a.begin (), directions_a.end (), rng);

      static int32_t screen_width_i = engine->ScreenWidth ();
      static int32_t screen_height_i = engine->ScreenHeight ();
      bool done_b = false;
      while (!stack_.empty () && !done_b)
      {
        olc::vi2d top_s = stack_.front ();

        for (std::vector<olc::vi2d>::iterator iterator = directions_a.begin ();
             iterator != directions_a.end ();
             ++iterator)
        {
          if (Common_Tools::testRandomProbability (0.12f))
            continue;

          int newX = (top_s.x + (*iterator).x + screen_width_i) % screen_width_i;
          int newY = (top_s.y + (*iterator).y + screen_height_i) % screen_height_i;
          if (world[newX][newY])
            continue;

          stack_.push_front ({newX, newY});
          world[newX][newY] = true;
          engine->Draw (newX, newY, color_);
          done_b = true;
          break;
        } // end FOR

        if (!done_b)
        {
          stack_.pop_front ();
          break; // back-up one step
        } // end IF
      } // end WHILE
    }

    olc::vi2d             position_;
    olc::Pixel            color_;
    std::deque<olc::vi2d> stack_;
  };

  std::vector<agent>              agents_;
  std::vector<std::vector<bool> > world_;
};

#endif // PGE_396_H
