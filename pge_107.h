#ifndef PGE_107_H
#define PGE_107_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "common_math_tools.h"

class PGE_107
 : public olc::PixelGameEngine
{
 public:
  PGE_107 ();
  virtual ~PGE_107 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle (float x, float y, olc::Pixel& color)
     : position_ (x, y)
     , color_ (color)
    {}

    void moveTo (olc::vf2d& position_in)
    {
      position_ = position_in;
    }

    olc::vf2d  position_;
    olc::Pixel color_;
  };

  class field
  {
   public:
    field (float x, float y, float radius, float intensity, olc::Pixel& color)
     : position_ (x, y)
     , radiusSq_ (radius * radius)
     , intensity_ (intensity / (radius * static_cast<float> (std::sqrt (2.0f * static_cast<float> (M_PI)))))
     , power_ (-0.5f / radiusSq_)
     , color_ (color)
    {}

    olc::vf2d value (float x, float y)
    {
      olc::vf2d direction =  olc::vf2d (x, y) - position_;
      float squareMagnitude = direction.mag2 ();
      direction = direction.norm ();
      direction *= intensity_ * std::exp (power_ * squareMagnitude);
      return direction;
    }

    olc::vf2d  position_;
    float      radiusSq_;
    float      intensity_;
    float      power_;
    olc::Pixel color_;
  };

  std::vector<field*>    fields_;
  std::vector<particle*> particles_;

  field* closestFieldTo (float, float);
};

#endif // PGE_107_H
