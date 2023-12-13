#ifndef PGE_254_H
#define PGE_254_H

#include <random>
#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_11.h"

class PGE_254
 : public olc::PixelGameEngine
{
 public:
  PGE_254 ();
  inline virtual ~PGE_254 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (olc::PixelGameEngine* engine_in)
     : position_ (Common_Tools::getRandomNumber (static_cast<float> (ENGINE_PGE_254_DEFAULT_BORDER), static_cast<float> (engine_in->ScreenWidth ()  - 1 - ENGINE_PGE_254_DEFAULT_BORDER)),
                  Common_Tools::getRandomNumber (static_cast<float> (ENGINE_PGE_254_DEFAULT_BORDER), static_cast<float> (engine_in->ScreenHeight () - 1 - ENGINE_PGE_254_DEFAULT_BORDER)))
     , initialPosition_ (position_)
     , direction_ (Common_Tools::testRandomProbability (0.5f) ? 1.0f : -1.0f)
     , step_ (1.0f)
     , scale_ (5.0f)
     , color_ (generateColor ())
    {}

    void update (olc::PixelGameEngine* engine_in)
    {
      if (Common_Tools::testRandomProbability (1.0e-4f))
        direction_ *= -1.0f;

      position_.x += direction_ * vector_field (position_.x, position_.y, scale_, engine_in).x * step_;
      position_.y += direction_ * vector_field (position_.x, position_.y, scale_, engine_in).y * step_;

      engine_in->DrawLine (initialPosition_, position_, color_, 0xFFFFFFFF);

      //this.pOld.set(this.p);
    }

    olc::vf2d  position_;
    olc::vf2d  initialPosition_;
    float      direction_;
    float      step_;
    float      scale_;
    olc::Pixel color_;

   private:
    olc::vf2d vector_field (float x, float y, float scale, olc::PixelGameEngine* engine_in)
    {
      x =
        Common_GL_Tools::map (x, 0.0f, static_cast<float> (engine_in->ScreenWidth () - 1),  -scale, scale);
      y =
        Common_GL_Tools::map (y, 0.0f, static_cast<float> (engine_in->ScreenHeight () - 1), -scale, scale);

      static float k1 = 5.0f;
      static float k2 = 3.0f;

      //let u = sin(k1 * y) + cos(k2 * y) + map(noise(x,y),0,1,-1,1);
      //let v = sin(k2 * x) - cos(k1 * x) + map(noise(x,y),0,1,-1,1);
      olc::vf2d result (std::sin (k1 * y) + std::cos (k2 * y),
                        std::sin (k2 * x) - std::cos (k1 * x));

      return result;
    }

    olc::Pixel generateColor ()
    {
      olc::Pixel temp = myRandom ();

      float hue_f, saturation_f, brightness_f;
      Common_Image_Tools::RGBToHSV (temp.r / 255.0f, temp.g / 255.0f, temp.b / 255.0f,
                                    hue_f, saturation_f, brightness_f);
      saturation_f *= 100.0f;

      static std::random_device rd;
      static std::mt19937 m (rd ());
      static std::normal_distribution dist (0.0f, 1.0f);

      hue_f += dist (m) * 10.0f;
      hue_f = std::max (std::min (hue_f, 360.0f), 0.0f);
      saturation_f += dist (m) * 10.0f;
      saturation_f = std::max (std::min (saturation_f, 100.0f), 0.0f);
      saturation_f /= 100.0f;
      brightness_f *= 0.75f;

      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_f, saturation_f, brightness_f,
                                    r, g, b);
      olc::Pixel result;
      result.r = static_cast<uint8_t> (r * 255.0f);
      result.g = static_cast<uint8_t> (g * 255.0f);
      result.b = static_cast<uint8_t> (b * 255.0f);
      result.a = static_cast<uint8_t> (Common_Tools::getRandomNumber (0, 128));

      return result;
    }

    // function to select 
    olc::Pixel myRandom ()
    {
      static olc::Pixel colors_a[5] = {olc::RED, olc::Pixel (0xfe, 0xb3, 0x0f), olc::Pixel (0x0a, 0xa4, 0xf7), olc::BLACK, olc::WHITE};
      static int weights_a[6] = {1, 2, 2, 1, 1, 0};

      int sum = 0;
      for (int i = 0; i < 6; i++)
        sum += weights_a[i];

      int rr = Common_Tools::getRandomNumber (0, sum);
      for (int j = 0; j < 6; j++)
      {
        if (weights_a[j] >= rr)
          return colors_a[j];
        rr -= weights_a[j];
      } // end FOR

      return colors_a[0];
    }
  };

  std::vector<agent> agents_;
};

#endif // PGE_254_H
