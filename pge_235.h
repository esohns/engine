#ifndef PGE_235_H
#define PGE_235_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_10.h"

class PGE_235
 : public olc::PixelGameEngine
{
 public:
  PGE_235 ();
  inline virtual ~PGE_235 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class dot
  {
   public:
    dot ()
    {}

    void setup (int i)
    {
      position_ = {0.0f, 0.0f};
      velocity_ = Common_Tools::getRandomNumber (50.0f, 100.0f);
      counter_ = Common_Tools::getRandomNumber (0, 1000);
      size_ = 0.0f;
      curSize_ = 0.0f;
      amplitude_ = 0.0f;
      curAmplitude_ = 0.0f;
      radius_ = 0.0f;
      radiusCounter_ = 0.0f;
      index_ = i;

      static olc::Pixel colors_a[7] = {olc::Pixel (0x23, 0xE5, 0xCA), olc::Pixel (0x10, 0x0D, 0xE6), olc::Pixel (0xF9, 0x43, 0x25),
                                       olc::Pixel (0xDF, 0x62, 0x8A), olc::Pixel (0xE2, 0x65, 0x8B), olc::Pixel (0xF0, 0xBB, 0x73),
                                       olc::Pixel (0x45, 0x3C, 0xA5)};
      int index_i = Common_Tools::getRandomNumber (0, 6);
      col_ = colors_a[index_i];
      index_i = Common_Tools::getRandomNumber (0, 6);
      curCol_ = colors_a[index_i];
    }

    void update (int centerX_in, int frameCount_in)
    {
      curAmplitude_ += (amplitude_ - curAmplitude_) * 0.001f;
      curCol_ = olc::PixelLerp (curCol_, col_, 0.03f);
      radiusCounter_ -= 1.5f;
      radius_ = radiusCounter_ + curAmplitude_ * 100.0f;
      if (radius_ < 0.0f)
      {
        radiusCounter_ =
          centerX_in / 2.0f + index_ / static_cast<float> (ENGINE_PGE_235_DEFAULT_MODULO * 10);
        curSize_ = 0.0f;
      } // end IF
      else
        curSize_ += (size_ - curSize_) * 0.01f;
      position_.x =
        std::cos ((index_ + frameCount_in / velocity_) / ENGINE_PGE_235_DEFAULT_MODULO * 2.0f * static_cast<float> (M_PI)) * radius_ + centerX_in;
      position_.y =
        std::sin ((index_ + frameCount_in / velocity_) / ENGINE_PGE_235_DEFAULT_MODULO * 2.0f * static_cast<float> (M_PI)) * radius_ + centerX_in;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> (curSize_ * 5.0f), curCol_);
    }

    olc::vf2d  position_;
    float      velocity_;
    int        counter_;
    float      size_;
    float      curSize_;
    float      amplitude_;
    float      curAmplitude_;
    float      radius_;
    float      radiusCounter_;
    int        index_;
    olc::Pixel col_;
    olc::Pixel curCol_;
  };
   
  int mod (int, int);

  int              centerX_;
  std::vector<dot> dots_;
};

#endif // PGE_235_H
