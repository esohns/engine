#include "stdafx.h"

#include "pge_119.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_119::PGE_119 ()
 : olc::PixelGameEngine ()
 , parts_ (NULL)
 , mode_ (true)
{
  sAppName = "Example 119";
}

PGE_119::~PGE_119 ()
{
  for (int i = 0; i < ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS * ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS; i++)
    delete parts_[i];
  delete [] parts_;
}

bool
PGE_119::OnUserCreate ()
{
  parts_ = new part*[ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS * ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS];

  int dx =
    (olc::PixelGameEngine::ScreenWidth () - ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS * ENGINE_PGE_119_DEFAULT_STEP) / 2;
  int dy =
    (olc::PixelGameEngine::ScreenHeight () - ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS * ENGINE_PGE_119_DEFAULT_STEP) / 2;
  for (int i = 0; i < ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS; i++)
    for (int j = 0; j < ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS; j++)
      parts_[i * ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS + j] = new part (static_cast<float> (i * ENGINE_PGE_119_DEFAULT_STEP + dx), static_cast<float> (j * ENGINE_PGE_119_DEFAULT_STEP + dy));

  return true;
}

bool
PGE_119::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    mode_ = !mode_;

  olc::PixelGameEngine::Clear (olc::WHITE);

  olc::vf2d m (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
               static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  for (int i = 0; i < ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS; i++)
    for (int j = 0; j < ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS; j++)
      parts_[i * ENGINE_PGE_119_DEFAULT_NUMBER_OF_PARTS + j]->update (m, mode_, this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
