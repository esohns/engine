#include "stdafx.h"

//#define OLC_GFX_OPENGL33
#include "pge.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "engine_common.h"

PGE::PGE ()
 : font (NULL)
 , streams ()
 , textScale (SYMBOL_SCALE)
{
  sAppName = "Example";
}

bool
PGE::OnUserCreate ()
{
  //inherited::SetPixelMode (olc::Pixel::Mode::ALPHA);

  font = new olc::Font (ACE_TEXT_ALWAYS_CHAR ("./matrix code nfi.png"));

  static int32_t screen_width_i = olc::PixelGameEngine::ScreenWidth ();
  static olc::vi2d symbol_size_i =
    //olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")) * textScale;
    font->GetTextSize (ACE_TEXT_ALWAYS_CHAR ("a")) * textScale;

  PGE::stream* stream_p = NULL;
  for (int32_t i = 0;
       i < screen_width_i;
       i += symbol_size_i.x)
  {
    stream_p = new PGE::stream ();
    ACE_ASSERT (stream_p);
    stream_p->make (i,
                    symbol_size_i.y);
    streams.push_back (stream_p);
  } // end FOR

  return true;
}

bool
PGE::OnUserUpdate (float fElapsedTime)
{
  static int32_t screen_height_i = inherited::ScreenHeight ();
  static int32_t symbol_size_y_i =
    //olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")).y;
    font->GetTextSize (ACE_TEXT_ALWAYS_CHAR ("a")).y * textScale;

  static int32_t frame_count_i = 1;

  // Clear Screen
  //inherited::Clear (olc::Pixel (0, 0, 0, 15)); // black
  //int pixels =
  //  olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  //olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  //for (int i = 0; i < pixels; i++)
  //  p[i].a = (p[i].a > ENGINE_PGE_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<PGE::stream*>::iterator iterator = streams.begin ();
       iterator != streams.end ();
       ++iterator)
    renderStream (**iterator, screen_height_i, symbol_size_y_i, frame_count_i);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
