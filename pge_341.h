#ifndef PGE_341_H
#define PGE_341_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_image_tools.h"

class PGE_341
 : public olc::PixelGameEngine
{
 public:
  PGE_341 ();
  inline virtual ~PGE_341 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y)
     : position_ (x, y)
     //, velocity_ (0.0f, 0.0f)
     //, amp_ (0.0f)
     //, theta_ (0.0f)
    {}

    void draw (olc::vi2d& offset, olc::PixelGameEngine* engine)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (360.0f, 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 50U);
      engine->FillCircle (offset + position_, 3, color);
    }

    olc::vf2d position_;
    //olc::vf2d velocity_;
    //float     amp_;
    //float     theta_;
  };

  std::vector<particle> particles1_;
  std::vector<particle> particles2_;
  float                 theta1_;
  float                 theta2_;
  float                 lindist_;
};

#endif // PGE_341_H
