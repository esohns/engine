#ifndef PGE_289_H
#define PGE_289_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "defines_13.h"

class PGE_289
 : public olc::PixelGameEngine
{
 public:
  PGE_289 ();
  inline virtual ~PGE_289 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

  class agent
  {
   public:
    agent (int32_t x, int32_t y, int32_t w, int32_t h, float direction)
     : position_ (x, y)
     , width_ (w)
     , height_ (h)
     , direction_ (direction)
    {}

    void show (olc::PixelGameEngine* engine)
    {
      static olc::Pixel colors_a[2] = {olc::Pixel (2, 134, 2), olc::Pixel (171, 43, 43)};

      olc::Pixel color = direction_ == 0.0f ? colors_a[0] : colors_a[1];
      engine->FillRect (position_.x - width_ / 2, position_.y - height_ / 2, width_, height_, color);
    }

    bool isInScreen ()
    {
      return position_.x < ENGINE_PGE_289_DEFAULT_WIDTH + width_ / 2 &&
             position_.y < ENGINE_PGE_289_DEFAULT_HEIGHT + height_ / 2;
    }

    void update (std::vector<agent>& agents)
    {
      int32_t newx =
        position_.x + static_cast<int32_t> (std::cos (direction_) * ENGINE_PGE_289_DEFAULT_STEP);
      int32_t newy =
        position_.y + static_cast<int32_t> (std::sin (direction_) * ENGINE_PGE_289_DEFAULT_STEP);
      
      std::vector<agent*> agents_a;
      for (std::vector<agent>::iterator iterator = agents.begin ();
           iterator != agents.end ();
           ++iterator)
        if (&*iterator != this)
          agents_a.push_back (&*iterator);

      bool collide = isCollide (agents_a, newx, newy, width_, height_);
      if (!collide)
      {
        position_.x = newx;
        position_.y = newy;
      } // end IF
    }

    bool isCollide (std::vector<agent*>& agents, int32_t x, int32_t y, int32_t w, int32_t h)
    {
      for (std::vector<agent*>::iterator iterator = agents.begin();
           iterator != agents.end();
           ++iterator)
        if (std::abs ((*iterator)->position_.x - x) < ((*iterator)->width_ + w) / 2 &&
            std::abs ((*iterator)->position_.y - y) < ((*iterator)->height_ + h) / 2)
          return true;

      return false;
    }

    olc::vi2d position_;
    int32_t   width_;
    int32_t   height_;
    float     direction_;
  };

 private:
  std::vector<agent> agents_;

  bool isCollide (std::vector<PGE_289::agent>&, int32_t, int32_t, int32_t, int32_t);
  void tryCreateAgent ();
};

#endif // PGE_289_H
