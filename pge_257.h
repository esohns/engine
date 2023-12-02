#ifndef PGE_257_H
#define PGE_257_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_11.h"

class PGE_257
 : public olc::PixelGameEngine
{
 public:
  PGE_257 ();
  inline virtual ~PGE_257 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class dot
  {
   public:
    dot (float x, float y, float hue, float pLength)
     : origin_ (x, y)
     , position_ (x, y)
     , sizeCircle_ (0.0f)
     , sizeCircleOG_ (0.0f)
     , sizeVar_ (0.0f)
     , sizeDot_ (0.0f)
     , colorDot_ (olc::BLACK)
     , amplitude_ (0.0f, 0.0f)
     , offset_ (0.0f, 0.0f)
    {
      float s =
        Common_Tools::getRandomNumber (static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_CIRCLE_MIN), static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_CIRCLE_MAX)) / 2.0f;
      sizeCircle_ = s;
      sizeCircleOG_ = s;
      sizeVar_ =
        Common_Tools::getRandomNumber (static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_VAR_MIN), static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_VAR_MAX)) / 2.0f;
      sizeDot_ =
        std::floor (Common_Tools::getRandomNumber (static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_DOT_MIN), static_cast<float> (ENGINE_PGE_257_DEFAULT_SIZE_DOT_MAX)) / 2.0f);

      // init color
      float r, g, b;
      Common_Image_Tools::HSVToRGB (std::fmod (hue + Common_Tools::getRandomNumber (-70.0f, 70.0f), 360.0f),
                                    1.0f,
                                    0.9f,
                                    r, g, b);
      float alpha = Common_Tools::getRandomNumber (0.1f, 0.3f);
      colorDot_.r = static_cast<uint8_t> (r * 255.0f);
      colorDot_.g = static_cast<uint8_t> (g * 255.0f);
      colorDot_.b = static_cast<uint8_t> (b * 255.0f);
      colorDot_.a = static_cast<uint8_t> (alpha * 255.0f);

      // init motion vars
      amplitude_.x =
        (Common_Tools::testRandomProbability (0.5f) ? -1.0f : 1.0f) * Common_Tools::getRandomNumber (ENGINE_PGE_257_DEFAULT_AMP_MIN, ENGINE_PGE_257_DEFAULT_AMP_MAX) * pLength;
      amplitude_.y =
        (Common_Tools::testRandomProbability (0.5f) ? -1.0f : 1.0f) * Common_Tools::getRandomNumber (ENGINE_PGE_257_DEFAULT_AMP_MIN, ENGINE_PGE_257_DEFAULT_AMP_MAX) * pLength;
      offset_.x = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      offset_.y = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      frequency_ = Common_Tools::getRandomNumber (0.5f, 1.0f) * ENGINE_PGE_257_DEFAULT_DT;
    }

    void update (float t)
    {
      position_.x = origin_.x + std::sin (t * frequency_ + offset_.x) * amplitude_.x;
      position_.y = origin_.y + std::cos (t * frequency_ + offset_.y) * amplitude_.y;
      sizeCircle_ =
        sizeCircleOG_ + (std::sin (t * 0.0003f + offset_.x) + 1.0f) * sizeVar_;
    }

    olc::vf2d  origin_;
    olc::vf2d  position_;
    float      sizeCircle_;
    float      sizeCircleOG_;
    float      sizeVar_;
    float      sizeDot_;
    olc::Pixel colorDot_;
    olc::vf2d  amplitude_;
    olc::vf2d  offset_;
    float      frequency_;
  };

  float            hue_;
  std::vector<dot> dots_;

  void drawRing (dot&, dot&);
};

#endif // PGE_257_H
