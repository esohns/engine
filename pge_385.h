#ifndef PGE_385_H
#define PGE_385_H

#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_17.h"

class PGE_385
 : public olc::PixelGameEngine
{
 public:
  PGE_385 ();
  inline virtual ~PGE_385 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
     particle (int numTypes,
               olc::PixelGameEngine* engine_in)
     : position_ ()
     , velocity_ (0.0f, 0.0f)
     , type_ (Common_Tools::getRandomNumber (0, numTypes - 1))
    {
       float rad = Common_Tools::getRandomNumber (0.0f, 100.0f);
       float ang = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));

       static float half_width_f = static_cast<float> (engine_in->ScreenWidth ()) / 2.0f;
       static float half_height_f = static_cast<float> (engine_in->ScreenHeight ()) / 2.0f;
       position_ = olc::vf2d (rad * std::cos (ang) + half_width_f, rad * std::sin (ang) + half_height_f);
    }

    void update (std::vector<particle>& particles,
                 std::vector< std::vector<float> >& forces,
                 std::vector< std::vector<float> >& minDistances,
                 std::vector< std::vector<float> >& radii,
                 olc::PixelGameEngine* engine_in)
    {
      olc::vf2d direction;
      olc::vf2d totalForce (0.0f, 0.0f);
      olc::vf2d acceleration (0.0f, 0.0f);
      float dis;

      static int32_t screen_width_i = engine_in->ScreenWidth ();
      static int32_t screen_height_i = engine_in->ScreenHeight ();
      static float half_width_f = screen_width_i / 2.0f;
      static float half_height_f = screen_height_i / 2.0f;
      for (std::vector<particle>::iterator iterator = particles.begin ();
           iterator != particles.end ();
           ++iterator)
      {
        if (&(*iterator) == this)
          continue;

        direction = (*iterator).position_;
        direction -= position_;
        if (direction.x > half_width_f)
          direction.x -= static_cast<float> (screen_width_i);
        if (direction.x < -half_width_f)
          direction.x += static_cast<float> (screen_width_i);
        if (direction.y > half_height_f)
          direction.y -= static_cast<float> (screen_height_i);
        if (direction.y < -half_height_f)
          direction.y += static_cast<float> (screen_height_i);

        dis = direction.mag ();
        direction = dis ? direction.norm () : olc::vf2d (0.0f, 0.0f);
        if (dis < minDistances[type_][(*iterator).type_])
        {
          olc::vf2d force = direction;
          force *= (std::abs (forces[type_][(*iterator).type_]) * -3.0f);
          force *= Common_GL_Tools::map (dis, 0.0f, minDistances[type_][(*iterator).type_], 1.0f, 0.0f);
          force *= ENGINE_PGE_385_DEFAULT_K;
          totalForce += force;
        } // end IF

        if (dis < radii[type_][(*iterator).type_])
        {
          olc::vf2d force = direction;
          force *= forces[type_][(*iterator).type_];
          force *= Common_GL_Tools::map (dis, 0.0f, radii[type_][(*iterator).type_], 1.0f, 0.0f);
          force *= ENGINE_PGE_385_DEFAULT_K;
          totalForce += force;
        } // end IF
      } // end FOR

      acceleration += totalForce;
      velocity_ += acceleration;
      position_ += velocity_;

      position_.x =
        static_cast<float> ((static_cast<int> (position_.x) + screen_width_i) % screen_width_i);
      position_.y =
        static_cast<float> ((static_cast<int> (position_.y) + screen_height_i) % screen_height_i);
      velocity_ *= ENGINE_PGE_385_DEFAULT_FRICTION;
    }

    void draw (float colorStep,
               olc::PixelGameEngine* engine_in)
    {
      float r, g, b;
      Common_Image_Tools::HSVToRGB (type_ * colorStep, 1.0f, 1.0f, r, g, b);
      olc::Pixel color = olc::PixelF (r, g, b, 1.0f);

      engine_in->DrawCircle (position_, 2, color, 0xFF);
    }

    olc::vf2d position_;
    olc::vf2d velocity_;
    int       type_;
  };

  int                              numberOfTypes_;
  float                            colorStep_;
  std::vector<particle>            particles_;
  std::vector<std::vector<float> > forces_;
  std::vector<std::vector<float> > minDistances_;
  std::vector<std::vector<float> > radii_;

  void setParameters ();
};

#endif // PGE_385_H
