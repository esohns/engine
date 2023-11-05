#ifndef PGE_225_H
#define PGE_225_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_10.h"

class PGE_225
 : public olc::PixelGameEngine
{
 public:
  PGE_225 ();
  inline virtual ~PGE_225 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct block
  {
    olc::vf2d position;
    float     w;
  };

  class mover
  {
   public:
    mover (float x, float y)
     : position_ (x, y)
     , d_ (0)
     , dead_ (false)
     , step_ (Common_Tools::getRandomNumber (5, 20))
     , color_ (olc::BLACK)
    {
      static olc::Pixel colors_a[9] =
        {olc::Pixel (0xf0, 0x10, 0x1c), olc::Pixel (0xf0, 0x56, 0x97), olc::Pixel (0x0b, 0x46, 0x9b),
         olc::Pixel (0x32, 0xb6, 0xc3), olc::Pixel (0xf7, 0x80, 0x00), olc::Pixel (0xfd, 0xdf, 0x0e),
         olc::Pixel (0x9f, 0xe0, 0x63), olc::Pixel (0x76, 0x1D, 0xB0), olc::Pixel (0x00, 0xE2, 0xBA)};
      int index = Common_Tools::getRandomNumber (0, 8);
      color_ = colors_a[index];
    }

    void move (olc::PixelGameEngine* engine)
    {
      d_ += step_;
      if (d_ > engine->ScreenWidth () * 3)
        dead_ = true;
    }

    olc::vf2d  position_;
    int        d_;
    bool       dead_;
    int        step_;
    olc::Pixel color_;
  };

  std::vector<struct block> blocks_;
  std::vector<mover>        movers_;
};

#endif // PGE_225_H
