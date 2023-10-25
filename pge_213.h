#ifndef PGE_213_H
#define PGE_213_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_213
 : public olc::PixelGameEngine
{
 public:
  PGE_213 ();
  inline virtual ~PGE_213 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class complex
  {
   public:
    complex ()
    {}
     
    complex (float x, float y)
     : x_ (x)
     , y_ (y)
    {}

    complex add (const complex& z)
    {
      return complex (x_ + z.x_, y_ + z.y_);
    }

    complex minus (const complex& z)
    {
      return complex (x_ - z.x_, y_ - z.y_);
    }

    complex mult (const complex& z)
    {
      return complex (x_ * z.x_ - y_ * z.y_, x_ * z.y_ + y_ * z.x_);
    }

    complex scale (float s)
    {
      return complex (x_ * s, y_ * s);
    }

    complex sq ()
    {
      return mult (*this);
    }

    float modulus ()
    {
      return std::sqrt (x_ * x_ + y_ * y_);
    }

    float arg ()
    {
      return std::atan2 (y_, x_);
    }

    complex sqrt ()
    {
      float r_f = std::sqrt (modulus ());
      float arg_f = arg () / 2.0f;
      return complex (r_f * std::cos (arg_f), r_f * std::sin (arg_f));
    }

    float x_;
    float y_;
  };

  class circle
  {
   public:
    circle (complex& z, float k)
     : k_ (k)
     , r_ (1.0f / std::abs (k))
     , z_ (z)
     , center_ ()
     , tangentCircles_ ()
    {
      center_ = z.scale (1.0f / k);
    }

    bool isEqual (const circle& c)
    {
      static float tolerance = 2.0f;

      bool equalR = std::abs (r_ - c.r_) < tolerance;
      bool equalX = std::abs (center_.x_ - c.center_.x_) < tolerance;
      bool equalY = std::abs (center_.y_ - c.center_.y_) < tolerance;

      return equalR && equalX && equalY;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (static_cast<int32_t> (center_.x_), static_cast<int32_t> (center_.y_),
                             static_cast<int32_t> (r_),
                             olc::BLACK, 0xFF);
    }

    float               k_;
    float               r_;
    complex             z_;
    complex             center_;
    std::vector<circle> tangentCircles_;
  };

  std::vector<circle> circles_;
  float               r1_;
  float               r2_;
  complex             z1_;
  complex             z2_;
  complex             center_;
  complex             touchPoint_;
  float               theta_;
  complex             mouse_;

  float find_r3 (complex&, complex&, float);
  complex find_z3 (complex&, float, float, float);
  circle thirdCircle (circle&, circle&, float);
  std::vector<circle> decartes (circle&, circle&, circle&);
  std::vector<circle> apollonian (circle&, float);
};

#endif // PGE_213_H
