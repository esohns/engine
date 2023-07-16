#ifndef PGE_54_H
#define PGE_54_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_54
 : public olc::PixelGameEngine
{
 public:
  PGE_54 ();
  virtual ~PGE_54 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class star
  {
   public:
    star (olc::PixelGameEngine* engine_in)
     : angle_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , radius_ (Common_Tools::getRandomNumber (engine_in->ScreenWidth () * 0.2f, engine_in->ScreenWidth () * 0.25f))
     , position_ (0.0f, 0.0f)
     , velocity_ (Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f))
    {
      position_ = {(engine_in->ScreenWidth () / 2.0f) + std::sin (angle_) * radius_,
                   (engine_in->ScreenHeight () / 2.0f) + std::cos (angle_) * radius_};
      velocity_ = velocity_.norm ();
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d bam =
        {Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f)};
      bam = bam.norm ();
      bam *= 0.45f;
      velocity_ += bam;

      olc::vf2d mouse_position = {static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ())};
      float m =
        Common_GL_Tools::map (position_.dist (mouse_position), 0.0f, static_cast<float> (engine_in->ScreenWidth ()), 8.0f, 0.05f);
      velocity_ = velocity_.norm ();
      velocity_ *= m;

      // edges
      static float half_width_f = engine_in->ScreenWidth () / 2.0f;
      static float half_height_f = engine_in->ScreenHeight () / 2.0f;
      if (position_.dist ({half_width_f, half_height_f}) > half_width_f * 0.98f)
      {
        if (position_.x < half_width_f)
          position_.x = engine_in->ScreenWidth () - position_.x - 4.0f;
        else if (position_.x > half_width_f)
          position_.x = engine_in->ScreenWidth () - position_.x + 4.0f;
        if (position_.y < half_height_f)
          position_.y = engine_in->ScreenWidth () - position_.y - 4.0f;
        else if (position_.x > half_height_f)
          position_.y = engine_in->ScreenWidth () - position_.y + 4.0f;
      } // end IF

      position_ += velocity_;
    }

    olc::vf2d position_;

   private:
    float     angle_;
    float     radius_;
    olc::vf2d velocity_;
  };

  std::vector<star*> stars_;
};

#endif // PGE_54_H
