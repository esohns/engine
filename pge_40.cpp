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
  int cloth_width =
    ENGINE_PGE_40_DEFAULT_CLOTH_WIDTH / ENGINE_PGE_40_DEFAULT_CLOTH_SPACING;
  int cloth_height =
    ENGINE_PGE_40_DEFAULT_CLOTH_HEIGHT / ENGINE_PGE_40_DEFAULT_CLOTH_SPACING;
  int startX =
    static_cast<int> (olc::PixelGameEngine::ScreenWidth () * 0.5f - cloth_width * ENGINE_PGE_40_DEFAULT_CLOTH_SPACING * 0.5f);
  int startY =
    static_cast<int> (olc::PixelGameEngine::ScreenHeight () * 0.025f);
  cloth_ =
    new cloth (cloth_width, cloth_height, ENGINE_PGE_40_DEFAULT_CLOTH_SPACING, startX, startY);

  return true;
}

bool
PGE_40::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::BLACK);

  cloth_->update (this, fElapsedTime * ENGINE_PGE_40_DEFAULT_DT_FACTOR);
  cloth_->draw (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
