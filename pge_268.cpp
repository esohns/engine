#include "stdafx.h"

#include "pge_268.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_12.h"

PGE_268::PGE_268 ()
 : olc::PixelGameEngine ()
 , particles_ ()
 , edges_ ()
 , showGrid_ (false)
 , walls_ (false)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 268");
}

bool
PGE_268::OnUserCreate ()
{
  if (walls_)
  {
    float padding = 0.0f;
    olc::vf2d position_1 (padding, 0.0f);
    olc::vf2d position_2 (padding, static_cast<float> (olc::PixelGameEngine::ScreenHeight ()));
    edges_.push_back (edge (position_1,
                            position_2,
                            1.0f,
                            0.0000001f,
                            true));
    position_1.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) - padding;
    position_1.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight ());
    position_2.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) - padding;
    position_2.y = 0.0f;
    edges_.push_back (edge (position_1,
                            position_2,
                            1.0f,
                            0.0000001f,
                            true));
    position_1.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
    position_1.y = padding;
    position_2.x = 0.0f;
    position_2.y = padding;
    edges_.push_back (edge (position_1,
                            position_2,
                            1.0f,
                            0.0000001f,
                            true));
    position_1.x = 0.0f;
    position_1.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) - padding;
    position_2.x = static_cast<float> (olc::PixelGameEngine::ScreenWidth ());
    position_2.y = static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) - padding;
    edges_.push_back (edge (position_1,
                            position_2,
                            1.0f,
                            0.0000001f,
                            true));
  } // end IF
  
  int grid = 200;
  for (int i = grid / 2; i < olc::PixelGameEngine::ScreenWidth () + grid; i += grid)
    for (int j = grid / 2; j < olc::PixelGameEngine::ScreenHeight () + grid; j += grid)
    {
      std::vector<olc::vf2d> vertices_a;
      vertices_a.push_back (olc::vf2d (i + std::cos (0.0f)                             * grid / 2, j + std::sin (0.0f)                             * grid / 2));
      vertices_a.push_back (olc::vf2d (i + std::cos (static_cast<float> (M_PI_2))      * grid / 2, j + std::sin (static_cast<float> (M_PI_2))      * grid / 2));
      vertices_a.push_back (olc::vf2d (i + std::cos (static_cast<float> (M_PI))        * grid / 2, j + std::sin (static_cast<float> (M_PI))        * grid / 2));
      vertices_a.push_back (olc::vf2d (i + std::cos (1.5f * static_cast<float> (M_PI)) * grid / 2, j + std::sin (1.5f * static_cast<float> (M_PI)) * grid / 2));

      for (int i = 0; i < vertices_a.size (); i++)
        edges_.push_back (edge (vertices_a[i],
                                i == vertices_a.size () - 1 ? vertices_a[0] : vertices_a[i + 1],
                                1.33f,
                                1.0f,
                                false));
      if (showGrid_)
      {
        //fill("#E4F0FF3F")
        //quad(i+cos(0)*grid/2,j+sin(0)*grid/2,
        //    i+cos(90)*grid/2,j+sin(90)*grid/2,
        //    i+cos(180)*grid/2,j+sin(180)*grid/2,
        //    i+cos(270)*grid/2,j+sin(270)*grid/2)
      } // end IF
    } // end FOR

  return true;
}

bool
PGE_268::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    generateParticle ();

  for (int i = static_cast<int> (particles_.size ()) - 1; i >= 0; i--)
  {
    particles_[i].update ();
    particles_[i].draw (this);
    for (std::vector<edge>::iterator iterator = edges_.begin ();
         iterator != edges_.end ();
         ++iterator)
      (*iterator).refraction (particles_[i]);

    if (particles_[i].life_ <= 0)
    {
      std::vector<particle>::iterator iterator = particles_.begin ();
      std::advance (iterator, i);
      particles_.erase (iterator);
    } // end IF
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_268::generateParticle ()
{
  float main_hue_f = Common_Tools::getRandomNumber (0.0f, 720.0f);

  static std::random_device rd;
  static std::mt19937 m (rd ());
  std::normal_distribution<float> dist (main_hue_f, 20.0f);

  olc::vf2d mouse_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                     static_cast<float> (olc::PixelGameEngine::GetMouseY ()));

  for (int i = 0; i < 150; i++)
  {
    float angle = Common_GL_Tools::map (static_cast<float> (i), 0.0f, 150.0f, 0.0f, 360.0f) + 0.1f;
    angle = glm::radians (angle);
    olc::vf2d angle_s (std::cos (angle), std::sin (angle));

    float r, g, b;
    float hue_f = std::fmod (dist (m) + 360.0f, 360.0f);
    Common_Image_Tools::HSVToRGB (hue_f,
                                  Common_Tools::getRandomNumber (0.0f, 7.0f) / 100.0f,
                                  1.0f,
                                  r, g, b);
    olc::Pixel color;
    color.r = static_cast<uint8_t> (r * 255.0f);
    color.g = static_cast<uint8_t> (g * 255.0f);
    color.b = static_cast<uint8_t> (b * 255.0f);
    color.a =
      static_cast<uint8_t> (Common_GL_Tools::map (Common_Tools::getRandomNumber (0.01f, 0.05f), 0.0f, 100.0f, 0.0f, 255.0f));

    particles_.push_back (particle (mouse_s,
                                    angle_s,
                                    color));
  } // end FOR
}
