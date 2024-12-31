#ifndef PGE_452_H
#define PGE_452_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_math_tools.h"

class PGE_452
 : public olc::PixelGameEngine
{
 public:
  PGE_452 ();
  inline virtual ~PGE_452 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class object
  {
   public:
    object (float x, float y, float w)
     : position (x, y)
     , width (w)
     , currentWidth (w)
     , t (0)
     , t0 (-60)
     , t1 (40)
     , t2 (t1 + 60)
     , t3 (t2 + 40)
     , col1 (olc::BLACK)
     , col2 (olc::BLACK)
    {
      static olc::vf2d zero_s (0.0f, 0.0f);
      t = -static_cast<int32_t> (zero_s.dist (position) / 20.0f);

      static olc::Pixel palette_a[8] = { olc::Pixel (0xe6, 0x30, 0x2b), olc::Pixel (0xfd, 0x78, 0x00), olc::Pixel (0xfb, 0xd4, 0x00), olc::Pixel (0x51, 0xb7, 0x2d),
                                         olc::Pixel (0x2a, 0xbd, 0xe4), olc::Pixel (0x55, 0x34, 0xeb), olc::Pixel (0xf4, 0x77, 0xc3), olc::Pixel (0xff, 0xff, 0xff) };
      while (col1 == col2)
      {
        int index1 = Common_Tools::getRandomNumber (0, 7);
        int index2 = Common_Tools::getRandomNumber (0, 7);
        col1 = palette_a[index1];
        col2 = palette_a[index2];
      } // end WHILE
    }

    inline float easeInOutCirc (float x)
    {
      return x < 0.5f ? (1.0f - std::sqrt (1.0f - static_cast<float> (std::pow (2.0f * x, 2)))) / 2.0f
                      : (std::sqrt (1.0f - static_cast<float> (std::pow (-2.0f * x + 2.0f, 2))) + 1.0f) / 2.0f;
    }

    void show (olc::PixelGameEngine* engine)
    {
      olc::vf2d pos = position + olc::vf2d (width / 2.0f, width / 2.0f);
      float ww = width - currentWidth;
      engine->FillRect (pos + olc::vf2d (ww / 2.0f, ww / 2.0f), {static_cast<int32_t> (width), static_cast<int32_t> (width)}, col1);

      engine->FillRect (pos + olc::vf2d (-ww / 2.0f, -ww / 2.0f), {static_cast<int32_t> (width), static_cast<int32_t> (width)}, col2);

      if (currentWidth > 0.0f)
        engine->FillRect (pos, {static_cast<int32_t> (currentWidth + 1.0f), static_cast<int32_t> (currentWidth + 1.0f)}, olc::BLACK);
    }

    void update ()
    {
      if (0 < t && t < t1)
      {
        float n = Common_GL_Tools::map (static_cast<float> (t), 0.0f, static_cast<float> (t1 - 1), 0.0f, 1.0f);
        currentWidth = Common_Math_Tools::lerp (width, 0.0f, easeInOutCirc (n));
      } // end IF
      else if (t2 < t && t < t3)
      {
        float n = Common_GL_Tools::map (static_cast<float> (t), static_cast<float> (t2), static_cast<float> (t3 - 1), 0.0f, 1.0f);
        currentWidth = Common_Math_Tools::lerp (0.0f, width, easeInOutCirc (n));
      } // end ELSE IF
      if (t > t3)
      {
        t = t0;

        static olc::Pixel palette_a[8] = { olc::Pixel (0xe6, 0x30, 0x2b), olc::Pixel (0xfd, 0x78, 0x00), olc::Pixel (0xfb, 0xd4, 0x00), olc::Pixel (0x51, 0xb7, 0x2d),
                                           olc::Pixel (0x2a, 0xbd, 0xe4), olc::Pixel (0x55, 0x34, 0xeb), olc::Pixel (0xf4, 0x77, 0xc3), olc::Pixel (0xff, 0xff, 0xff) };
        col1 = col2 = olc::BLACK;
        while (col1 == col2)
        {
          int index1 = Common_Tools::getRandomNumber (0, 7);
          int index2 = Common_Tools::getRandomNumber (0, 7);
          col1 = palette_a[index1];
          col2 = palette_a[index2];
        } // end WHILE
      } // end IF

      t++;
    }

    olc::vf2d  position;
    float      width;
    float      currentWidth;
    int32_t    t;
    int32_t    t0;
    int32_t    t1;
    int32_t    t2;
    int32_t    t3;
    olc::Pixel col1;
    olc::Pixel col2;
  };


  std::vector<object> objects_;
  float               w_;
};

#endif // PGE_452_H
