#include "stdafx.h"

#include "pge_341.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "defines_16.h"

#include "engine_common.h"

PGE_341::PGE_341 ()
 : olc::PixelGameEngine ()
 , particles1_ ()
 , particles2_ ()
 , theta1_ (0.0f)
 , theta2_ (static_cast<float> (M_PI))
 , lindist_ (ENGINE_PGE_341_DEFAULT_DISTANCE)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 341");
}

bool
PGE_341::OnUserCreate ()
{
  particles1_.assign (ENGINE_PGE_341_DEFAULT_NUMBER_OF_PARTICLES,
                      particle (0.0f, 0.0f));
  for (int i = 0; i < static_cast<int> (particles1_.size ()); i++)
    particles1_[i] = particle (olc::PixelGameEngine::ScreenWidth () / 2.0f - 100.0f, i * 10.0f);

  particles2_.assign (ENGINE_PGE_341_DEFAULT_NUMBER_OF_PARTICLES,
                      particle (0.0f, 0.0f));
  for (int i = 0; i < static_cast<int> (particles2_.size ()); i++)
    particles2_[i] = particle (olc::PixelGameEngine::ScreenWidth () / 2.0f + 100.0f, i * 10.0f);

  return true;
}

bool
PGE_341::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a >= ENGINE_PGE_341_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_341_DEFAULT_ALPHA_DECAY : 0);

  static olc::vi2d offset_s (0, 50);
  static float half_width_f = olc::PixelGameEngine::ScreenWidth() / 2.0f;

  float r, g, b;
  olc::Pixel color;
  for (int i = 0; i < static_cast<int> (particles1_.size ()); i++)
  {
    particles1_[i].position_.x =
      Common_GL_Tools::map (std::sin (theta1_ + i / 5.0f),
                            -1.0f, 1.0f,
                            half_width_f - 1.0f / (i * 0.1f + 1.0f) * half_width_f, half_width_f + 1.0f / (i * 0.1f + 1.0f) * half_width_f);
    particles1_[i].draw (offset_s, this);

    for (int j = 0; j < static_cast<int> (particles1_.size ()); j++)
    {
      float d1 = particles1_[i].position_.dist (particles1_[j].position_);
      float d2 = particles1_[i].position_.dist (particles2_[j].position_);
      if (d1 < lindist_)
      {
        Common_Image_Tools::HSVToRGB ((static_cast<float> (j) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
        color =
          {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 200U};
        olc::PixelGameEngine::DrawLine (offset_s + particles1_[i].position_, offset_s + particles1_[j].position_, color, 0xFFFFFFFF);
      } // end IF
      if (d2 < lindist_)
      {
        Common_Image_Tools::HSVToRGB ((static_cast<float> (std::abs (255 - j)) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
        color =
          {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 200U};
        olc::PixelGameEngine::DrawLine (offset_s + particles1_[i].position_, offset_s + particles2_[j].position_, color, 0xFFFFFFFF);
      } // end IF
    } // end FOR
  } // end FOR

  for (int i = 0; i < static_cast<int> (particles2_.size ()); i++)
  {
    particles2_[i].position_.x =
      Common_GL_Tools::map (std::sin (theta2_ + i / 4.0f),
                            -1.0f, 1.0f,
                            half_width_f - 1.0f / (i * 0.1f + 1.0f) * half_width_f, half_width_f + 1.0f / (i * 0.1f + 1.0f) * half_width_f);
    particles2_[i].draw (offset_s, this);

    for (int j = 0; j < static_cast<int> (particles2_.size ()); j++)
    {
      float d1 = particles2_[i].position_.dist (particles2_[j].position_);
      if (d1 < lindist_)
      {
        Common_Image_Tools::HSVToRGB ((static_cast<float> (j) / 255.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
        color =
          {static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f), 200U};
        olc::PixelGameEngine::DrawLine (offset_s + particles2_[i].position_, offset_s + particles2_[j].position_, color, 0xFFFFFFFF);
      } // end IF
    } // end FOR
  } // end FOR

  theta1_ += 0.01f;
  theta2_ += 0.01f;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
