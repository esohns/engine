#ifndef PGE_362_H
#define PGE_362_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_17.h"

class PGE_362
 : public olc::PixelGameEngine
{
 public:
  PGE_362 ();
  inline virtual ~PGE_362 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class dot
  {
   public:
    dot (float x, float y, olc::PixelGameEngine* engine)
     : position_ (x, y)
     , distance_ (0.0f)
    {
      static olc::vf2d center_s (engine->ScreenWidth () / 2.0f,
                                 engine->ScreenHeight () / 2.0f);
      distance_ = position_.dist (center_s) * ENGINE_PGE_362_DEFAULT_ALPHA;
    }

    void show (float t, olc::PixelGameEngine* engine)
    {
      float v = (std::cos ((static_cast<float> (M_PI) / 180.0f) * (distance_ - t)) + 1.0f) / 2.0f;

      float hue = (std::fmod (100.0f * (1.0f - v) / 3.0f + 80.0f, 100.0f) / 100.0f) * 360.0f;
      //stroke(hue, 100, 100);
      //strokeWeight((1 - v) * 2 + 2);
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
      float alpha = ((50.0f * v + 10.0f) / 100.0f) * 255.0f;
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), static_cast<uint8_t> (alpha));

      float size =
        Common_GL_Tools::map (v, 0.0f, 1.0f, ENGINE_PGE_362_DEFAULT_DOT_SIZE * 4.0f - ENGINE_PGE_362_DEFAULT_MARGIN, ENGINE_PGE_362_DEFAULT_DOT_SIZE * 2.0f - ENGINE_PGE_362_DEFAULT_MARGIN);

      engine->FillCircle (position_, static_cast<int32_t> (size / 2.0f), color);
    }

    olc::vf2d position_;
    float     distance_;
  };

  std::vector<dot> dots_;
  float            t_;
};

#endif // PGE_362_H
