#include "stdafx.h"

#include "pge_58.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_58::PGE_58 ()
 : olc::PixelGameEngine ()
 , elements_ ()
{
  sAppName = "Example 58";
}

PGE_58::~PGE_58 ()
{
  for (std::vector<element*>::iterator iterator = elements_.begin ();
       iterator != elements_.end ();
       ++iterator)
    delete *iterator;
}

bool
PGE_58::OnUserCreate ()
{
  ACE_OS::srand (static_cast<u_int> (time (NULL)));

  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  initialize ();

  return true;
}

bool
PGE_58::OnUserUpdate (float fElapsedTime)
{
  static bool show_elements_b = false;

  if (olc::PixelGameEngine::GetMouse (0).bPressed)
    initialize ();
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    show_elements_b = !show_elements_b;

  for (std::vector<element*>::iterator iterator = elements_.begin ();
       iterator != elements_.end ();
       ++iterator)
  {
    (*iterator)->update ();
    if (show_elements_b)
      (*iterator)->show (this);
    (*iterator)->edges (this);
  } // end FOR

  for (int i = 0; i < elements_.size (); ++i)
    for (int j = i + 1; j < elements_.size (); j++)
      elements_[i]->onOverlap (*elements_[j], this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_58::initialize ()
{
  for (std::vector<element*>::iterator iterator = elements_.begin ();
       iterator != elements_.end ();
       ++iterator)
    delete *iterator;
  elements_.clear ();

  for (int i = 0; i < ENGINE_PGE_58_DEFAULT_NUMBER_OF_ELEMENTS; i++)
  {
    float s = Common_Tools::getRandomNumber (0.0f, 2.0f);
    olc::Pixel color = {static_cast<uint8_t> (182 * s), static_cast<uint8_t> (103 * s), static_cast<uint8_t> (13 * s), 2};
    elements_.push_back (new element (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth () - 1),
                                      olc::PixelGameEngine::ScreenHeight () / 2,
                                      Common_Tools::getRandomNumber (40, 60),
                                      color));
  } // end FOR

  olc::PixelGameEngine::Clear (olc::WHITE);
}
