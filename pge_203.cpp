#include "stdafx.h"

#include "pge_203.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_203::PGE_203 ()
 : olc::PixelGameEngine ()
 , ball_ (NULL)
 , color_ (olc::BLACK)
 , hexCurve_ (Common_Tools::testRandomProbability (0.5f))
 , history_ ()
 , noise_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 203");
}

PGE_203::~PGE_203 ()
{
  delete ball_;
}

bool
PGE_203::OnUserCreate ()
{
  return true;
}

bool
PGE_203::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    hexCurve_ = Common_Tools::testRandomProbability (0.5f);
    history_.clear ();
    olc::PixelGameEngine::Clear (olc::BLACK);
  } // end IF

  for (int i = 100; i--; )
  {
    if (!ball_) ball_ = new ball (this, color_);
    ball_->update (&noise_,
                   this,
                   hexCurve_);
    if (ball_->isFrameOut (this) || ball_->isOverlapped (history_))
    {
      delete ball_; ball_ = NULL;
    } // end IF
    else
    {
      history_.push_back (*ball_);
      ball_->show (this, color_);
    } // end ELSE
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
