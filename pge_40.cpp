#include "stdafx.h"

#include "pge_40.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_40::PGE_40 ()
 : olc::PixelGameEngine ()
 , cloth_ (NULL)
{
  sAppName = "Example 40";
}

PGE_40::~PGE_40 ()
{
  delete cloth_;
}

bool
PGE_40::OnUserCreate ()
{
  int clothWidth = ENGINE_PGE_40_DEFAULT_CLOTH_WIDTH;
  clothWidth /= ENGINE_PGE_40_DEFAULT_CLOTH_SPACING;
  int clothHeight = ENGINE_PGE_40_DEFAULT_CLOTH_HEIGHT;
  clothHeight /= ENGINE_PGE_40_DEFAULT_CLOTH_SPACING;
  int startX =
    static_cast<int> (olc::PixelGameEngine::ScreenWidth () * 0.5f - clothWidth * ENGINE_PGE_40_DEFAULT_CLOTH_SPACING * 0.5f);
  int startY =
    static_cast<int> (olc::PixelGameEngine::ScreenHeight () * 0.1f);
  cloth_ = new cloth (clothWidth, clothHeight, ENGINE_PGE_40_DEFAULT_CLOTH_SPACING, startX, startY);

  return true;
}

bool
PGE_40::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  static float lastElapsedTime = 0.0f;
  float deltaTime = fElapsedTime - lastElapsedTime;
  lastElapsedTime = fElapsedTime;

  cloth_->update (this, deltaTime * 100.0f);
  cloth_->draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
