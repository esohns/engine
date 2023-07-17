#ifndef PGE_58_H
#define PGE_58_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

class PGE_58
 : public olc::PixelGameEngine
{
 public:
  PGE_58 ();
  virtual ~PGE_58 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class element
  {
   public:
    element (int32_t x_in, int32_t y_in, int32_t radius_in, const olc::Pixel& color_in)
     : position_ (static_cast<float> (x_in), static_cast<float> (y_in))
     , radius_(radius_in)
     , direction_ ()
     , color_(color_in)
     , velocity_ (Common_Tools::getRandomNumber (-1.0f, 1.0f), Common_Tools::getRandomNumber (-1.0f, 1.0f))
    {
      direction_ = velocity_;
    }

    void update ()
    {
      float magnitude_f = velocity_.mag ();
      velocity_.x += (direction_.x - velocity_.x) * 0.01f;
      velocity_.y += (direction_.y - velocity_.y) * 0.01f;
      velocity_ = velocity_.norm ();
      velocity_ *= magnitude_f;

      position_ += velocity_;
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle(position_, radius_, {color_.r, color_.g, color_.b, 255}, 0xFF);
      engine_in->DrawLine (static_cast<int32_t> (position_.x + direction_.x * static_cast<float> (radius_)),
                           static_cast<int32_t> (position_.y + direction_.y * static_cast<float> (radius_)),
                           static_cast<int32_t> (position_.x),
                           static_cast<int32_t> (position_.y),
                           {color_.r, color_.g, color_.b, 255},
                           0xFFFFFFFF);
    }

    void visualize (const element& element_in,
                    olc::PixelGameEngine* engine_in)
    {
      Common_GL_Color_t color1, color2;
      color1.r = color_.r;
      color1.g = color_.g;
      color1.b = color_.b;
      color2.r = element_in.color_.r;
      color2.g = element_in.color_.g;
      color2.b = element_in.color_.b;
      Common_GL_Color_t lerped_color = Common_GL_Tools::lerpRGB (color1, color2, 0.5f);

      engine_in->DrawLine (position_, element_in.position_, {lerped_color.r, lerped_color.g, lerped_color.b, color_.a }, 0xFFFFFFFF);
    }

    void onOverlap (element& element_in,
                    olc::PixelGameEngine* engine_in)
    {
      int32_t d =
        static_cast<int32_t> ((position_.x - element_in.position_.x) * (position_.x - element_in.position_.x) +
                              (position_.y - element_in.position_.y) * (position_.y - element_in.position_.y));
      if (d == (radius_ + element_in.radius_) * (radius_ + element_in.radius_))
      {
        float angle_f = 0.01f * 2.0f * static_cast<float> (M_PI);
        olc::vf2d rotated_direction;
        rotated_direction.x = direction_.x * std::cos (angle_f) - direction_.y * std::sin (angle_f);
        rotated_direction.y = direction_.x * std::sin (angle_f) + direction_.y * std::cos (angle_f);
        direction_ = rotated_direction;

        rotated_direction.x = element_in.direction_.x * std::cos (angle_f) - element_in.direction_.y * std::sin (angle_f);
        rotated_direction.y = element_in.direction_.x * std::sin (angle_f) + element_in.direction_.y * std::cos (angle_f);
        element_in.direction_ = rotated_direction;
      } // end IF

      if (d < (radius_ + element_in.radius_) * (radius_ + element_in.radius_))
      {
        direction_.x = position_.x - element_in.position_.x;
        direction_.y = position_.y - element_in.position_.y;
        direction_ = direction_.norm ();
      
        element_in.direction_ = direction_;
        element_in.direction_ *= -1.0f;
      
        visualize (element_in, engine_in);
      } // end IF
    }

    void edges (olc::PixelGameEngine* engine_in)
    {
      if (position_.x < radius_)
      {
        position_.x = static_cast<float> (radius_);
        velocity_.x = -velocity_.x;
      } // end IF
      if (position_.x > engine_in->ScreenWidth () - radius_)
      {
        position_.x = static_cast<float> (engine_in->ScreenWidth () - radius_);
        velocity_.x = -velocity_.x;
      } // end IF

      if (position_.y < radius_)
      {
        position_.y = static_cast<float> (radius_);
        velocity_.y = -velocity_.y;
      } // end IF
      if (position_.y > engine_in->ScreenHeight () - radius_)
      {
        position_.y = static_cast<float> (engine_in->ScreenHeight () - radius_);
        velocity_.y = -velocity_.y;
      } // end IF
    }

    olc::vf2d  position_;
    int32_t    radius_;
    olc::vf2d  direction_;
    olc::Pixel color_;

   private:
    olc::vf2d  velocity_;
  };

  void initialize ();

  std::vector<element*> elements_;
};

#endif // PGE_58_H
