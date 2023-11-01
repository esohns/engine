#ifndef PGE_214_H
#define PGE_214_H

#include <vector>

#include "olcPixelGameEngine.h"

class PGE_214
 : public olc::PixelGameEngine
{
 public:
  PGE_214 ();
  inline virtual ~PGE_214 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class complex
  {
   public:
    complex (const complex& c)
      : x_ (c.x_)
      , y_ (c.y_)
     {}

    complex (float x, float y)
     : x_ (x)
     , y_ (y)
    {}

    complex add (const complex& z) const
    {
      return complex (x_ + z.x_, y_ + z.y_);
    }

    complex minus (const complex& z) const
    {
      return complex (x_ - z.x_, y_ - z.y_);
    }

    complex mult (const complex& z) const
    {
      return complex (x_ * z.x_ - y_ * z.y_, x_ * z.y_ + y_ * z.x_);
    }

    complex scale (float s) const
    {
      return complex (x_ * s, y_ * s);
    }

    //complex sq () const
    //{
    //  return mult (*this);
    //}

    float modulus () const
    {
      return std::sqrt (x_ * x_ + y_ * y_);
    }

    float arg () const
    {
      return std::atan2 (y_, x_);
    }

    complex sqrt () const
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
    circle ()
     : k_ (0.0f)
     , r_ (0.0f)
     , z_ (0.0f, 0.0f)
     , center_ (0.0f, 0.0f)
     , tangentCircles_ ()
     , gray_ (255)
    {}

    circle (const circle& c)
     : k_ (c.k_)
     , r_ (c.r_)
     , z_ (c.z_)
     , center_ (c.center_)
     , tangentCircles_ (c.tangentCircles_)
     , gray_ (c.gray_)
    {}

    circle (complex& z, float k)
     : k_ (k)
     , r_ (1.0f / std::abs (k))
     , z_ (z)
     , center_ (z.scale (1.0f / k))
     , tangentCircles_ ()
     , gray_ (255)
    {}

    bool isEqual (const circle& c) const
    {
      const float tolerance_f = 1.0f;

      bool equalR_b = std::abs (r_ - c.r_) < tolerance_f;
      bool equalX_b = std::abs (center_.x_ - c.center_.x_) < tolerance_f;
      bool equalY_b = std::abs (center_.y_ - c.center_.y_) < tolerance_f;

      return equalR_b && equalX_b && equalY_b;
    }

    bool isLessThan (const circle& c) const
    {
      bool lessX_b = center_.x_ < c.center_.x_;
      bool lessY_b = center_.y_ < c.center_.y_;

      return lessX_b && lessY_b;
    }

    void draw (olc::PixelGameEngine* engine_in)
    {
      engine_in->DrawCircle (static_cast<int32_t> (center_.x_), static_cast<int32_t> (center_.y_),
                             static_cast<int32_t> (r_),
                             olc::WHITE, 0xFF);
      //engine_in->FillCircle (static_cast<int32_t> (center_.x_), static_cast<int32_t> (center_.y_),
      //                       static_cast<int32_t> (r_),
      //                       gray_ ? olc::WHITE : olc::BLACK);
    }

    float               k_;
    float               r_;
    complex             z_;
    complex             center_;
    std::vector<circle> tangentCircles_;
    uint8_t             gray_;
  };

  class reduce_vector_circle
  {
   public:
    reduce_vector_circle (PGE_214* instance,
                          std::vector<circle>& initial)
     : instance_ (instance)
     , initial_ (initial)
    {}

    std::vector<circle>& operator() (circle& lhs, circle& rhs)
    {
      std::vector<circle> result_a = instance_->apollonian (lhs);
      initial_.insert (initial_.end (), result_a.begin (), result_a.end ());
      result_a = instance_->apollonian (rhs);
      initial_.insert (initial_.end (), result_a.begin (), result_a.end ());

      return initial_;
    }

    std::vector<circle>& operator() (std::vector<circle>& lhs, circle& rhs)
    {
      std::vector<circle> result_a = instance_->apollonian (rhs);
      lhs.insert (lhs.end (), result_a.begin (), result_a.end ());

      return lhs;
    }
    std::vector<circle>& operator() (circle& lhs, std::vector<circle>& rhs)
    {
      std::vector<circle> result_a = instance_->apollonian (lhs);
      rhs.insert (rhs.end (), result_a.begin (), result_a.end ());

      return rhs;
    }

    std::vector<circle>& operator() (std::vector<circle>& lhs, std::vector<circle>& rhs)
    {
      lhs.insert (lhs.end (), rhs.begin (), rhs.end ());

      return lhs;
    }

   private:
    PGE_214* instance_;
    std::vector<circle>& initial_;
  };

  std::vector<circle> circles_;
  float               r1_;
  float               r2_;
  float               r3_;
  complex             z1_;
  complex             z2_;
  complex             z3_;

  std::vector<circle> decartes (circle&, circle&, circle&);
  std::vector<circle> apollonian (circle&);
};

#endif // PGE_214_H
