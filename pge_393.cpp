#include "stdafx.h"

#include "pge_393.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_393::PGE_393 ()
 : olc::PixelGameEngine ()
 , mjrcs_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 393");
}

bool
PGE_393::OnUserCreate ()
{
  float w = olc::PixelGameEngine::ScreenWidth () / static_cast<float> (ENGINE_PGE_393_DEFAULT_C);
  for (int i = 0; i < ENGINE_PGE_393_DEFAULT_C; i++)
    for (int j = 0; j < ENGINE_PGE_393_DEFAULT_C; j++)
    {
      float x = i * w;
      float y = j * w;
      olc::vf2d v (x, y);
      static float half_width_f = static_cast<float> (olc::PixelGameEngine::ScreenWidth ()) / 2.0f;
      static float half_height_f = static_cast<float> (olc::PixelGameEngine::ScreenHeight ()) / 2.0f;
      float dst = v.dist ({ half_width_f, half_height_f });
      int t =
        static_cast<int> (Common_GL_Tools::map (dst, 0.0f, std::sqrt ((half_width_f * half_width_f) + (half_height_f * half_height_f)), 0.0f, -50.0f));
      for (int k = 0; k < 7; k++)
        mjrcs_.push_back (mjrc (x, y, w, t));
    } // end FOR

  return true;
}

bool
PGE_393::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_393_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_393_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<mjrc>::iterator iterator = mjrcs_.begin ();
       iterator != mjrcs_.end ();
       ++iterator)
  {
    (*iterator).update ();
    (*iterator).draw (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
