#ifndef PGE_209_H
#define PGE_209_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_209
 : public olc::PixelGameEngine
{
 public:
  PGE_209 ();
  inline virtual ~PGE_209 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class actor
  {
   public:
    actor ()
     : position_ ()
     , t_ (0.0f)
     , id_ (0)
     , color_ (olc::BLACK)
    {}

    void setPos (float x, float y)
    {
      position_.x = x;
      position_.y = y;
      t_ = 0.0f;
    }
    void setId (int newId)
    {
      id_ = newId;
    }
    void setColor (olc::Pixel& newColor)
    {
      color_ = newColor;
    }
    
    void update ()
    {
      actor::next (id_, position_, t_);
    }
    void render (olc::PixelGameEngine* engine_in)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      engine_in->Draw (half_width_i  + static_cast<int32_t> (position_.x),
                       half_height_i + static_cast<int32_t> (position_.y),
                       color_);
    }
    
    static void next (int id, olc::vf2d& p, float& t)
    {
      olc::vf2d temp;
      switch (id)
      {
        case 0:
          temp.x = p.x + p.y * 0.01f;
          temp.y = p.y - p.x * 0.01f;
          p = temp;
          t += 0.01f;
          break;
        case 1:
          temp.x = p.x + p.y * 0.01f;
          temp.y = p.y + p.x * 0.01f;
          p = temp;
          t += 0.01f;
          break;
        case 2:
          temp.x = p.x + (2.0f * p.x - 3.0f * p.y) * 0.01f;
          temp.y = p.y + (3.0f * p.x - 2.0f * p.y) * 0.01f;
          p = temp;
          t += 0.01f;
          break;
        case 3:
          temp.x = p.x + ( p.y + 70.0f * std::cos (2.0f * p.y) + 70.0f * std::sin (3.0f * p.x)) * 0.005f;
          temp.y = p.y + (-p.x + 70.0f * std::sin (3.0f * p.y) - 70.0f * std::cos (2.0f * p.x)) * 0.005f;
          p = temp;
          t += 0.005f;
          break;
        case 4:
          temp.x = p.x + (p.x * p.x - p.y * p.y - 2500.0f) * 0.0001f;
          temp.y = p.y + (2.0f * p.x * p.y) * 0.0001f;
          p = temp;
          t += 0.0001f;
          break;
        case 5:
          temp.x = p.y - 0.97f * p.x + 5.0f / (1.0f + p.x * p.x) - 5.0f;
          temp.y = -0.995f * p.x;
          p = temp;
          t += 0.0f;
          break;
        case 6: // Lorentz attractor.
          temp.x = p.x + (-10.0f * p.x - 10.0f * p.y) * 0.001f;
          temp.y = p.y + (-p.x * t + 28.0f * p.x - p.y) * 0.001f;
          p = temp;
          t = (p.x * p.y - (8.0f / 3.0f) * t) * 0.001f;
          break;
        case 7:
          temp.x = p.x - (p.x * p.x * p.x - 3.0f * p.x * p.y * p.y - 1331000.0f) * 0.000001f;
          temp.y = p.y - (3.0f * p.x * p.x * p.y - p.y * p.y * p.y) * 0.000001f;
          p = temp;
          t += 0.000001f;
          break;
        case 8:
          temp.x = p.x + (std::cos (t) - t * std::sin (t)) * 0.1f;
          temp.y = p.y + (std::sin (t) + t * std::cos (t)) * 0.1f;
          p = temp;
          t += 0.1f;
          break;
        case 9:
          temp.x = p.x + (p.y - 100.0f + t * 10.0f) * 0.01f;
          temp.y = p.y - (p.x - 150.0f + t * 15.0f) * 0.01f;
          p = temp;
          t += 0.01f;
          break;
        case 10:
          temp.x = p.x + (p.x + p.y) * 0.01f;
          temp.y = p.y + (p.x - p.y) * 0.01f;
          p = temp;
          t += 0.01f;
          break;
        case 11:
          temp.x = p.x + (static_cast<float> (std::pow (p.x, 4)) - 6.0f * static_cast<float> (std::pow (p.x * p.y, 2)) + static_cast<float> (std::pow (p.y, 4)) - 146410000.0f) * 0.00000001f;
          temp.y = p.y + 4.0f * (static_cast<float> (std::pow (p.x, 3)) * p.y - p.x * static_cast<float> (std::pow (p.y, 3))) * 0.00000001f;
          p = temp;
          t += 0.00000001f;
          break;
        case 12:
          temp.x = p.x - (p.x * p.x * p.x - 3.0f * p.x * p.y * p.y - 10000.0f * p.x) * 0.000001f;
          temp.y = p.y - (3.0f * p.x * p.x * p.y - p.y * p.y * p.y - 10000.0f * p.y) * 0.000001f;
          p = temp;
          t += 0.000001f;
          break;
        case 13:
          temp.x = p.x + (-2.0f * p.x * p.y) * 0.0001f;
          temp.y = p.y + (p.x * p.x - p.y * p.y - 10000.0f) * 0.0001f;
          p = temp;
          t += 0.0001f;
          break;
        case 14:
          ACE_ASSERT (false);
          break;
      } // end SWITCH
    }

    olc::vf2d  position_;
    float      t_;
    int        id_;
    olc::Pixel color_;
  };

  std::vector<actor> actors_;
  int                currentId_;

  void setting (int);
};

#endif // PGE_209_H
