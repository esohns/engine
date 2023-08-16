#include "stdafx.h"

#include "pge_112.h"

#include "ace/Log_Msg.h"

#include "defines_4.h"
#include "engine_common.h"

PGE_112::PGE_112 ()
 : olc::PixelGameEngine ()
 , movers_ ()
 , transparentBG_ (true)
 , mode_ (5)
 , noise_ ()
{
  sAppName = "Example 112";
}

PGE_112::~PGE_112 ()
{
  for (std::vector<mover*>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_112::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);
  olc::PixelGameEngine::Clear ({0x57, 0x38, 0x5c, 255});

  for (int i = 0; i < ENGINE_PGE_112_DEFAULT_NUMBER_OF_MOVERS; i++)
    movers_.push_back (new mover (this));

  return true;
}

bool
PGE_112::OnUserUpdate (float fElapsedTime)
{
  static int32_t frame_count_i = 1;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
  {
    mode_++;
    if (mode_ > 5) mode_ = 0;
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    transparentBG_ = !transparentBG_;
  if (olc::PixelGameEngine::GetKey(olc::Key::N).bPressed)
  {
    float noiseScale = Common_Tools::getRandomNumber (5.0f, 400.0f);
    float noiseStrength = Common_Tools::getRandomNumber (0.5f, 6.0f);
    float forceStrength = Common_Tools::getRandomNumber (0.5f, 4.0f);
    for (size_t i = 0; i < movers_.size (); i++)
    {
      movers_[i]->noiseScale_ = noiseScale;
      movers_[i]->noiseStrength_ = noiseStrength;
      movers_[i]->forceStrength_ = forceStrength;
    } // end FOR
  } // end IF

  if (transparentBG_) olc::PixelGameEngine::Clear ({0x57, 0x38, 0x5c, 40});
  else olc::PixelGameEngine::Clear ({0x57, 0x38, 0x5c, 255});

  size_t i = 0;
  while (i < movers_.size ())
  {
    if (mode_ != 5) movers_[i]->update (mode_, this, &noise_, frame_count_i);
    else
    {
      movers_[i]->flock (movers_);
      movers_[i]->move ();
      movers_[i]->checkEdges (this);
      movers_[i]->show (this);
    } // end ELSE
    i++;
  } // end WHILE

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
