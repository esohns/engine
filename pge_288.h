#ifndef PGE_288_H
#define PGE_288_H

#include <vector>

#include "noise/noise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_13.h"

class PGE_288
 : public olc::PixelGameEngine
{
 public:
  PGE_288 ();
  inline virtual ~PGE_288 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class arc
  {
   public:
    arc (float x, float y, float r, float a0, float a1, float h)
     : center_ (x, y)
     , radius_ (r)
     , beginAngle_ (a0)
     , endAngle_ (a1)
     , hue_ (h)
    {}

    void show (float alpha, olc::PixelGameEngine* engine)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (hue_, 1.0f, 1.0f, r, g, b);
      olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), static_cast<uint8_t> (alpha * 255.0f));

      draw_circle_arc (center_, radius_, beginAngle_, endAngle_, color, engine);
    }

    olc::vf2d center_;
    float     radius_;
    float     beginAngle_;
    float     endAngle_;
    float     hue_;

   private:
    void draw_circle_arc (olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color, olc::PixelGameEngine* engine)
    {
      std::vector<olc::vf2d> points_arc;

      for (int i = 0; i <= ENGINE_PGE_288_DEFAULT_ARC_RESOLUTION + 1; ++i)
      {
        float angle_point =
          (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_288_DEFAULT_ARC_RESOLUTION));
        olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
        point_s *= radius;
        point_s += center;
        points_arc.push_back (point_s);
      } // end FOR

      for (int i = 0; i <= ENGINE_PGE_288_DEFAULT_ARC_RESOLUTION; i++)
        engine->DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
    }
  };

  std::vector<arc>      arcs_;

  float                 a_;
  float                 h_;
  float                 da_;
  float                 t_;
  float                 l_;
  float                 t0_;
  float                 t1_;
  float                 rad_;

  float                 dt_;
  float                 dt0_;
  float                 dt1_;

  noise::module::Perlin noise_;

  void setA ()
  {
    a_ += da_;
  }

  void setL ()
  {
    t0_ += dt0_;
    l_ =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (t0_, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 50.0f);
  }

  void setRad ()
  {
    t1_ += dt1_;
    rad_ =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (t1_, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, static_cast<float> (M_PI));
  }

  void setDa ()
  {
    t_ += dt_;
    da_ =
      Common_GL_Tools::map (static_cast<float> (noise_.GetValue (t_, 0.0, 0.0)), -1.0f, 1.0f, -0.1f, 0.1f);
  }

  void setAll ()
  {
    h_ = std::fmod (h_ + 0.5f, 360.0f);

    setA ();
    setL ();
    setRad ();
    setDa ();
  }

  void addArcs (int, int, int, int);
};

#endif // PGE_288_H
