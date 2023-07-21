#ifndef PGE_62_H
#define PGE_62_H

#include "olcPixelGameEngine.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "defines_3.h"

class PGE_62
 : public olc::PixelGameEngine
{
 public:
  PGE_62 ();
  virtual ~PGE_62 ();

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  class particle
  {
   public:
    particle ()
     : position_ (0.0f, 0.0f)
     , speed_ (ENGINE_PGE_62_DEFAULT_SPEED)
    {
      randomize ();
    }

    void randomize ()
    {
      position_.x = Common_Tools::getRandomNumber (-1.0f, 1.0f);
      position_.y = Common_Tools::getRandomNumber (-1.0f, 1.0f);
    }

    float chladni (float x_in, float y_in,
                   int32_t n_in, int32_t m_in)
    {
      return static_cast<float> (std::cos (n_in * M_PI * x_in / ENGINE_PGE_62_DEFAULT_L) * std::cos (m_in * M_PI * y_in / ENGINE_PGE_62_DEFAULT_L) -
                                 std::cos (m_in * M_PI * x_in / ENGINE_PGE_62_DEFAULT_L) * std::cos (n_in * M_PI * y_in / ENGINE_PGE_62_DEFAULT_L));
    }

    void update (int32_t n_in, int32_t m_in)
    {
      float jiggle_x = Common_Tools::getRandomNumber (-ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, ENGINE_PGE_62_DEFAULT_MAX_JIGGLE);
      float jiggle_y = Common_Tools::getRandomNumber (-ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, ENGINE_PGE_62_DEFAULT_MAX_JIGGLE);

      float amount = chladni (position_.x, position_.y, n_in, m_in);
      if (amount >= 0.0f)
      {
        if (chladni (position_.x + ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, position_.y, n_in, m_in) >= amount)
          position_.x = (position_.x - Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_x);
        else
          position_.x = (position_.x + Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_x);
        if (chladni (position_.x, position_.y + ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, n_in, m_in) >= amount)
          position_.y = (position_.y - Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_y);
        else
          position_.y = (position_.y + Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_y);
      } // end IF
      else
      {
        if (chladni (position_.x + ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, position_.y, n_in, m_in) <= amount)
          position_.x = (position_.x + Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_x);
        else
          position_.x = (position_.x - Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_x);
        if (chladni (position_.x, position_.y + ENGINE_PGE_62_DEFAULT_MAX_JIGGLE, n_in, m_in) <= amount)
          position_.y = (position_.y + Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_y);
        else
          position_.y = (position_.y - Common_Tools::getRandomNumber (-0.5f, 1.0f) * amount * speed_ + jiggle_y);
      } // end ELSE

      if (position_.x > 1.0) position_.x = 1.0;
      else if (position_.x < -1.0) position_.x = -1.0;
      if (position_.y > 1.0) position_.y = 1.0;
      else if (position_.y < -1.0) position_.y = -1.0;
    }

    void show (olc::PixelGameEngine* engine_in,
               olc::Pixel& color_in)
    {
      olc::vf2d position = position_;
      position.x += 1.0f;
      position.x /= 2.0f;
      position.x =
        Common_GL_Tools::map (position.x, 0.0f, 1.0f, 0.0f, static_cast<float> (engine_in->ScreenWidth () - 1));
      position.y += 1.0f;
      position.y /= 2.0f;
      position.y =
        Common_GL_Tools::map (position.y, 0.0f, 1.0f, 0.0f, static_cast<float> (engine_in->ScreenHeight () - 1));

      engine_in->Draw (position, color_in);
    }

    olc::vf2d  position_;
    float      speed_;
  };

  std::vector<particle*> particles_;
  int32_t                n_;
  int32_t                m_;
  olc::Pixel             color_;
  olc::Pixel             lastColor_;
  float                  lastChange_;

  void shuffle ();
};

#endif // PGE_62_H
