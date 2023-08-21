#ifndef PGE_134_H
#define PGE_134_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_5.h"

class PGE_134
 : public olc::PixelGameEngine
{
 public:
  PGE_134 ();
  inline virtual ~PGE_134 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class agent
  {
   public:
    olc::vf2d position_;
    float     z_;
    float     angle_;
     
    agent (float angle)
     : position_ (0.0f, Common_Tools::getRandomNumber (-50.0f, 50.0f))
     , z_ (0.0f)
     , angle_ (angle)
    {}
  };

  class system
  {
   public:
    std::vector<agent>                      agents_;
    std::vector<std::pair<agent*, agent*> > lines_;

    system ()
     : agents_ ()
     , lines_ ()
    {}

    void initialize ()
    {
      for (int i = 0; i < ENGINE_PGE_134_DEFAULT_NUMBER_OF_AGENTS; ++i)
        agents_.push_back (agent (i / 275.0f));
    }

    void update (int frameCount)
    {
      for (std::vector<agent>::iterator iterator = agents_.begin ();
           iterator != agents_.end ();
           ++iterator)
      {
        float d = (*iterator).angle_ + frameCount / 100.0f;
        (*iterator).position_.x = std::cos (d) * 200.0f;
        (*iterator).z_ = std::sin (d) + 1.1f;
      } // end FOR
    }

    void relativeNeighborhoodGraph ()
    {
      for (int i = static_cast<int> (agents_.size ()); i--;)
        for (int j = i; j--; )
        {
          float d = distance (agents_[j], agents_[i]);
          bool condition_b = true;
          for (std::vector<agent>::iterator iterator = agents_.begin ();
               iterator != agents_.end ();
               ++iterator)
          {
            float d_2 = distance (*iterator, agents_[i]);
            float d_3 = distance (*iterator, agents_[j]);
            if (d > d_2 && d > d_3)
            {
              condition_b = false;
              break;
            } // end IF
          } // end FOR
          if (condition_b)
            lines_.push_back (std::make_pair (&agents_[i], &agents_[j]));
        } // end FOR
    }

    float distance (agent& a, agent& b)
    {
      return (a.position_.x - b.position_.x) * (a.position_.x - b.position_.x) + (a.position_.y - b.position_.y) * (a.position_.y - b.position_.y) +
              (a.z_ - b.z_) * (a.z_ - b.z_);
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      olc::Pixel color;
      float r, g, b;
      for (std::vector<std::pair<agent*, agent*> >::iterator iterator = lines_.begin ();
           iterator != lines_.end ();
           ++iterator)
      {
        float ax = (*iterator).first->position_.x / (*iterator).first->z_;
        float ay = (*iterator).first->position_.y / (*iterator).first->z_;
        float bx = (*iterator).second->position_.x / (*iterator).second->z_;
        float by = (*iterator).second->position_.y / (*iterator).second->z_;

        float hue =
          Common_GL_Tools::map (std::abs ((1.0f / (*iterator).second->z_ * 24.0f) - 24.0f), 0.0f, 255.0f, 0.0f, 360.0f);
        Common_Image_Tools::HSVToRGB (hue, 1.0f, 1.0f, r, g, b);
        color.r = static_cast<uint8_t> (r * 255.0f);
        color.g = static_cast<uint8_t> (g * 255.0f);
        color.b = static_cast<uint8_t> (b * 255.0f);

        //strokeWeight(1 / b.z);
        engine_in->DrawLine (half_width_i + static_cast<int32_t> (ax), half_height_i + static_cast<int32_t> (ay),
                             half_width_i + static_cast<int32_t> (bx), half_height_i + static_cast<int32_t> (by),
                             color, 0xFFFFFFFF);
      } // end FOR
    }
  };

  system system_;
};

#endif // PGE_134_H
