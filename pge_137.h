#ifndef PGE_137_H
#define PGE_137_H

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_5.h"

class PGE_137
 : public olc::PixelGameEngine
{
 public:
  PGE_137 ();
  inline virtual ~PGE_137 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class vertex
  {
   public:
    float     radius_;
    float     theta_;
    olc::vf2d position_;
    float     z_;
    int       blinker_;

    vertex (float radius, float theta)
     : radius_ (radius)
     , theta_ (theta)
     , position_ ()
     , z_ (ENGINE_PGE_137_DEFAULT_ZMAX)
     , blinker_ (Common_Tools::getRandomNumber (16, 64))
    {}

    void update ()
    {
      theta_ += ENGINE_PGE_137_DEFAULT_ROTATION_SPEED;
      position_.x = std::cos (theta_) * radius_;
      position_.y = std::sin (theta_) * radius_;
      z_ -= 0.025f;
    }
  };
  
  class tunnel
  {
   public:
    std::vector<vertex> vertices_;

    void spawn (noise::module::Perlin* noise_in,
                int32_t frameCount)
    {
      float theta =
        Common_GL_Tools::map (static_cast<float> (noise_in->GetValue (frameCount / 10.0, 0.0, 0.0)),
                              -1.0f, 1.0f, 0.0f, 1.0f) * static_cast<float> (M_PI) * 2.0f;
      vertices_.push_back (vertex (0.9f, theta));
      vertices_.push_back (vertex (1.0f, theta));
      vertices_.push_back (vertex (1.1f, theta));
    }

    float dist2 (vertex& a, vertex& b)
    {
      return (a.position_.x - b.position_.x) * (a.position_.x - b.position_.x) + (a.position_.y - b.position_.y) * (a.position_.y - b.position_.y) + (a.z_ - b.z_) * (a.z_ - b.z_);
    }

    void connectVertices (olc::PixelGameEngine* engine_in,
                          float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      for (int i = static_cast<int> (vertices_.size ()); i--;)
        for (int j = i; j--;)
        {
          float d = dist2 (vertices_[j], vertices_[i]);
          bool condition_b = true;
          for (std::vector<vertex>::iterator iterator = vertices_.begin ();
               iterator != vertices_.end ();
               ++iterator)
            if (d > dist2 (vertices_[i], *iterator) && d > dist2 (vertices_[j], *iterator))
            {
              condition_b = false;
              break;
            } // end IF
          if (condition_b)
          {
            float z1 = 3.0f - vertices_[i].z_;
            float x1 = vertices_[i].position_.x / z1;
            float y1 = vertices_[i].position_.y / z1;

            float z2 = 3.0f - vertices_[j].z_;
            float x2 = vertices_[j].position_.x / z2;
            float y2 = vertices_[j].position_.y / z2;

            //strokeWeight(abs(6 / (z1 + z2)));
            engine_in->DrawLine (half_width_i + static_cast<int32_t> (x1 * zoom), half_height_i + static_cast<int32_t> (y1 * zoom),
                                 half_width_i + static_cast<int32_t> (x2 * zoom), half_height_i + static_cast<int32_t> (y2 * zoom),
                                 olc::WHITE, 0xFFFFFFFF);
          } // end IF
        } // end FOR
    }

    void update ()
    {
      for (std::vector<vertex>::iterator iterator = vertices_.begin ();
            iterator != vertices_.end ();
            ++iterator)
        (*iterator).update ();

      vertices_.erase (std::remove_if (vertices_.begin (), vertices_.end (),
                                       [] (vertex& v) { return v.z_ <= ENGINE_PGE_137_DEFAULT_ZMIN * 1.03f; }),
                       vertices_.end ());
    }

    void blink (int32_t frameCount,
                olc::PixelGameEngine* engine_in,
                float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      for (std::vector<vertex>::iterator iterator = vertices_.begin ();
           iterator != vertices_.end ();
           ++iterator)
      {
        if (frameCount & (*iterator).blinker_) continue;
        float z = 3.0f - (*iterator).z_;
        float x = (*iterator).position_.x / z;
        float y = (*iterator).position_.y / z;
        engine_in->FillCircle (half_width_i + static_cast<int32_t> (x * zoom), half_height_i + static_cast<int32_t> (y * zoom),
                               static_cast<int32_t> (20.0f / std::abs (z)),
                               olc::CYAN);
      } // end FOR
    }
  };

  tunnel                tunnel_;
  noise::module::Perlin noise_;
  float                 zoom_;
};

#endif // PGE_137_H
