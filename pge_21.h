#ifndef PGE_21_H
#define PGE_21_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines.h"

class PGE_21
 : public olc::PixelGameEngine
{
 private:
  class star
  {
   public:
    star ()
     : position_ ()
     , z_ (0)
     , pz_ (0)
    {
      ACE_INT32 half_i = ENGINE_PGE_21_DEFAULT_WIDTH / 2;
      position_.x = Common_Tools::getRandomNumber (-half_i, half_i);
      half_i = ENGINE_PGE_21_DEFAULT_HEIGHT / 2;
      position_.y = Common_Tools::getRandomNumber (-half_i, half_i);
      z_ = Common_Tools::getRandomNumber (0, ENGINE_PGE_21_DEFAULT_WIDTH - 1);
      pz_ = z_;
    }

    void update (ACE_UINT32 speed_in)
    {
      z_ -= speed_in;
      if (z_ < 1)
      {
        ACE_INT32 half_i = ENGINE_PGE_21_DEFAULT_WIDTH / 2;
        position_.x = Common_Tools::getRandomNumber (-half_i, half_i);
        half_i = ENGINE_PGE_21_DEFAULT_HEIGHT / 2;
        position_.y = Common_Tools::getRandomNumber (-half_i, half_i);
        z_ = ENGINE_PGE_21_DEFAULT_WIDTH - 1;
        pz_ = z_;
      } // end IF
    }

    void show (olc::PixelGameEngine* engine_in)
    {
      static ACE_INT32 half_width_i = ENGINE_PGE_21_DEFAULT_WIDTH / 2;
      ACE_INT32 sx =
        half_width_i + static_cast<ACE_INT32> (Common_GL_Tools::map (position_.x / (float)z_, -0.5f, 0.5f, static_cast<float> (-half_width_i), static_cast<float> (half_width_i)));
      static ACE_INT32 half_height_i = ENGINE_PGE_21_DEFAULT_HEIGHT / 2;
      ACE_INT32 sy =
        half_height_i + static_cast<ACE_INT32> (Common_GL_Tools::map (position_.y / (float)z_, -0.5f, 0.5f, static_cast<float> (-half_height_i), static_cast<float> (half_height_i)));

      ACE_INT32 px =
        half_width_i + static_cast<ACE_INT32> (Common_GL_Tools::map (position_.x / (float)pz_, -0.5f, 0.5f, static_cast<float> (-half_width_i), static_cast<float> (half_width_i)));
      ACE_INT32 py =
        half_height_i + static_cast<ACE_INT32> (Common_GL_Tools::map (position_.y / (float)pz_, -0.5f, 0.5f, static_cast<float> (-half_height_i), static_cast<float> (half_height_i)));

      ACE_INT32 r =
        static_cast<ACE_INT32> (ENGINE_PGE_21_DEFAULT_MAX_RADIUS_F - Common_GL_Tools::map (static_cast<float> (z_), 0.0f, static_cast<float> (ENGINE_PGE_21_DEFAULT_WIDTH - 1), 0.0f, ENGINE_PGE_21_DEFAULT_MAX_RADIUS_F));

      if ((sx > 0 && sx < (ENGINE_PGE_21_DEFAULT_WIDTH - 1)) && (sy > 0 && sy < (ENGINE_PGE_21_DEFAULT_HEIGHT - 1)))
      {
        engine_in->DrawLine (px, py, sx, sy, olc::WHITE, 0xFFFFFFFF);
        engine_in->FillCircle (sx, sy, r, olc::WHITE);
      } // end IF

      pz_ = z_;
    }

   public:
    olc::vi2d position_;
    ACE_INT32 z_;
    ACE_INT32 pz_;
  };

 public:
  PGE_21 ();
  ~PGE_21 ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  std::vector<star> stars_;
  ACE_UINT32        speed_;
};

#endif // PGE_21_H
