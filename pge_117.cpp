#include "stdafx.h"

#include "pge_117.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_117::PGE_117 ()
 : olc::PixelGameEngine ()
 , line_ (NULL)
{
  sAppName = "Example 117";
}

PGE_117::~PGE_117 ()
{
  delete line_;
}

bool
PGE_117::OnUserCreate ()
{
  line_ = new line (true);
  for (float a = 0.0f; a < 2.0f * static_cast<float> (M_PI); a += 2.0f * static_cast<float> (M_PI) / 100.0f)
    line_->addPoint (0.5f * olc::PixelGameEngine::ScreenWidth () + ENGINE_PGE_117_DEFAULT_RADIUS * std::cos (a),
                     0.5f * olc::PixelGameEngine::ScreenHeight () + ENGINE_PGE_117_DEFAULT_RADIUS * std::sin (a),
                     false, 5);

  return true;
}

bool
PGE_117::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bHeld)
    line_->insertPoint (false, 5, 5, Common_Tools::getRandomNumber (0, static_cast<int> (line_->points_.size () - 2)));

  olc::PixelGameEngine::Clear (olc::BLACK);

  line_->update (ENGINE_PGE_117_DEFAULT_TSTEP);
  line_->show (false, this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
