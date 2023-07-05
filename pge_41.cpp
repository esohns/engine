#include "stdafx.h"

#include "pge_41.h"

#include "ace/Assert.h"
#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "defines_2.h"
#include "engine_common.h"

PGE_41::PGE_41 ()
 : olc::PixelGameEngine ()
 , movers_ ()
 , sun_ (NULL)
{
  sAppName = "Example 41";
}

PGE_41::~PGE_41 ()
{
  for (std::vector<mover*>::const_iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
    delete *iterator;
  delete sun_;
}

bool
PGE_41::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  int32_t width_i, height_i;
  width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  height_i = olc::PixelGameEngine::ScreenHeight () / 2;

  for (int i = 0; i < ENGINE_PGE_41_DEFAULT_NUMBER_OF_MOVERS; i++)
  {
    olc::vf2d position = { Common_Tools::getRandomNumber (0.0f, 1.0f),
                           Common_Tools::getRandomNumber (0.0f, 1.0f) };
    olc::vf2d velocity = position;
    position = position.norm ();
    position *= Common_Tools::getRandomNumber (100.0f, 150.0f);
    position.x += width_i;
    position.y += height_i;

    velocity = velocity.norm ();
    velocity *= Common_Tools::getRandomNumber (10.0f, 15.0f);
    velocity = { -velocity.y, velocity.x }; // rotate by 90° (M_PI / 2)

    int32_t mass =
      static_cast<int32_t> (Common_Tools::getRandomNumber (10.0f, 15.0f));

    movers_.push_back (new mover (static_cast<int> (position.x), static_cast<int> (position.y), velocity.x, velocity.y, mass));
  } // end FOR
  sun_ = new mover (width_i, height_i, 0.0f, 0.0f, 500);

  return true;
}

bool
PGE_41::OnUserUpdate (float fElapsedTime)
{
  //olc::PixelGameEngine::Clear ({0,0,0, ENGINE_PGE_41_DEFAULT_ALPHA_DECAY});
  int pixels = GetDrawTargetWidth () * GetDrawTargetHeight ();
  olc::Pixel* m = GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    m[i].a = (m[i].a > ENGINE_PGE_41_DEFAULT_ALPHA_DECAY ? m[i].a - ENGINE_PGE_41_DEFAULT_ALPHA_DECAY : 0);

  for (std::vector<mover*>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
  {
    sun_->attract (**iterator);
    for (std::vector<mover*>::iterator iterator_2 = movers_.begin ();
         iterator_2 != movers_.end ();
         ++iterator_2)
      if (*iterator != *iterator_2)
      {
        (*iterator)->attract (**iterator_2);
        //olc::PixelGameEngine::DrawLine ((*iterator)->position_, (*iterator_2)->position_, { 255, 255, 255, 100 }, 0xFFFFFFFF);
      } // end IF
  } // end FOR

  for (std::vector<mover*>::iterator iterator = movers_.begin ();
       iterator != movers_.end ();
       ++iterator)
  {
    (*iterator)->update ();
    (*iterator)->show (this);
  } // end FOR
  //sun_->show (this);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}
