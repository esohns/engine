#ifndef PGE_90_H
#define PGE_90_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_4.h"

class PGE_90
 : public olc::PixelGameEngine
{
 public:
  PGE_90 ();
  inline virtual ~PGE_90 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class field;
  class particle
  {
   public:
    particle (float x, float y, field* parent, bool forward)
     : position_ (x, y)
     , forward_ (forward)
     , parent_ (parent)
     , step_ (0.01f / parent->m_)
    {
      step_ *= forward ? 1.0f : -1.0f;
    }

    void update (olc::PixelGameEngine* engine_in,
                 olc::Pixel& color)
    {
      olc::vf2d point_s = move_point ();
      if (!ENGINE_PGE_90_DEFAULT_FRAME)
        position_ =
          Common_Tools::getRandomNumber (0.0f, 1.0f) < 0.5f ? point_s 
                                                            : olc::vf2d (std::min (std::max (-1.0f, point_s.x), 1.0f), std::min (std::max (-1.0f, point_s.y), 1.0f));
      else
        position_ =
          olc::vf2d (std::min (std::max (-1.0f, point_s.x), 1.0f), std::min (std::max (-1.0f, point_s.y), 1.0f));

      int32_t canvas_size = std::min (engine_in->ScreenWidth (), engine_in->ScreenHeight ());
      olc::vf2d position_s (canvas_size * (ENGINE_PGE_90_DEFAULT_BORDER + ((1 + position_.x) / 2) * (1 - 2 * ENGINE_PGE_90_DEFAULT_BORDER)),
                            canvas_size * (ENGINE_PGE_90_DEFAULT_BORDER + ((1 + position_.y) / 2) * (1 - 2 * ENGINE_PGE_90_DEFAULT_BORDER)));
      engine_in->Draw (position_s, color);
    }

    olc::vf2d move_point ()
    {
      float dx = get_delta (0);
      float dy = get_delta (1);

      olc::vf2d position_s = position_;
      position_s += {dx * step_, dy * step_};
      return position_s;
    }

    float get_delta (int32_t i)
    {
      float value_f = 0.0f;
      int32_t n = 8;
      float x = position_.x;
      float y = position_.y;
      value_f +=
        parent_->a_[0 + n * i] * std::sin (x * parent_->b_[0 + n * i] + parent_->c_[0 + n * i]);
      value_f +=
        parent_->a_[1 + n * i] * std::sin (y * parent_->b_[1 + n * i] + parent_->c_[1 + n * i]);
      value_f +=
        parent_->a_[2 + n * i] * std::cos (x * parent_->b_[2 + n * i] + parent_->c_[2 + n * i]);
      value_f +=
        parent_->a_[3 + n * i] * std::cos (y * parent_->b_[3 + n * i] + parent_->c_[3 + n * i]);
      value_f +=
        parent_->a_[4 + n * i] * x + parent_->b_[4 + n * i] * x * x + parent_->c_[4 + n * i] * x * x * x;
      value_f +=
        parent_->a_[5 + n * i] * y + parent_->b_[5 + n * i] * y * y + parent_->c_[5 + n * i] * y * y * y;
      value_f +=
        parent_->a_[6 + n * i] * x * y + parent_->b_[6 + n * i] * x * x * y + parent_->c_[6 + n * i] * x * y * y;
      value_f +=
        parent_->a_[7 + n * i] * x * x * y * y + parent_->b_[7 + n * i] * x * x * y * y * y + parent_->c_[7 + n * i] * x * x * x * y * y;
      return value_f;
    }

    olc::vf2d position_;
    bool      forward_;
    field*    parent_;
    float     step_;
  };

  class field
  {
   public:
    field ()
     : p_ ()
     , m_ (8.0f + Common_Tools::getRandomNumber (0.0f, 1.0f) * 4.0f)
     , a_ ()
     , b_ ()
     , c_ ()
    {
      reset ();
    }

    ~field ()
    {
      for (std::vector<particle*>::iterator iterator = p_.begin ();
           iterator != p_.end ();
           ++iterator)
        delete *iterator;
    }

    void update (olc::PixelGameEngine* engine_in,
                 olc::Pixel& color)
    {
      for (size_t i = 0; i < p_.size (); i++)
        p_[i]->update (engine_in, color);
    }

    void initialize_points (int32_t n)
    {
      for (std::vector<particle*>::iterator iterator = p_.begin ();
           iterator != p_.end ();
           ++iterator)
        delete *iterator;
      p_.clear ();

      for (int i = 0; i < n; i++)
      {
        float x = Common_Tools::getRandomNumber (-1.0f, 1.0f);
        float y = Common_Tools::getRandomNumber (-1.0f, 1.0f);
        p_.push_back (new particle (x, y, this, true));
        p_.push_back (new particle (x, y, this, false));
      } // end FOR
    }

    void reset ()
    {
      a_.clear (); b_.clear (); c_.clear ();

      get_random_array (a_, -m_, m_, 32);
      get_random_array (b_, -m_, m_, 32);
      get_random_array (c_, -m_, m_, 32);
    }

    void get_random_array (std::vector<float>& array_in, float min, float max, int32_t amount)
    {
      array_in.clear ();
      for (int i = 0; i < amount; i++)
        array_in.push_back (min + Common_Tools::getRandomNumber (0.0f, 1.0f) * (max - min));
    }

    std::vector<particle*> p_;
    float                  m_;
    std::vector<float>     a_;
    std::vector<float>     b_;
    std::vector<float>     c_;
  };

  field      field_;
  olc::Pixel bg_color_;
  olc::Pixel color_;
};

#endif // PGE_90_H
