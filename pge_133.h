#ifndef PGE_133_H
#define PGE_133_H

#include "olcPixelGameEngine.h"

#include "common_image_tools.h"

#include "defines_5.h"

class PGE_133
 : public olc::PixelGameEngine
{
 public:
  PGE_133 ();
  virtual ~PGE_133 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct segment
  {
    olc::vf2d position_;
    float     angle_;
  };

  class strand
  {
   public:
    olc::vf2d                   root_;
    float                       hue_;
    std::vector<struct segment> positions_;

    strand (float x, float y, float hue)
     : root_ (x, y)
     , hue_ (hue)
     , positions_ (ENGINE_PGE_133_DEFAULT_NUMBER_OF_SEGMENTS)
    {}

    void calcAngles (olc::PixelGameEngine* engine_in)
    {
      struct segment parent;
      parent.position_ = {static_cast<float> (engine_in->GetMouseX ()), static_cast<float> (engine_in->GetMouseY ())};
      parent.angle_ = 0.0f;
      for (std::vector<struct segment>::iterator iterator = positions_.begin ();
           iterator != positions_.end ();
           ++iterator)
      {
        float dx = parent.position_.x - (*iterator).position_.x;
        float dy = parent.position_.y - (*iterator).position_.y;
        (*iterator).angle_ = std::atan2 (dy, dx);
        (*iterator).position_.x = parent.position_.x - std::cos ((*iterator).angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH;
        (*iterator).position_.y = parent.position_.y - std::sin ((*iterator).angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH;
        parent = *iterator;
      } // end FOR
    }

    void calcPositions ()
    {
      positions_[positions_.size() - 1].position_.x = root_.x;
      positions_[positions_.size() - 1].position_.y = root_.y;
      for (int j = static_cast<int> (positions_.size () - 1); j; j--)
      {
        positions_[j - 1].position_.x = positions_[j].position_.x + std::cos (positions_[j].angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH;
        positions_[j - 1].position_.y = positions_[j].position_.y + std::sin (positions_[j].angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH;
      } // end FOR
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      float r, g, b;
      olc::Pixel color;
      Common_Image_Tools::HSVToRGB (hue_, 1.0f, 0.8f, r, g, b);
      color.r = static_cast<uint8_t> (r * 255.0f);
      color.g = static_cast<uint8_t> (g * 255.0f);
      color.b = static_cast<uint8_t> (b * 255.0f);
      color.a = static_cast<uint8_t> (0.8f * 255.0f);
      for (std::vector<struct segment>::iterator iterator = positions_.begin ();
           iterator != positions_.end ();
           ++iterator)
        engine_in->DrawLine (static_cast<int32_t> ((*iterator).position_.x),
                             static_cast<int32_t> ((*iterator).position_.y),
                             static_cast<int32_t> ((*iterator).position_.x + std::cos ((*iterator).angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH),
                             static_cast<int32_t> ((*iterator).position_.y + std::sin ((*iterator).angle_) * ENGINE_PGE_133_DEFAULT_SEGMENT_LENGTH),
                             color, 0xFFFFFFFF);
    }
  };

  std::vector<strand*> strands_;
};

#endif // PGE_133_H
