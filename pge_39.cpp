#include "stdafx.h"

#include "pge_39.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_gl_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_39::PGE_39 ()
 : olc::PixelGameEngine ()
 , columns_ (0)
 , rows_(0)
 , field_ (NULL)
 , noise_ ()
 , zOffset_ (0.0)
{
  sAppName = "Example 39";
}

PGE_39::~PGE_39 ()
{
  delete [] field_;
}

bool
PGE_39::OnUserCreate ()
{
  columns_ = olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_39_DEFAULT_RESOLUTION;
  rows_ = olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_39_DEFAULT_RESOLUTION;

  field_ = new float[columns_ * rows_];
  ACE_OS::memset (field_, 0, sizeof (float) * columns_ * rows_);

  return true;
}

bool
PGE_39::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  double xOffset_d = 0.0;
  for (int i = 0; i < columns_; i++)
  {
    xOffset_d += ENGINE_PGE_39_DEFAULT_XY_INCREMENT;
    double yOffset_d = 0.0;
    for (int j = 0; j < rows_; j++)
    {
      field_[j * columns_ + i] =
        static_cast<float> (noise_.GetValue (xOffset_d, yOffset_d, zOffset_));
      yOffset_d += ENGINE_PGE_39_DEFAULT_XY_INCREMENT;
    } // end FOR
  } // end FOR
  zOffset_ += ENGINE_PGE_39_DEFAULT_Z_INCREMENT;

  for (float h = -1.0f; h < 1.0f; h += 0.2f)
  {
    olc::Pixel pixel_s = {static_cast<uint8_t> (255 * h), static_cast<uint8_t> (255 * -h), 127, 255};
//    olc::Pixel pixel_s = olc::WHITE;

    for (int i = 0; i < columns_ - 1; i++)
      for (int j = 0; j < rows_ - 1; j++)
      {
        float f0 = field_[j * columns_ + i] - h;
        float f1 = field_[j * columns_ + (i + 1)] - h;
        float f2 = field_[((j + 1) * columns_) + (i + 1)] - h;
        float f3 = field_[((j + 1) * columns_) + i] - h;
        //float f0 = field_[j * columns_ + i];
        //float f1 = field_[j * columns_ + (i + 1)];
        //float f2 = field_[((j + 1) * columns_) + (i + 1)];
        //float f3 = field_[((j + 1) * columns_) + i];

        int x = i * ENGINE_PGE_39_DEFAULT_RESOLUTION;
        int y = j * ENGINE_PGE_39_DEFAULT_RESOLUTION;
        olc::vf2d a = {x + ENGINE_PGE_39_DEFAULT_RESOLUTION * f0 / (f0 - f1), static_cast<float> (y)};
        olc::vf2d b = {static_cast<float> (x + ENGINE_PGE_39_DEFAULT_RESOLUTION), y + ENGINE_PGE_39_DEFAULT_RESOLUTION * f1 / (f1 - f2)};
        olc::vf2d c = {x + ENGINE_PGE_39_DEFAULT_RESOLUTION * (1.0f - f2 / (f2 - f3)), static_cast<float> (y + ENGINE_PGE_39_DEFAULT_RESOLUTION)};
        olc::vf2d d = {static_cast<float> (x), y + ENGINE_PGE_39_DEFAULT_RESOLUTION * (1.0f - f3 / (f3 - f0))};

        switch (getState (f0, f1, f2, f3))
        {
          case 0:
            break;
          case 1:
            olc::PixelGameEngine::DrawLine (c, d, pixel_s, 0xFFFFFFFF);
            break;
          case 2:
            olc::PixelGameEngine::DrawLine (b, c, pixel_s, 0xFFFFFFFF);
            break;
          case 3:
            olc::PixelGameEngine::DrawLine (b, d, pixel_s, 0xFFFFFFFF);
            break;
          case 4:
            olc::PixelGameEngine::DrawLine (a, b, pixel_s, 0xFFFFFFFF);
            break;
          case 5:
            olc::PixelGameEngine::DrawLine (a, d, pixel_s, 0xFFFFFFFF);
            olc::PixelGameEngine::DrawLine (b, c, pixel_s, 0xFFFFFFFF);
            break;
          case 6:
            olc::PixelGameEngine::DrawLine (a, c, pixel_s, 0xFFFFFFFF);
            break;
          case 7:
            olc::PixelGameEngine::DrawLine (a, d, pixel_s, 0xFFFFFFFF);
            break;
          case 8:
            olc::PixelGameEngine::DrawLine (a, d, pixel_s, 0xFFFFFFFF);
            break;
          case 9:
            olc::PixelGameEngine::DrawLine (a, c, pixel_s, 0xFFFFFFFF);
            break;
          case 10:
            olc::PixelGameEngine::DrawLine (a, b, pixel_s, 0xFFFFFFFF);
            olc::PixelGameEngine::DrawLine (c, d, pixel_s, 0xFFFFFFFF);
            break;
          case 11:
            olc::PixelGameEngine::DrawLine (a, b, pixel_s, 0xFFFFFFFF);
            break;
          case 12:
            olc::PixelGameEngine::DrawLine (b, d, pixel_s, 0xFFFFFFFF);
            break;
          case 13:
            olc::PixelGameEngine::DrawLine (b, c, pixel_s, 0xFFFFFFFF);
            break;
          case 14:
            olc::PixelGameEngine::DrawLine (c, d, pixel_s, 0xFFFFFFFF);
            break;
          case 15:
            break;
          default:
            ACE_ASSERT (false);
            break;
        } // end SWITCH
      } // end FOR
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

int
PGE_39::getState (float a, float b, float c, float d)
{
  return (a > 0.0f ? 8 : 0) + (b > 0.0f ? 4 : 0) + (c > 0.0f ? 2 : 0) + (d > 0.0f ? 1 : 0);
}
