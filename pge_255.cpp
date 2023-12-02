#include "stdafx.h"

#include "pge_255.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_255::PGE_255 ()
 : olc::PixelGameEngine ()
 , strings_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 255");
}

bool
PGE_255::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += ENGINE_PGE_255_DEFAULT_GRASS_DISTANCE)
    strings_.push_back (string (x, this));

  return true;
}

bool
PGE_255::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (olc::WHITE);

  olc::vf2d player (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                    static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  static olc::Pixel player_color (255, 255, 0, 128);
  olc::PixelGameEngine::FillCircle (player, ENGINE_PGE_255_DEFAULT_PLAYER_RADIUS, player_color);

  for (std::vector<string>::iterator iterator = strings_.begin ();
       iterator != strings_.end ();
       ++iterator)
  {
    (*iterator).collide (player);
    (*iterator).update (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
