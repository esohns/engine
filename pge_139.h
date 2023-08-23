#ifndef PGE_139_H
#define PGE_139_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_5.h"

class PGE_139
 : public olc::PixelGameEngine
{
 public:
  PGE_139 ();
  inline virtual ~PGE_139 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class vertex
  {
   public:
    float alpha_;
    float beta_;
    float x;
    float y;
    float z;

    vertex ()
     : alpha_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , beta_ (Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI)))
     , x (0.0f)
     , y (0.0f)
     , z (0.0f)
    {}

    void update ()
    {
      beta_ += ENGINE_PGE_139_DEFAULT_ROTATION_SPEED;
      x = std::sin (alpha_) * std::cos (beta_);
      y = std::cos (alpha_);
      z = std::sin (alpha_) * std::sin (beta_);
    }
  };

  class vertices
  {
   public:
    std::vector<vertex> vertices_;

    void clear ()
    {
      vertices_.clear ();
    }

    void spawn (int frameCount)
    {
      if (frameCount % 10 != 0 ||
          vertices_.size () >= ENGINE_PGE_139_DEFAULT_NUMBER_OF_VERTICES) return;
      vertices_.push_back (vertex ());
    }

    float dist2 (vertex& a, vertex& b)
    {
      return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z);
    }

    void connectVertices (olc::PixelGameEngine* engine_in,
                          float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      for (size_t i = vertices_.size (); i--;)
        for (size_t j = i; j--;)
        {
          float d = dist2 (vertices_[j], vertices_[i]);
          bool condition_b = true;
          for (std::vector<vertex>::iterator iterator = vertices_.begin();
               iterator != vertices_.end();
               ++iterator)
            if (d > dist2 (vertices_[i], *iterator) && d > dist2 (vertices_[j], *iterator))
            {
              condition_b = false;
              break;
            } // end IF
          if (condition_b)
          {
            float z1 = 3.0f - vertices_[i].z;
            float x1 = vertices_[i].x / z1;
            float y1 = vertices_[i].y / z1;

            float z2 = 3 - vertices_[j].z;
            float x2 = vertices_[j].x / z2;
            float y2 = vertices_[j].y / z2;

            //strokeWeight(10 / (z1 + z2));
            engine_in->DrawLine (half_width_i + static_cast<int32_t> (x1 * zoom), half_height_i + static_cast<int32_t> (y1 * zoom),
                                 half_width_i + static_cast<int32_t> (x2 * zoom), half_height_i + static_cast<int32_t> (y2 * zoom),
                                 olc::WHITE, 0xFFFFFFFF);
          } // end IF
        } // end FOR
    }

    void update ()
    {
      for (std::vector<vertex>::iterator iterator = vertices_.begin();
           iterator != vertices_.end();
           ++iterator)
        (*iterator).update ();
    }

    void show (olc::PixelGameEngine* engine_in,
               float zoom)
    {
      static int32_t half_width_i = engine_in->ScreenWidth () / 2;
      static int32_t half_height_i = engine_in->ScreenHeight () / 2;

      for (std::vector<vertex>::iterator iterator = vertices_.begin();
           iterator != vertices_.end();
           ++iterator)
      {
        float z = 3.0f - (*iterator).z;
        float x = (*iterator).x / z;
        float y = (*iterator).y / z;
        engine_in->FillCircle (half_width_i + static_cast<int32_t> (x * zoom), half_height_i + static_cast<int32_t> (y * zoom), static_cast<int32_t> (15.0f / z), olc::WHITE);
      } // end FOR
    }
  };

  vertices vertices_;
  float    zoom_;
};

#endif // PGE_139_H
