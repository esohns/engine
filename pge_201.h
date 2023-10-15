#ifndef PGE_201_H
#define PGE_201_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "defines_9.h"

class PGE_201
 : public olc::PixelGameEngine
{
 public:
  PGE_201 ();
  inline virtual ~PGE_201 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class line
  {
   public:
    line (float angle)
     : angle_ (angle)
     , length_ (0.0f)
     , color_ (olc::BLACK)
     , state_ (0)
    {
      color_.r = 128 + Common_Tools::getRandomNumber (0, 127);
      color_.g = 128 + Common_Tools::getRandomNumber (0, 127);
      color_.b = 128 + Common_Tools::getRandomNumber (0, 127);
    }

    void act (olc::PixelGameEngine* engine_in,
              float parentLength)
    {
      if (state_ == 0)
      {
        if (parentLength > 50.0f && Common_Tools::testRandomProbability (0.005f))
          state_ = 1;
      } // end IF
      else
      {
        if (length_ < 100.0f)
          length_ += 2.0f;
      } // end ELSE

      float x =
        std::cos (angle_) * ((parentLength) - (length_)) + (engine_in->ScreenWidth () / 2.0f);
      float y =
        std::sin (angle_) * ((parentLength) - (length_)) + (engine_in->ScreenHeight () / 2.0f);
      float dx =
        std::cos (angle_) * (parentLength) + (engine_in->ScreenWidth () / 2.0f);
      float dy =
        std::sin (angle_) * (parentLength) + (engine_in->ScreenHeight () / 2.0f);
      engine_in->DrawLine (static_cast<int32_t> (x), static_cast<int32_t> (y),
                           static_cast<int32_t> (dx), static_cast<int32_t> (dy),
                           color_, 0xFFFFFFFF);

      engine_in->FillCircle (static_cast<int32_t> ((x + dx) / 2.0f), static_cast<int32_t> ((y + dy) / 2.0f),
                             static_cast<int32_t> (parentLength / 50.0f),
                             color_);
      engine_in->FillCircle (static_cast<int32_t> (x), static_cast<int32_t> (y),
                             static_cast<int32_t> (parentLength / 70.0f),
                             color_);
    }

    float      angle_;
    float      length_;
    olc::Pixel color_;
    int        state_;
  };

  class arc
  {
   public:
    arc (float start, float end) // degrees
     : lines_ ()
     , start_ ((static_cast<float> (M_PI) / 180.0f) * start)
     , end_ ((static_cast<float> (M_PI) / 180.0f) * end)
     , length_ (0.0f)
     , color_ (olc::BLACK)
     //, lineAngle_ (0.0f)
     //, lineLength_ (0.0f)
     , state_ (0)
    {
      color_.r = 192 + Common_Tools::getRandomNumber (0, 64);
      color_.g = 192 + Common_Tools::getRandomNumber (0, 64);
      color_.b = 192 + Common_Tools::getRandomNumber (0, 64);

      int c = Common_Tools::getRandomNumber (0, 4);
      for (int i = 0; i < c; i++)
      {
        line l (static_cast<float> (M_PI) / 180.0f * (Common_Tools::getRandomNumber (0.0f, end - start) + start));
        lines_.push_back (l);
      } // end FOR
    }

    inline bool operator== (arc& rhs) { return (start_ == rhs.start_ && end_ == rhs.end_ && length_ == rhs.length_); }

    void act (olc::PixelGameEngine* engine_in)
    {
      for (std::vector<line>::iterator iterator = lines_.begin ();
           iterator != lines_.end ();
           ++iterator)
        (*iterator).act (engine_in, length_);

      switch (state_)
      {
        case 0:
          if (Common_Tools::testRandomProbability (0.005f))
            state_ = Common_Tools::testRandomProbability (0.5f) ? 1 : 2;
          break;
        case 1:
          start_ += (static_cast<float> (M_PI) / 180.0f) * 0.5f;
          end_ += (static_cast<float> (M_PI) / 180.0f) * 0.5f;
          for (std::vector<line>::iterator iterator = lines_.begin();
               iterator != lines_.end();
               ++iterator)
            (*iterator).angle_ += (static_cast<float> (M_PI) / 180.0f) * 0.5f;

          if (Common_Tools::testRandomProbability (0.05f))
            state_ = 0;
          break;
        case 2:
          start_ -= (static_cast<float> (M_PI) / 180.0f) * 0.5f;
          end_ -= (static_cast<float> (M_PI) / 180.0f) * 0.5f;
          for (std::vector<line>::iterator iterator = lines_.begin();
               iterator != lines_.end();
               ++iterator)
            (*iterator).angle_ -= (static_cast<float> (M_PI) / 180.0f) * 0.5f;

          if (Common_Tools::testRandomProbability (0.05f))
            state_ = 0;
          break;
      } // end SWITCH

      olc::vf2d center (engine_in->ScreenWidth ()  / 2.0f,
                        engine_in->ScreenHeight () / 2.0f);
      draw_circle_arc (engine_in, center, length_, start_, end_, color_);
      length_ += 1;
    }

    void draw_circle_arc (olc::PixelGameEngine* engine_in, olc::vf2d& center, float radius, float angle_from, float angle_to, olc::Pixel& color)
    {
      std::vector<olc::vf2d> points_arc;

      for (int i = 0; i <= ENGINE_PGE_201_DEFAULT_ARC_RESOLUTION + 1; ++i)
      {
        float angle_point =
          (angle_from + i * (angle_to - angle_from) / static_cast<float> (ENGINE_PGE_201_DEFAULT_ARC_RESOLUTION));
        olc::vf2d point_s (static_cast<float> (std::cos (angle_point)), static_cast<float> (std::sin (angle_point)));
        point_s *= radius;
        point_s += center;
        points_arc.push_back (point_s);
      } // end FOR

      for (int i = 0; i <= ENGINE_PGE_201_DEFAULT_ARC_RESOLUTION; i++)
        engine_in->DrawLine (points_arc[i], points_arc[i + 1], color, 0xFFFFFFFF);
    }

    std::vector<line> lines_;
    float             start_;
    float             end_;
    float             length_;
    olc::Pixel        color_;
    //float             lineAngle_;
    //float             lineLength_;
    int               state_;
  };

  std::vector<arc> arcs_;

  void createArc ();
};

#endif // PGE_201_H
