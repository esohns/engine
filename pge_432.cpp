#include "stdafx.h"

#include "pge_432.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_432::PGE_432 ()
 : olc::PixelGameEngine ()
 , movers_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 432");
}

bool
PGE_432::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_432_DEFAULT_NUMBER_OF_MOVERS; i++)
    movers_.push_back (mover (this));

  return true;
}

bool
PGE_432::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_432_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_432_DEFAULT_ALPHA_DECAY : 0);

  for (int i = 0; i < static_cast<int> (movers_.size ()); i++)
  {
    olc::vf2d p1 = movers_[i].position;
    for (int j = static_cast<int> (movers_.size ()) - 1; j >= 0; j--)
    {
      olc::vf2d p2 = movers_[j].position;
      float dst = p1.dist (p2);
      if (dst < static_cast<float> (ENGINE_PGE_432_DEFAULT_MAX_LINE_LENGTH) && i < j)
      {
        //float sw =
        //  Common_GL_Tools::map (dst, 0.0f, static_cast<float> (ENGINE_PGE_432_DEFAULT_MAX_LINE_LENGTH), olc::PixelGameEngine::ScreenWidth () * 0.003f, 0.0f);
        olc::PixelGameEngine::DrawLine (p1, p2, olc::WHITE, 0xFFFFFFFF);
      } // end IF
    } // end FOR
  } // end FOR

  for (std::vector<mover>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
  {
    (*iterator).draw (this);
    (*iterator).update (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
