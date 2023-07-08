#include "stdafx.h"

#include "pge_43.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"
#include "ace/OS.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_43::PGE_43 ()
 : olc::PixelGameEngine ()
 , waves_ ()
{
  sAppName = "Example 43";
}

PGE_43::~PGE_43 ()
{
  for (std::vector<wave*>::iterator iterator = waves_.begin ();
       iterator != waves_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_43::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_43_DEFAULT_NUMBER_OF_WAVES; i++)
    waves_.push_back (new wave (Common_Tools::getRandomNumber (20.0f, 80.0f),
                                Common_Tools::getRandomNumber (100.0f, 600.0f),
                                Common_Tools::getRandomNumber (0.0f, static_cast<float> (2.0 * M_PI))));

  return true;
}

bool
PGE_43::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear ({0,0,0, ENGINE_PGE_41_DEFAULT_ALPHA_DECAY});
  int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  olc::Pixel* m = GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    m[i].a = (m[i].a > ENGINE_PGE_41_DEFAULT_ALPHA_DECAY ? m[i].a - ENGINE_PGE_41_DEFAULT_ALPHA_DECAY : 0);

  for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); x += 10)
  {
    float y = 0;
    for (std::vector<wave*>::iterator iterator = waves_.begin ();
         iterator != waves_.end ();
         ++iterator)
      y += (*iterator)->evaluate (x);
    olc::PixelGameEngine::FillCircle (x, static_cast<int32_t> (y + (olc::PixelGameEngine::ScreenHeight () / 2)), ENGINE_PGE_43_DEFAULT_RADIUS, {255, 255, 255, 100});
  } // end FOR

  for (std::vector<wave*>::iterator iterator = waves_.begin ();
       iterator != waves_.end ();
       ++iterator)
    (*iterator)->update ();

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
