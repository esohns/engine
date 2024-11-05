#ifndef PGE_393_H
#define PGE_393_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_common.h"
#include "common_gl_tools.h"

#include "common_math_tools.h"

#include "defines_18.h"

class PGE_393
 : public olc::PixelGameEngine
{
 public:
  PGE_393 ();
  inline virtual ~PGE_393 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class mjrc
  {
   public:
    mjrc (float x, float y, float w, int t)
     : position_ (x, y)
     , position0_ (x, y)
     , position1_ (x + w * Common_Tools::getRandomNumber (-1.0f, 1.0f), y + w * Common_Tools::getRandomNumber (-1.0f, 1.0f))
     , w_ (w)
     , w0_ (w)
     , w1_ (w * Common_Tools::getRandomNumber (0.25f, 2.0f))
     , h_ (w)
     , h0_ (w)
     , h1_ (w * Common_Tools::getRandomNumber (0.25f, 2.0f))
     , t_ (t)
     , t1_ (80)
     , t2_ (t1_ + 40)
     , a_ (0.0f)
     , a0_ (0.0f)
     , a1_ (Common_Tools::getRandomNumber (-1.0f, 1.0f) * static_cast<float> (M_PI))
     , color1_ (olc::WHITE)
     , color2_ ()
     , color_ (color1_)
    {
      static olc::Pixel palette_a[5] = {olc::Pixel (0x00, 0x00, 0xe6, 255), olc::Pixel (0x00, 0xae, 0xff, 255),
                                        olc::Pixel (0xff, 0xdd, 0x00, 255), olc::Pixel (0xef, 0x0b, 0x10, 255),
                                        olc::Pixel (0x01, 0x9B, 0x83, 255)};
      int index_i = Common_Tools::getRandomNumber (0, 4);
      color2_ = palette_a[index_i];
    }

    void draw (olc::PixelGameEngine* engine)
    {
      engine->DrawRect (position_, { static_cast<int32_t> (w_), static_cast<int32_t> (h_) }, color_);
    }

    void update ()
    {
      if (0 <= t_ && t_ < t1_)
      {
        // *TODO*: this should ease out and in again; instead it just eases out...
        float n = mjrc::easeInOutQuart (static_cast<float> (t_) / static_cast<float> (t1_ - 1));
        //float n = mjrc::easeInOutQuart (Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f));
        float t2 = std::sin (n * static_cast<float> (M_PI));
        position_.x = mjrc::lerp (position0_.x, position1_.x, t2);
        position_.y = mjrc::lerp (position0_.y, position1_.y, t2);
        w_ = mjrc::lerp (w0_, w1_, t2);
        h_ = mjrc::lerp (h0_, h1_, t2);
        a_ = mjrc::lerp (a0_, a1_, t2);

        Common_GL_Color_t color, color1, color2;
        color1.r = color1_.r; color1.g = color1_.g; color1.b = color1_.b;
        color2.r = color2_.r; color2.g = color2_.g; color2.b = color2_.b;
        color = Common_GL_Tools::lerpRGB (color1, color2, std::abs (t2));
        color_.r = color.r; color_.g = color.g; color_.b = color.b;
      } // end IF

      if (t_ > t2_)
        t_ = 0;
      t_++;
    }

    static inline float easeInOutQuart (float x) { return x < 0.5f ? 8.0f * x * x * x * x : 1.0f - std::pow (-2.0f * x + 2.0f, 4.0f) / 2.0f; }
    static inline float lerp (float start, float end, float amt) { return start + (end - start) * amt; }

    olc::vf2d position_;
    olc::vf2d position0_;
    olc::vf2d position1_;
    float     w_;
    float     w0_;
    float     w1_;
    float     h_;
    float     h0_;
    float     h1_;
    int       t_;
    int       t1_;
    int       t2_;
    float     a_;
    float     a0_;
    float     a1_;
    olc::Pixel color1_;
    olc::Pixel color2_;
    olc::Pixel color_;
  };

  std::vector<mjrc> mjrcs_;
};

#endif // PGE_393_H
