#ifndef PGE_205_H
#define PGE_205_H

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_9.h"

class PGE_205
 : public olc::PixelGameEngine
{
 public:
  PGE_205 ();
  virtual ~PGE_205 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y, int32_t radius)
     : acceleration_ (0.0f, 0.0f)
     , velocity_ (0.0f, 0.0f)
     , position_ (x, y)
     , radius_ (radius)
     , aStrength_ (ENGINE_PGE_205_DEFAULT_ATTR_STRENGTH * radius)
     , rStrength_ (ENGINE_PGE_205_DEFAULT_REPU_STRENGTH * radius)
     , color_ (olc::WHITE)
    {}

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->FillCircle (position_, radius_, color_);
    }

    void update (olc::vf2d& center_in,
                 olc::PixelGameEngine* engine_in)
    {
      velocity_ += acceleration_;
      acceleration_ *= 0.0f;
      if (velocity_.mag () > ENGINE_PGE_205_DEFAULT_MIN_VELOCITY)
      {
        velocity_ = velocity_.norm ();
        velocity_ *= ENGINE_PGE_205_DEFAULT_MIN_VELOCITY;
      } // end IF
      position_ += velocity_;

      velocity_ *= 0.92f;

      float hue_f =
        Common_GL_Tools::map (velocity_.mag (), 0.0f, ENGINE_PGE_205_DEFAULT_MIN_VELOCITY, 80.0f, 0.0f);
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
      color_.r = static_cast<uint8_t> (r * 255.0f);
      color_.g = static_cast<uint8_t> (g * 255.0f);
      color_.b = static_cast<uint8_t> (b * 255.0f);

      float distance_f = center_in.dist (position_);
      radius_ =
        static_cast<int32_t> (Common_GL_Tools::map (distance_f, 0.0f, engine_in->ScreenWidth () / 2.0f, static_cast<float> (ENGINE_PGE_205_DEFAULT_RADIUS), 0.0f));
    }

    void updateForce (particle& target)
    {
      olc::vf2d force = target.position_ - position_;
      float d = force.mag ();
      force = force.norm ();

      if (d > 0.5f)
      {
        if (d <= radius_ * 8.0f)
        {
          olc::vf2d fa = force * (aStrength_ / (d * d));
          acceleration_ += fa;
        } // end IF
        if (d <= radius_ * 24.0f)
        {
          olc::vf2d fs = force * (-rStrength_ / (d * d));
          acceleration_ += fs;
        } // end IF
        if (d < static_cast<float> (radius_ + target.radius_))
        {
          olc::vf2d mv = force * (d - (radius_ + target.radius_));
          velocity_ += mv * 0.5f;
          target.velocity_ += mv * -0.5f;
        } // end IF
      } // end IF
    }

    void updateMouse (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d mv (static_cast<float> (engine_in->GetMouseX ()),
                    static_cast<float> (engine_in->GetMouseY ()));
      olc::vf2d force = mv - position_;
      float d = force.mag ();
      force = force.norm ();

      if (d < ENGINE_PGE_205_DEFAULT_MIN_VELOCITY * 2.0f)
      {
        olc::vf2d fs = force * -ENGINE_PGE_205_DEFAULT_MIN_VELOCITY;
        acceleration_ += fs;
      } // end IF
    }

    olc::vf2d  acceleration_;
    olc::vf2d  velocity_;
    olc::vf2d  position_;
    int32_t    radius_;
    float      aStrength_;
    float      rStrength_;
    olc::Pixel color_;;

  };

  std::vector<particle*> particles_;
};

#endif // PGE_205_H
