#ifndef PGE_352_H
#define PGE_352_H

#include <vector>

#if defined (GLM_SUPPORT)
#include "glm/glm.hpp"
#endif // GLM_SUPPORT

#include "olcPixelGameEngine.h"

#include "common_gl_tools.h"

#include "defines_16.h"

class PGE_352
 : public olc::PixelGameEngine
{
 public:
  PGE_352 ();
  inline virtual ~PGE_352 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class node
  {
   public:
    node (float x, float y, float z)
     : vec3_ (x, y, z)
     , vec2_ (0.0f, 0.0f)
     , scaling_ (1.0f)
    {}

    void rotate ()
    {
      float x_rotated =
        vec3_.x * std::cos (ENGINE_PGE_352_DEFAULT_ROTATE_SPEED) - vec3_.z * std::sin (ENGINE_PGE_352_DEFAULT_ROTATE_SPEED);
      float y_rotated =
        vec3_.x * std::sin (ENGINE_PGE_352_DEFAULT_ROTATE_SPEED) + vec3_.z * std::cos (ENGINE_PGE_352_DEFAULT_ROTATE_SPEED);
      vec3_.x = x_rotated;
      vec3_.z = y_rotated;
    }

    void update (int frameCount, float zoom)
    {
      float magnitude =
        Common_GL_Tools::map (std::cos (frameCount / 60.0f), -1.0f, 1.0f, 3.0f, 1.0f);
      float depth = magnitude + vec3_.z;
      vec2_.x = (vec3_.x / depth) * zoom;
      vec2_.y = (vec3_.y / depth) * zoom;
      scaling_ = 10.0f / depth;
    }

    void show (olc::PixelGameEngine* engine, olc::vf2d& center)
    {
      engine->FillCircle (center + vec2_, static_cast<int32_t> (scaling_ / 2.0f), olc::BLACK);
    }

    glm::vec3 vec3_;
    olc::vf2d vec2_;
    float     scaling_;
  };

  class link
  {
   public:
    link (node& node1, node& node2)
     : node1_ (&node1)
     , node2_ (&node2)
     , age_ (0.0f)
    {}

    void show (olc::PixelGameEngine* engine, olc::vf2d& center)
    {
      //float totalScaling = node1_->scaling_ + node2_->scaling_;
      //strokeWeight(totalScaling / 4);
      if (age_ >= 1.0f)
      {
        engine->DrawLine (node1_->vec2_ + center,
                          node2_->vec2_ + center,
                          olc::BLACK, 0xFFFFFFFF);
        return;
      } // end IF

      age_ += ENGINE_PGE_352_DEFAULT_GROW_SPEED;
      olc::vf2d difference = node2_->vec2_ - node1_->vec2_;
      olc::vf2d target = node1_->vec2_ + (difference * age_);
      engine->DrawLine (node1_->vec2_ + center,
                        target + center,
                        olc::RED, 0xFFFFFFFF);
    }

    node* node1_;
    node* node2_;
    float age_;
  };

  void makeLinks ();

  std::vector<link> links_;
  std::vector<node> nodes_;
  float             zoom_;
};

#endif // PGE_352_H
