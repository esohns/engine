#ifndef PGE_226_H
#define PGE_226_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_10.h"

class PGE_226
 : public olc::PixelGameEngine
{
 public:
  PGE_226 ();
  inline virtual ~PGE_226 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (olc::PixelGameEngine* engine)
     : position_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenWidth () - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenHeight () - 1)))
     , d_ (0)
     , color_ (olc::BLACK)
     , t_ (-Common_Tools::getRandomNumber (0, 100))
     , t1_ (Common_Tools::getRandomNumber (25, 30))
     , yStep_ (Common_Tools::getRandomNumber (0.0f, 1.0f))
     , dMax_ (Common_Tools::getRandomNumber (8, 22))
    {
      static olc::Pixel colors_a[7] =
        {olc::Pixel (0xf7, 0x06, 0x40), olc::Pixel (0xf7, 0x8e, 0x2c), olc::Pixel (0xfd, 0xd9, 0x03),
         olc::Pixel (0x63, 0xbe, 0x93), olc::Pixel (0xff, 0xff, 0xff), olc::Pixel (0x29, 0x9d, 0xbf),
         olc::Pixel (0xf6, 0x54, 0xa9)};
      int index = Common_Tools::getRandomNumber (0, 6);
      color_ = colors_a[index];
    }

    void show (olc::PixelGameEngine* engine)
    {
      engine->FillCircle (position_, d_ / 2, color_);
    }

    void move (olc::PixelGameEngine* engine)
    {
      t_++;
      if (0 < t_ && t_ < t1_)
      {
        float n =
          Common_GL_Tools::map (static_cast<float> (t_), 0.0f, static_cast<float> (t1_ - 1), 0.0f, 1.0f);
        d_ = static_cast<int32_t> (dMax_ * std::sin (n * static_cast<float> (M_PI)));
      } // end IF

      if (t_ > t1_)
        initialize (engine);

      position_.y -= yStep_;
    }

    void initialize (olc::PixelGameEngine* engine)
    {
      position_.x = Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenWidth () - 1));
      position_.y = Common_Tools::getRandomNumber (0.0f, static_cast<float> (engine->ScreenHeight () - 1));
      t_ = 0;
      t1_ = Common_Tools::getRandomNumber (25, 30);
      yStep_ = Common_Tools::getRandomNumber (0.0f, 1.0f);
      dMax_ = Common_Tools::getRandomNumber (8, 22);
    }

    olc::vf2d  position_;
    int32_t    d_;
    olc::Pixel color_;
    int        t_;
    int        t1_;
    float      yStep_;
    int        dMax_;
  };

  std::vector<object> objects_;
};

#endif // PGE_226_H
