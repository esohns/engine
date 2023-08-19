#include "stdafx.h"

#include "pge_121.h"

#include "ace/Log_Msg.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_5.h"
#include "engine_common.h"

PGE_121::PGE_121 ()
 : olc::PixelGameEngine ()
 , cells_ ()
 , nbCellW_ (0)
 , nbCellH_ (0)
{
  sAppName = "Example 121";
}

bool
PGE_121::OnUserCreate ()
{
  nbCellW_ =
    static_cast<int32_t> (olc::PixelGameEngine::ScreenWidth () / ENGINE_PGE_121_DEFAULT_CELL_WIDTH);
  nbCellH_ =
    static_cast<int32_t> (olc::PixelGameEngine::ScreenHeight () / ENGINE_PGE_121_DEFAULT_CELL_HEIGHT);

  for (int i = 0; i < nbCellW_ * nbCellH_; i++)
    cells_.push_back (olc::vf2d (0.0f, 0.0f));

  return true;
}

bool
PGE_121::OnUserUpdate (float fElapsedTime)
{
  static olc::vf2d pmouse (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                           static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  olc::vf2d delta_mouse (olc::PixelGameEngine::GetMouseX () - pmouse.x,
                         olc::PixelGameEngine::GetMouseY () - pmouse.y);

  for (int32_t i = 0; i < nbCellW_; i++)
    for (int32_t j = 0; j < nbCellH_; j++)
    {
      int32_t k = i + j * nbCellW_;
      olc::vf2d position (static_cast<float> (ENGINE_PGE_121_DEFAULT_CELL_WIDTH * i + ENGINE_PGE_121_DEFAULT_CELL_WIDTH / 2.0f),
                          static_cast<float> (ENGINE_PGE_121_DEFAULT_CELL_HEIGHT * j + ENGINE_PGE_121_DEFAULT_CELL_HEIGHT / 2.0f));
      pmouse.x = static_cast<float> (olc::PixelGameEngine::GetMouseX());
      pmouse.y = static_cast<float> (olc::PixelGameEngine::GetMouseY());
      float d = std::max (1.0f, pmouse.dist (position));

      delta_mouse = delta_mouse.mag () ? delta_mouse.norm () : delta_mouse;
      delta_mouse *= 1.0f / (d * 10.0f);
      cells_[k] += delta_mouse;
      if (cells_[k].mag () > 10.0f)
      {
        cells_[k] = cells_[k].norm ();
        cells_[k] *= 10.0f;
      } // end IF

      float h =
        Common_GL_Tools::map (std::atan2 (cells_[k].y, cells_[k].x), -static_cast<float> (M_PI), static_cast<float> (M_PI), 0.0f, 1.0f);
      float b =
        Common_GL_Tools::map (std::min (cells_[k].mag () * 100.0f, 10.0f), 0.0f, 10.0f, 0.0f, 1.0f);

      float r, g, b_2;
      Common_Image_Tools::HSVToRGB (h * 360.0f, 1.0f, b, r, g, b_2);
      olc::Pixel color_s;
      color_s.r = static_cast<uint8_t> (r * 255.0f);
      color_s.g = static_cast<uint8_t> (g * 255.0f);
      color_s.b = static_cast<uint8_t> (b_2 * 255.0f);
      //color_s.a = 255;
      olc::PixelGameEngine::FillRect (static_cast<int32_t> (position.x - ENGINE_PGE_121_DEFAULT_CELL_WIDTH / 2.0f),
                                      static_cast<int32_t> (position.y - ENGINE_PGE_121_DEFAULT_CELL_HEIGHT / 2.0f),
                                      ENGINE_PGE_121_DEFAULT_CELL_WIDTH, ENGINE_PGE_121_DEFAULT_CELL_HEIGHT,
                                      color_s);

      cells_[k] *= 0.98f;
    } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
