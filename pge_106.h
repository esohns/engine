#ifndef PGE_106_H
#define PGE_106_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

class PGE_106
 : public olc::PixelGameEngine
{
 public:
  PGE_106 ();
  virtual ~PGE_106 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (float x, float y, float w)
     : position_ (x, y)
     , w_ (w)
     , cw_ (w)
     , t_ (-position_.dist ({0.0f, 0.0f}) / 20.0f)
     , t0_ (-60.0f)
     , t1_ (40.0f)
     , t2_ (t1_ + 60.0f)
     , t3_ (t2_ + 40.0f)
     , color1_ (olc::BLACK)
     , color2_ (olc::BLACK)
    {
      static olc::Pixel palette_a[8] =
        {{0xe6, 0x30, 0x2b},{0xfd, 0x78, 0x00},{0xfb, 0xd4, 0x00},{0x51, 0xb7, 0x2d},{0x2a, 0xbd, 0xe4},{0x55, 0x34, 0xeb},{0xf4, 0x77, 0xc3},{0xff, 0xff, 0xff}};
      while (color1_ == color2_)
      {
        int i = Common_Tools::getRandomNumber (0, 7);
        color1_ = palette_a[i];
        i = Common_Tools::getRandomNumber (0, 7);
        color2_ = palette_a[i];
      } // end WHILE
    }

    float easeInOutCirc (float x)
    {
      return x < 0.5f ? (1.0f - static_cast<float> (std::sqrt (1.0f - static_cast<float> (std::pow (2.0f * x, 2))))) / 2.0f
                      : (static_cast<float> (std::sqrt (1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 2))) + 1.0f)) / 2.0f;
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      if (0.0f < t_ && t_ < t1_)
      {
        float n = Common_GL_Tools::map (t_, 0.0f, t1_ - 1.0f, 0.0f, 1.0f);
        cw_ = Common_Math_Tools::lerp (w_, 0.0f, easeInOutCirc (n));
      } // end IF
      else if (t2_ < t_ && t_ < t3_)
      {
        float n = Common_GL_Tools::map (t_, t2_, t3_ - 1.0f, 0.0f, 1.0f);
        cw_ = Common_Math_Tools::lerp (0.0f, w_, easeInOutCirc (n));
      } // end ELSE IF
      if (t_ > t3_)
      {
        t_ = t0_;

        static olc::Pixel palette_a[8] =
          {{0xe6, 0x30, 0x2b},{0xfd, 0x78, 0x00},{0xfb, 0xd4, 0x00},{0x51, 0xb7, 0x2d},{0x2a, 0xbd, 0xe4},{0x55, 0x34, 0xeb},{0xf4, 0x77, 0xc3},{0xff, 0xff, 0xff}};
        color1_ = color2_ = olc::BLACK;
        while (color1_ == color2_)
        {
          int i = Common_Tools::getRandomNumber (0, 7);
          color1_ = palette_a[i];
          i = Common_Tools::getRandomNumber (0, 7);
          color2_ = palette_a[i];
        } // end WHILE
      }
      t_ += 1.0f;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      float ww = w_ - cw_;
      engine_in->FillRect (static_cast<int32_t> (position_.x - (ww / 2.0f) + (ww / 2.0f)), static_cast<int32_t> (position_.y - (ww / 2.0f) + (ww / 2.0f)), static_cast<int32_t> (w_), static_cast<int32_t> (w_), color1_);
      engine_in->FillRect (static_cast<int32_t> (position_.x - (ww / 2.0f) - (ww / 2.0f)), static_cast<int32_t> (position_.y - (ww / 2.0f) - (ww / 2.0f)), static_cast<int32_t> (w_), static_cast<int32_t> (w_), color1_);
      engine_in->FillRect (static_cast<int32_t> (position_.x), static_cast<int32_t> (position_.y), static_cast<int32_t> (cw_ + 1.0f), static_cast<int32_t> (cw_ + 1.0f), olc::BLACK);
    }

    void run (olc::PixelGameEngine* engine_in)
    {
      show (engine_in);
      update (engine_in);
    }

    olc::vf2d  position_;
    float      w_;
    float      cw_;
    float      t_;
    float      t0_;
    float      t1_;
    float      t2_;
    float      t3_;
    olc::Pixel color1_;
    olc::Pixel color2_;
  };

  std::vector<object*> objects_;
};

#endif // PGE_106_H
