#ifndef PGE_242_H
#define PGE_242_H

#include <random>
#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_11.h"

class PGE_242
 : public olc::PixelGameEngine
{
 public:
  PGE_242 ();
  inline virtual ~PGE_242 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (float x, float y, int w)
     : position_ (x, y)
     , width_ (w)
     , c_ (x, y)
     , colors_ ()
     , pDirection (0)
     , c0_ (0.0f, 0.0f)
     , c1_ (0.0f, 0.0f)
     , t_ (0)
    {
      static olc::Pixel colors_a[10] = {olc::Pixel (0xF2, 0x7E, 0xA9), olc::Pixel (0x36, 0x6C, 0xD9), olc::Pixel (0x5E, 0xAD, 0xF2), olc::Pixel (0xC1, 0x29, 0x2E), olc::Pixel (0xf1, 0xd3, 0x02),
                                        olc::Pixel (0x63, 0x6E, 0x73), olc::Pixel (0xF2, 0xE6, 0xD8), olc::Pixel (0xff, 0x8c, 0x42), olc::Pixel (0x81, 0xc1, 0x4b), olc::Pixel (0x2e, 0x93, 0x3c)};
      std::vector<olc::Pixel> colors_2;
      colors_2.insert (colors_2.end (), &colors_a[0], &colors_a[9]);
      static std::random_device rd;
      static std::default_random_engine rng (rd ());
      std::shuffle (std::begin (colors_2), std::end (colors_2), rng);
      //std::random_shuffle (colors_2.begin (), colors_2.end ());
      for (int i = 0; i < 4; i++)
        colors_.push_back (colors_2[i]);

      initialize ();
    }

    void initialize ()
    {
      int drc = Common_Tools::getRandomNumber (0, 4);
      while (drc == pDirection)
        drc = Common_Tools::getRandomNumber (0, 4);
      pDirection = drc;

      float d = width_ * Common_Tools::getRandomNumber (0.4f, 0.75f);

      switch (drc)
      {
        case 0:
          c1_.x = position_.x + ((width_ / 2.0f) - (d / 2.0f));
          c1_.y = position_.y + ((width_ / 2.0f) - (d / 2.0f));
          break;
        case 1:
          c1_.x = position_.x - ((width_ / 2.0f) - (d / 2.0f));
          c1_.y = position_.y + ((width_ / 2.0f) - (d / 2.0f));
          break;
        case 2:
          c1_.x = position_.x + ((width_ / 2.0f) - (d / 2.0f));
          c1_.y = position_.y - ((width_ / 2.0f) - (d / 2.0f));
          break;
        case 3:
          c1_.x = position_.x - ((width_ / 2.0f) - (d / 2.0f));
          c1_.y = position_.y - ((width_ / 2.0f) - (d / 2.0f));
          break;
        case 4:
          c1_ = position_;
          break;
        default:
          ACE_ASSERT (false);
          break;
      } // end SWITCH

      c0_ = c_;
      t_ = 0;
    }

    float easeInOutExpo (float x)
    {
      return x == 0.0f ? 0.0f 
                       : x == 1.0f ? 1.0f
                                   : x < 0.5f ? static_cast<float> (std::pow (2, 20.0f * x - 10.0f)) / 2.0f
                                              : (2.0f - static_cast<float> (std::pow (2, -20.0f * x + 10.0f))) / 2.0f;
    }

    void update ()
    {
      if (0 < t_ && t_ < ENGINE_PGE_242_DEFAULT_T1)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (ENGINE_PGE_242_DEFAULT_T1), 0.0f, 1.0f);
        c_ = c0_.lerp (c1_, easeInOutExpo (n));
      }
      if (t_ > ENGINE_PGE_242_DEFAULT_T1)
        initialize ();
      t_++;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      float xx = position_.x - (width_ / 2.0f);
      float yy = position_.y - (width_ / 2.0f);
      float ww = c_.x - xx;
      float hh = c_.y - yy;
      float off = width_ * 0.1f;
      //float crr = width_ * 0.5f;
      engine_in->FillRect (static_cast<int32_t> (xx + (off / 2.0f)), static_cast<int32_t> (yy + (off / 2.0f)),
                           static_cast<int32_t> (ww - off), static_cast<int32_t> (hh - off),
                           colors_[0]);
      engine_in->FillRect (static_cast<int32_t> (xx + ww + (off / 2.0f)), static_cast<int32_t> (yy + (off / 2.0f)),
                           static_cast<int32_t> (width_ - ww - off), static_cast<int32_t> (hh - off),
                           colors_[1]);
      engine_in->FillRect (static_cast<int32_t> (c_.x + (off / 2.0f)), static_cast<int32_t> (c_.y + (off / 2.0f)),
                           static_cast<int32_t> (width_ - ww - off), static_cast<int32_t> (width_ - hh - off),
                           colors_[2]);
      engine_in->FillRect (static_cast<int32_t> (xx + (off / 2.0f)), static_cast<int32_t> (yy + hh + (off / 2.0f)),
                           static_cast<int32_t> (ww - off), static_cast<int32_t> (width_ - hh - off),
                           colors_[3]);
    }

    olc::vf2d               position_;
    int                     width_;
    olc::vf2d               c_;
    std::vector<olc::Pixel> colors_;
    int                     pDirection;
    olc::vf2d               c0_;
    olc::vf2d               c1_;
    int                     t_;
  };

  int                 w_;
  std::vector<object> objects_;
};

#endif // PGE_242_H
