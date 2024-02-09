#include "stdafx.h"

#include "pge_352.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_352::PGE_352 ()
 : olc::PixelGameEngine ()
 , links_ ()
 , nodes_ ()
 , zoom_ (0.0f)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 352");
}

bool
PGE_352::OnUserCreate ()
{
  zoom_ =
    std::min (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ()) * 1.2f;

  for (int i = 0; i < ENGINE_PGE_352_DEFAULT_NUMBER_OF_NODES; i++)
  {
    float alpha = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    float beta = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    float x = std::sin (alpha) * std::cos (beta);
    float y = std::cos (alpha);
    float z = std::sin (alpha) * std::sin (beta);
    nodes_.push_back (node (x, y, z));
  } // end FOR
  makeLinks ();

  return true;
}

bool
PGE_352::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);

  olc::PixelGameEngine::Clear (olc::WHITE);

  for (std::vector<node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
    (*iterator).rotate ();
  for (std::vector<node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
    (*iterator).update (frame_count_i, zoom_);
  for (std::vector<node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
    (*iterator).show (this, center_s);

  for (std::vector<link>::iterator iterator = links_.begin ();
       iterator != links_.end ();
       ++iterator)
    (*iterator).show (this, center_s);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_352::makeLinks ()
{
  for (std::vector<node>::iterator iterator = nodes_.begin ();
       iterator != nodes_.end ();
       ++iterator)
  {
    std::vector<node>::iterator iterator_2 = iterator;
    std::advance (iterator_2, 1);
    for (;
         iterator_2 != nodes_.end ();
         ++iterator_2)
    {
      glm::vec3 centre = ((*iterator).vec3_ + (*iterator_2).vec3_) / 2.0f;
      float radius =
        glm::distance ((*iterator).vec3_, (*iterator_2).vec3_) / 2.0f;
      bool condition_met_b = true;
      for (std::vector<node>::iterator iterator_3 = nodes_.begin ();
           iterator_3 != nodes_.end ();
           ++iterator_3)
      {
        if (iterator_3 == iterator || iterator_3 == iterator_2)
          continue;
        if (glm::distance ((*iterator_3).vec3_, centre) <= radius)
        {
          condition_met_b = false;
          break;
        } // end IF
      } // end FOR
      if (condition_met_b)
        links_.push_back (link (*iterator, *iterator_2));
    } // end FOR
  } // end FOR
}
