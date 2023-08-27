#ifndef PGE_152_H
#define PGE_152_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_image_tools.h"

#include "defines_6.h"

class PGE_152
 : public olc::PixelGameEngine
{
 public:
  PGE_152 ();
  inline virtual ~PGE_152 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class star
  {
   public:
    olc::vf2d  position_;
    olc::vf2d  origin_;
    olc::Pixel color_;

    star (int i,
          olc::PixelGameEngine* engine_in)
     : position_ (std::cos (static_cast<float> (i)) * 280.0f + engine_in->ScreenWidth () / 2,
                  std::sin (static_cast<float> (i)) * 280.0f + engine_in->ScreenHeight () / 2)
     , origin_ (position_)
     , color_ ()
    {
      float hue = static_cast<float> (i % 360);
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
      color_.r = static_cast<uint8_t> (r * 255.0f);
      color_.g = static_cast<uint8_t> (g * 255.0f);
      color_.b = static_cast<uint8_t> (b * 255.0f);
    }

    void update (std::vector<olc::vf2d>& blackHoles_in,
                 olc::PixelGameEngine* engine_in)
    {
      float angle = sumAngles (blackHoles_in);
      if (angle == -1.0f) return;
      olc::vf2d next (position_.x + std::cos (angle) * ENGINE_PGE_152_DEFAULT_STEP,
                      position_.y + std::sin (angle) * ENGINE_PGE_152_DEFAULT_STEP);
      engine_in->DrawLine (position_, next, color_, 0xFFFFFFFF);
      position_ = next;
    }

    float sumAngles (std::vector<olc::vf2d>& blackHoles_in)
    {
      float angle = 0.0f;

      for (std::vector<olc::vf2d>::iterator iterator = blackHoles_in.begin ();
           iterator != blackHoles_in.end ();
           ++iterator)
      {
        float dx = (*iterator).x - position_.x;
        float dy = (*iterator).y - position_.y;
        angle += std::atan2 (dy, dx);
        if (std::sqrt (dx * dx + dy * dy) < ENGINE_PGE_152_DEFAULT_STEP)
        {
          position_ = origin_;
          return -1.0f;
        } // end IF
      } // end FOR

      return angle;
    }
  };

  std::vector<olc::vf2d> blackHoles_;
  std::vector<star>      stars_;
};

#endif // PGE_152_H
