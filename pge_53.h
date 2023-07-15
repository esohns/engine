#ifndef PGE_53_H
#define PGE_53_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_2.h"

class PGE_53
 : public olc::PixelGameEngine
{
 public:
  PGE_53 ();
  virtual ~PGE_53 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    agent (olc::PixelGameEngine* engine_in,
           const olc::Pixel& color_in)
     : position_ (engine_in->ScreenWidth () / 2.0, engine_in->ScreenHeight () / 2.0)
     , direction_ (Common_Tools::getRandomNumber (0.0, M_PI * 2.0))
     , color_ (color_in)
     , step_ (Common_Tools::getRandomNumber (2.0, 3.0))
    {}

    uint8_t sense (double directionOffset_in,
                   olc::PixelGameEngine* engine_in)
    {
      double angle = direction_ + directionOffset_in;
      double x_d =
        position_.x + ENGINE_PGE_53_DEFAULT_SENSOR_OFFSET * std::cos (angle);
      double y_d =
        position_.y + ENGINE_PGE_53_DEFAULT_SENSOR_OFFSET * std::sin (angle);
      int32_t x = static_cast<int32_t> (x_d + engine_in->ScreenWidth ()) % engine_in->ScreenWidth ();
      int32_t y = static_cast<int32_t> (y_d + engine_in->ScreenHeight ()) % engine_in->ScreenHeight ();

      int32_t index = (x + y * engine_in->ScreenWidth ());
      olc::Pixel* m = engine_in->GetDrawTarget ()->GetData ();
      return m[index].r/* + m[index].g + m[index].b + m[index].a / 4.0*/;
    }

    void updateDirection (double turnAngle_in,
                          olc::PixelGameEngine* engine_in)
    {
      uint8_t right = sense (turnAngle_in,
                             engine_in);
      uint8_t center = sense (0.0f,
                              engine_in);
      uint8_t left = sense (-turnAngle_in,
                            engine_in);

      if (center > left && center > right);
      else if (left < right) direction_ += turnAngle_in;
      else if (left > right) direction_ += -turnAngle_in;
      //else direction_ += Common_Tools::getRandomNumber (-turnAngle_in / 7.0, turnAngle_in / 7.0);
    }

    void updatePosition (olc::PixelGameEngine* engine_in)
    {
      position_.x += std::cos (direction_) * step_;
      position_.y += std::sin (direction_) * step_;
      position_.x = static_cast<int32_t> (position_.x + engine_in->ScreenWidth ()) % engine_in->ScreenWidth ();
      position_.y = static_cast<int32_t> (position_.y + engine_in->ScreenHeight ()) % engine_in->ScreenHeight ();

      int32_t index =
        static_cast<int32_t> (position_.x) + static_cast<int32_t> (position_.y) * engine_in->ScreenWidth ();
      olc::Pixel* m = engine_in->GetDrawTarget ()->GetData ();
      m[index].r = color_.r;
      m[index].g = color_.g;
      m[index].b = color_.b;
      m[index].a = 255;
      //m[index].a += m[index].a < 155 ? 100 : 255 - m[index].a;
    }

    olc::vd2d  position_;

   private:
    double     direction_;
    olc::Pixel color_;
    double     step_;
  };

  std::vector<agent*> agents_;
};

#endif // PGE_53_H
