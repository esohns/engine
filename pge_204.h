#ifndef PGE_204_H
#define PGE_204_H

#include "noise/noise.h"
//#include "opensimplexnoise.h"

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_9.h"

class PGE_204
 : public olc::PixelGameEngine
{
 public:
  PGE_204 ();
  inline virtual ~PGE_204 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  struct joint
  {
    float      theta;
    float      vTheta;
    olc::Pixel color;
  };

  class string
  {
   public:
    string (float x, float y)
     : joints_ ()
     , root_ (x, y)
    {
      struct joint joint_s;
      joint_s.theta = 0.0f;
      joint_s.vTheta = 0.0f;
      joint_s.color.r = 150;
      joint_s.color.g = 81;
      joint_s.color.b = 77;
      for (int i = 0; i < 20; i++)
      {
        joint_s.color.a = Common_Tools::getRandomNumber (0, 255);
        joints_.push_back (joint_s);
      } // end FOR

      applyRandomForce ();
    }

    void applyWind (noise::module::Perlin* noise_in,
                    int32_t frameCount_in)
    {
      int i = 0;
      for (std::vector<struct joint>::iterator iterator = joints_.begin ();
           iterator != joints_.end ();
           ++iterator, ++i)
      {
        float n =
          static_cast<float> (Common_GL_Tools::map (noise_in->GetValue (root_.x / 999.0f, i / 99.0f, frameCount_in / 99.0f),
                                                    -1.0, 1.0, 0.0, 1.0));
        (*iterator).vTheta += (n - 0.45f) / 400.0f;
      } // end FOR
    }

    void applyRandomForce ()
    {
      for (std::vector<struct joint>::iterator iterator = joints_.begin ();
           iterator != joints_.end ();
           ++iterator)
        (*iterator).vTheta += Common_Tools::getRandomNumber (-0.25f, 0.25f);
    }

    void update (olc::PixelGameEngine* engine_in)
    {
      olc::vf2d pos = root_;
      float phi = static_cast<float> (M_PI_2);

      for (std::vector<struct joint>::iterator iterator = joints_.begin ();
           iterator != joints_.end ();
           ++iterator)
      {
        (*iterator).theta += (*iterator).vTheta;
        phi += (*iterator).theta;
        (*iterator).vTheta -= std::sin ((*iterator).theta) / 200.0f;
        (*iterator).theta *= ENGINE_PGE_204_DEFAULT_FRICTION;
        olc::vf2d old = pos;
        olc::vf2d temp (static_cast<float> (ENGINE_PGE_204_DEFAULT_JOINT_LENGTH), 0.0f);
        // rotate temp by phi
        float x = std::cos (phi) * temp.x - std::sin (phi) * temp.y;
        float y = std::sin (phi) * temp.x + std::cos (phi) * temp.y;
        temp.x = x; temp.y = y;
        pos += temp;

        engine_in->DrawLine (old, pos, olc::BLACK, 0xFFFFFFFF);
        engine_in->FillCircle (pos, ENGINE_PGE_204_DEFAULT_RADIUS, (*iterator).color);
      } // end FOR
    }

    std::vector<struct joint> joints_;
    olc::vf2d                 root_;
  };

  noise::module::Perlin noise_;
  //OpenSimplexNoise      noise_;
  std::vector<string>   strings_;
};

#endif // PGE_204_H
