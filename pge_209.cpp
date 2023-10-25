#include "stdafx.h"

#include "pge_209.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_image_tools.h"

#include "defines_9.h"
#include "engine_common.h"

PGE_209::PGE_209 ()
 : olc::PixelGameEngine ()
 , actors_ ()
 , currentId_ (13)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 209");
}

bool
PGE_209::OnUserCreate ()
{
  for (int i = 0; i < ENGINE_PGE_209_DEFAULT_NUMBER_OF_ACTORS; i++)
    actors_.push_back (actor ());
  setting (currentId_);

  return true;
}

bool
PGE_209::OnUserUpdate (float fElapsedTime)
{
  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    olc::PixelGameEngine::Clear (olc::BLACK);

    currentId_ = (currentId_ + 1) % ENGINE_PGE_209_DEFAULT_MAX_ID;
    setting (currentId_);
  } // end IF

  for (std::vector<actor>::iterator iterator = actors_.begin ();
       iterator != actors_.end ();
       ++iterator)
  {
    (*iterator).update ();
    (*iterator).render (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_209::setting (int id)
{
  float h = Common_Tools::getRandomNumber (0.0f, 80.0f);
  olc::Pixel color;
  float r, g, b;
  for (int i = 0; i < 20; i++)
    for (int k = 0; k < 20; k++)
    {
      actors_[i + k * 20].setPos (-190.0f + 20.0f * i, -190.0f + 20.0f * k);
      actors_[i + k * 20].setId (id);
      Common_Image_Tools::HSVToRGB (((h + static_cast<float> (i % 20)) / 100.0f) * 360.0f, 1.0f, 1.0f, r, g, b);
      color.r = static_cast<uint8_t> (r * 255.0f);
      color.g = static_cast<uint8_t> (g * 255.0f);
      color.b = static_cast<uint8_t> (b * 255.0f);
      actors_[i + k * 20].setColor (color);
    } // end FOR
}

