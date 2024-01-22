#ifndef PGE_325_H
#define PGE_325_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_325
 : public olc::PixelGameEngine
{
 public:
  PGE_325 ();
  inline virtual ~PGE_325 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y)
     : position_ (x, y)
     , origin_ (x, y)
    {}

    void display (olc::PixelGameEngine* engine, olc::Pixel& color)
    {
      engine->Draw (position_, color);
    }

    void moveToMouse (float r, int m, bool q, olc::PixelGameEngine* engine)
    {
      olc::vf2d mouse_s (static_cast<float> (engine->GetMouseX ()),
                         static_cast<float> (engine->GetMouseY ()));
      float d = position_.dist (mouse_s);
      float rate = 3.0f * (r - d) * int(d <= r) / r;  //50*mult/(50+d);

      float theta = std::atan2 (mouse_s.y - position_.y, mouse_s.x - position_.x);
      float horiz = std::cos (theta);
      float verti = std::sin (theta);
      float norma = std::abs (std::abs (horiz) + std::abs (verti));

      if (q) rate *= rate / 5.0f;
      position_.x += rate * horiz * norma;
      position_.y += rate * verti * norma;
    }

    void moveToHome (float r, int m, bool q, olc::PixelGameEngine* engine)
    {
      olc::vf2d mouse_s (static_cast<float> (engine->GetMouseX ()),
                         static_cast<float> (engine->GetMouseY ()));
      float d = position_.dist (mouse_s);
      int m2 = m * m;
      float rate = m2 / static_cast<float> (m2 + d - m * int(d <= r));

      if (q) rate *= rate / 5.0f;
      position_.x -= rate * (position_.x - origin_.x) / static_cast<float> (m);
      position_.y -= rate * (position_.y - origin_.y) / static_cast<float> (m);
    }

    void backToOrigin (float vit)
    {
      position_.x -= (position_.x - origin_.x) / vit;
      position_.y -= (position_.y - origin_.y) / vit;
    }

    olc::vf2d position_;
    olc::vf2d origin_;
  };

  std::vector<particle> particles_;
  bool                  isQuad_;
  int                   mult_;
  float                 r_;
  int32_t               columns_;
  int32_t               rows_;
  bool                  back_;
};

#endif // PGE_325_H
