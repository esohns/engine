#include "stdafx.h"

#include "pge_306.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_14.h"

PGE_306::PGE_306 ()
 : olc::PixelGameEngine ()
 , circles_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 306");
}

bool
PGE_306::OnUserCreate ()
{
  for (int i = 0; i < 100; i++)
  {
    olc::vf2d position (i * 10.0f, static_cast<float> (i));
    float radius = static_cast<float> (i) + 1.0f;
    circles_.push_back (circle (position, radius));
  } // end FOR

  return true;
}

bool
PGE_306::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_306_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_306_DEFAULT_ALPHA_DECAY : 0);

  int i = 0;
  for (std::vector<circle>::iterator iterator = circles_.begin ();
       iterator != circles_.end ();
       ++iterator, ++i)
  {
    float r, g, b;
    Common_Image_Tools::HSVToRGB (360.0f * i / static_cast<float> (circles_.size ()), 200.0f / 255.0f, 1.0f, r, g, b);
    olc::Pixel color (static_cast<uint8_t> (r * 255.0f), static_cast<uint8_t> (g * 255.0f), static_cast<uint8_t> (b * 255.0f));
    (*iterator).update (this);
    (*iterator).display (this, color);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
