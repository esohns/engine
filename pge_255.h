#ifndef PGE_255_H
#define PGE_255_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_11.h"

class PGE_255
 : public olc::PixelGameEngine
{
 public:
  PGE_255 ();
  inline virtual ~PGE_255 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct joint
  {
    float      angle;
    float      angularVelocity;
    olc::Pixel color;
  };

  class string
  {
   public:
    string (int32_t x, olc::PixelGameEngine* engine_in)
     : root_ (static_cast<float> (x), static_cast<float> (engine_in->ScreenHeight () - 1))
     , joints_ ()
    {
      struct joint joint_s;
      joint_s.angle = 0.0f;
      joint_s.angularVelocity = 0.0f;
      for (int i = 0; i < ENGINE_PGE_255_DEFAULT_NUMBER_OF_JOINTS; ++i)
      {
        joint_s.color = olc::Pixel (98, 120, 66, Common_Tools::getRandomNumber (0, 255));
        joints_.push_back (joint_s);
      } // end FOR
    }

    void collide (const olc::vf2d& player)
    {
      olc::vf2d pos = root_;
      float totalAngle = -static_cast<float> (M_PI) / 2.0f;
      for (std::vector<struct joint>::iterator iterator = joints_.begin ();
           iterator != joints_.end ();
           ++iterator)
      {
        float distance_f = pos.dist (player);
        static olc::vf2d temp (static_cast<float> (ENGINE_PGE_255_DEFAULT_JOINT_LENGTH), 0.0f);
        olc::vf2d temp_rot;
        if (distance_f < static_cast<float> (ENGINE_PGE_255_DEFAULT_PLAYER_RADIUS))
        {
          float angle_f = totalAngle - ENGINE_PGE_255_DEFAULT_F;
          temp_rot.x = std::cos (angle_f) * temp.x - std::sin (angle_f) * temp.y;
          temp_rot.y = std::sin (angle_f) * temp.x + std::cos (angle_f) * temp.y;
          olc::vf2d sampleL = pos + temp_rot;
          angle_f = totalAngle + ENGINE_PGE_255_DEFAULT_F;
          temp_rot.x = std::cos (angle_f) * temp.x - std::sin (angle_f) * temp.y;
          temp_rot.y = std::sin (angle_f) * temp.x + std::cos (angle_f) * temp.y;
          olc::vf2d sampleR = pos + temp_rot;
          float d =
            Common_GL_Tools::map (distance_f, 0.0f, static_cast<float> (ENGINE_PGE_255_DEFAULT_PLAYER_RADIUS), 5.0f, 1.0f);
          (*iterator).angle += ENGINE_PGE_255_DEFAULT_F * d * (sampleL.dist (player) > sampleR.dist (player) ? -1.0f : 1.0f);
          (*iterator).angularVelocity *= 0.9f;
        } // end IF
        totalAngle += (*iterator).angle;

        temp_rot.x = std::cos (totalAngle) * temp.x - std::sin (totalAngle) * temp.y;
        temp_rot.y = std::sin (totalAngle) * temp.x + std::cos (totalAngle) * temp.y;
        pos += temp_rot;
      } // end FOR
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d pos = root_;
      float totalAngle = -static_cast<float> (M_PI) / 2.0f;
      static olc::vf2d temp (static_cast<float> (ENGINE_PGE_255_DEFAULT_JOINT_LENGTH), 0.0f);
      olc::vf2d temp_rot;
      for (std::vector<struct joint>::iterator iterator = joints_.begin ();
           iterator != joints_.end ();
           ++iterator)
      {
        (*iterator).angle += (*iterator).angularVelocity;
        totalAngle += (*iterator).angle;
        (*iterator).angularVelocity -= std::sin ((*iterator).angle) / 200.0f;
        (*iterator).angle *= ENGINE_PGE_255_DEFAULT_FRICTION;
        olc::vf2d old = pos;
        temp_rot.x = std::cos (totalAngle) * temp.x - std::sin (totalAngle) * temp.y;
        temp_rot.y = std::sin (totalAngle) * temp.x + std::cos (totalAngle) * temp.y;
        pos += temp_rot;
        engine_in->DrawLine (old, pos, olc::BLACK, 0xFFFFFFFF);
        engine_in->FillCircle (pos, ENGINE_PGE_255_DEFAULT_BEAN_RADIUS, (*iterator).color);
      } // end FOR
    }

    olc::vf2d                 root_;
    std::vector<struct joint> joints_;
  };

  std::vector<string> strings_;
};

#endif // PGE_255_H
