#ifndef PGE_123_H
#define PGE_123_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

class PGE_123
 : public olc::PixelGameEngine
{
 public:
  PGE_123 ();
  virtual ~PGE_123 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    olc::vf2d position_;
    olc::vf2d velocity_;
    olc::vf2d acceleration_;
    int lifeSpan_;
    int passedLife_;
    bool dead_;
    float alpha_;
    float weight_;
    float weightRange_;
    float decay_;
    float xOfst_;
    float yOfst_;
    float h_;
    olc::Pixel color_;
    float sizeMult_;

    particle (float x, float y, float xOfst, float yOfst, float& h, float minH, float maxH, int32_t frameCount)
     : position_ (x, y)
     , velocity_ ()
     , acceleration_ ()
     , lifeSpan_ (90)
     , passedLife_ (0)
     , dead_  (false)
     , alpha_ (0.0f)
     , weight_ (0.0f)
     , weightRange_ (1.0f)
     , decay_ (0.75f)
     , xOfst_ (xOfst)
     , yOfst_ (yOfst)
     , h_ (h)
     , color_ (olc::WHITE)
     , sizeMult_ (0.0f)
    {
      float randDegrees = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      velocity_.x = std::cos (randDegrees); velocity_.y = std::sin (randDegrees);
      velocity_ *= Common_Tools::getRandomNumber (0.0f, 5.0f);

      h += 0.5f;
      if (h > maxH) h = minH + h - maxH;
      float r, g, b;
      Common_Image_Tools::HSVToRGB ((h / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
      color_.r = static_cast<uint8_t> (r * 255.0f);
      color_.g = static_cast<uint8_t> (g * 255.0f);
      color_.b = static_cast<uint8_t> (b * 255.0f);
      color_.a = 10;

      sizeMult_ = std::sin (frameCount * 0.02f);
      if (sizeMult_ < 0.05f) sizeMult_ = 0.05f;
      if (sizeMult_ > 0.5f) sizeMult_ = 0.5f;
    }

    void update (noise::module::Perlin* noise_in,
                 int32_t frameCount)
    {
      if (passedLife_ >= lifeSpan_)
        dead_ = true;
      else
        passedLife_++;

      alpha_ = float (lifeSpan_ - passedLife_) / lifeSpan_ * 70.0f + 50.0f;
      weight_ = float (lifeSpan_ - passedLife_) / lifeSpan_ * weightRange_;

      acceleration_ *= 0.0f;
      float rn =
        (Common_GL_Tools::map (static_cast<float> (noise_in->GetValue ((position_.x + frameCount + xOfst_) * 0.01,
                                                                       (position_.y + frameCount + yOfst_) * 0.01,
                                                                       0.0)), -1.0f, 1.0f, 0.0f, 1.0f) - 0.5f) * 2.0f * static_cast<float> (M_PI) * 4.0f;
      float mag =
        Common_GL_Tools::map (static_cast<float> (noise_in->GetValue ((position_.y - frameCount) * 0.01,
                                                                      (position_.x - frameCount) * 0.01,
                                                                      0.0)), -1.0f, 1.0f, 0.0f, 1.0f);
      olc::vf2d dir (std::cos (rn), std::sin (rn));
      acceleration_ += dir;
      acceleration_ *= mag;

      float randRn = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
      olc::vf2d randV (std::cos (randRn), std::sin (randRn));
      randV *= 0.25f;
      acceleration_ += randV;

      velocity_ += acceleration_;
      velocity_ *= decay_;
      if (velocity_.mag () > 3.0f)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= 3.0f;
      } // end IF
      position_ += velocity_;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, static_cast<int32_t> ((lifeSpan_ - passedLife_) * sizeMult_ / 3.0f), color_);
      //engine_in->Draw (position_, color_);
    }
  };

  std::vector<particle*> particles_;
  int                    hIndex_;
  float                  minH_;
  float                  maxH_;
  float                  h_;
  noise::module::Perlin  noise_;

  void changeHue ();
};

#endif // PGE_123_H
