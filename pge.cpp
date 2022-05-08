#include "stdafx.h"

#include "pge.h"

#include <complex>

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "defines.h"
#include "engine_common.h"

PGE::PGE ()
{
  sAppName = "Example";
}

bool
PGE::OnUserCreate ()
{
  textScale = 1;
  PGE::stream* stream_p = NULL;
  for (int32_t i = 0;
       i < olc::PixelGameEngine::ScreenWidth ();
       i += olc::PixelGameEngine::GetTextSize(ACE_TEXT_ALWAYS_CHAR("T")).y * textScale)
  {
    stream_p = new PGE::stream ();
    ACE_ASSERT (stream_p);
    stream_p->make (i,
                    olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")).y * textScale);
    streams.push_back (stream_p);
  } // end FOR

  return true;
}

bool
PGE::OnUserUpdate (float fElapsedTime)
{
  // Clear Screen
  olc::PixelGameEngine::Clear (olc::Pixel (0, 0, 0, 175)); // black

  static int32_t frame_count = 0;
  ++frame_count;
  for (std::vector<PGE::stream*>::iterator iterator = streams.begin ();
       iterator != streams.end ();
       ++iterator)
    renderStream (**iterator, frame_count);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
