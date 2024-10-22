#ifndef PGE_213_H
#define PGE_213_H

#include <complex>
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
  //class complex
  //{
  // public:
  //  complex (const complex& c)
  //    : x_ (c.x_)
  //    , y_ (c.y_)
  //   {}

  //  complex (float x, float y)
  //   : x_ (x)
  //   , y_ (y)
  //  {}

  //  complex add (const complex& z) const
  //  {
  //    return complex (x_ + z.x_, y_ + z.y_);
  //  }

  //  complex minus (const complex& z) const
  //  {
  //    return complex (x_ - z.x_, y_ - z.y_);
  //  }

  //  complex mult (const complex& z) const
  //  {
  //    return complex (x_ * z.x_ - y_ * z.y_, x_ * z.y_ + y_ * z.x_);
  //  }

  //  complex scale (float s) const
  //  {
  //    return complex (x_ * s, y_ * s);
  //  }

  //  //complex sq () const
  //  //{
  //  //  return mult (*this);
  //  //}

  //  float modulus () const
  //  {
  //    return std::sqrt (x_ * x_ + y_ * y_);
  //  }

  //  float arg () const
  //  {
  //    return std::atan2 (y_, x_);
  //  }

  //  complex sqrt () const
  //  {
  //    float r_f = std::sqrt (modulus ());
  //    float arg_f = arg () / 2.0f;
  //    return complex (r_f * std::cos (arg_f), r_f * std::sin (arg_f));
  //  }

  //  float x_;
  //  float y_;
  //};

  class circle
  {
   public:
    //circle (const circle& c)
    // : k_ (c.k_)
    // , r_ (c.r_)
    // , z_ (c.z_)
    // , center_ (c.center_)
    // , tangentCircles_ (c.tangentCircles_)
    // , gray_ (c.gray_)
    //{}

    circle (std::complex<float>& z, float k)
     : k_ (k)
     , r_ (1.0f / std::abs (k))
     , z_ (z)
     , center_ (z * (1.0f / k))
     , tangentCircles_ ()
     //, gray_ (255)
    {}

    bool isEqual (circle& c)
    {
      static float tolerance_f = 2.0f;

      bool equalR_b = std::abs (r_ - c.r_) < tolerance_f;
      bool equalX_b = std::abs (center_.real () - c.center_.real ()) < tolerance_f;
      bool equalY_b = std::abs (center_.imag () - c.center_.imag ()) < tolerance_f;

      return equalR_b && equalX_b && equalY_b;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (static_cast<int32_t> (center_.real ()), static_cast<int32_t> (center_.imag ()),
                             static_cast<int32_t> (r_),
                             olc::BLACK, 0xFF);
      //engine_in->FillCircle (static_cast<int32_t> (center_.x_), static_cast<int32_t> (center_.y_),
      //                       static_cast<int32_t> (r_),
      //                       gray_ ? olc::WHITE : olc::BLACK);
    }

    float               k_;
    float               r_;
    std::complex<float> z_;
    std::complex<float> center_;
    std::vector<circle> tangentCircles_;
    //uint8_t             gray_;
  };

  std::vector<circle> circles_;
  float               r1_;
  float               r2_;
  std::complex<float> z1_;
  std::complex<float> z2_;
  std::complex<float> center_;
  std::complex<float> touchPoint_;
  std::complex<float> mouse_;

  float find_r3 (std::complex<float>&, std::complex<float>&, float);
  std::complex<float> find_z3 (std::complex<float>&, float, float, float);
  circle thirdCircle (circle&, circle&, float);
  std::vector<circle> decartes (circle&, circle&, circle&);
  std::vector<circle> apollonian (circle&, float);
};

#endif // PGE_213_H
