#ifndef PGE_64_H
#define PGE_64_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_image_tools.h"

class PGE_64
 : public olc::PixelGameEngine
{
 public:
  PGE_64 ();
  virtual ~PGE_64 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class circle
  {
   public:
    circle (float x_in, float y_in, float n_in, int frameCount_in)
     : position_ (x_in, y_in)
     , radius_ (0)
     , color_ (olc::BLACK)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (static_cast<float> (0.5 * (std::cos (std::fmod (frameCount_in / 50.0f, static_cast<float> (M_PI) * 2.0f) + n_in) + 1.0)) * 360.0f,
                                    0.8f,
                                    1.0f,
                                    r, g, b);
      color_.r = static_cast<uint8_t> (r * 255.0f);
      color_.g = static_cast<uint8_t> (g * 255.0f);
      color_.b = static_cast<uint8_t> (b * 255.0f);
      color_.a = static_cast<uint8_t> (0.3f * 255.0f);
    }

    void update ()
    {
      radius_ += 3;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      // *IMPORTANT NOTE*: FillCircle is sloooooooow :-(
      //engine_in->FillCircle (position_, radius_, color_);
      engine_in->DrawCircle (position_, radius_, color_, 0xFF);
    }

    olc::vf2d  position_;
    int32_t    radius_;
    olc::Pixel color_;
  };

  class ripple
  {
   public:
    ripple (olc::PixelGameEngine* engine_in)
     : position_ (static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenWidth () - 1)),
                  static_cast<float> (Common_Tools::getRandomNumber (0, engine_in->ScreenHeight () - 1)))
     , circles_ ()
     , lifetime_ (800)
     , n_ (Common_Tools::getRandomNumber (0.0f, static_cast<float> (M_PI) * 2.0f))
    {}

    ~ripple ()
    {
      for (std::vector<circle*>::iterator iterator = circles_.begin ();
           iterator != circles_.end ();
           ++iterator)
        delete *iterator;
    }

    void add (int frameCount_in)
    {
      if (lifetime_ > 700 && (frameCount_in % 10 == 0))
        circles_.push_back (new circle (position_.x, position_.y, n_, frameCount_in));
    }

    void update ()
    {
      --lifetime_;

      for (std::vector<circle*>::iterator iterator = circles_.begin ();
           iterator != circles_.end ();
           ++iterator)
        (*iterator)->update ();
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      for (std::vector<circle*>::iterator iterator = circles_.begin ();
           iterator != circles_.end ();
           ++iterator)
        (*iterator)->show (engine_in);
    }

    olc::vf2d            position_;
    std::vector<circle*> circles_;
    int32_t              lifetime_;
    float                n_;
  };

  std::vector<ripple*> ripples_;
};

#endif // PGE_64_H
