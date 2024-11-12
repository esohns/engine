#include "stdafx.h"

#include "pge_406.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

#include "defines_18.h"

PGE_406::PGE_406 ()
 : olc::PixelGameEngine ()
 , atoms_ ()
{
  sAppName = "Example 406";
}

bool
PGE_406::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_406_DEFAULT_NUMBER_OF_ATOMS; i++)
    atoms_.push_back (atom (Common_Tools::testRandomProbability (0.5f) ? PGE_406::atom::TYPE_POSITIVE : PGE_406::atom::TYPE_NEGATIVE,
                            this));

  return true;
}

bool
PGE_406::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  //olc::PixelGameEngine::Clear (olc::BLACK);
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
  {
    p[i].a = (p[i].a > ENGINE_PGE_406_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_406_DEFAULT_ALPHA_DECAY : 0);
    //if (!p[i].a)
    //  p[i] = olc::BLACK;
  } // end FOR

  avoidOverlapping ();

  // separate, then attract; repeat
  (frame_count_i % 500) < 200 ? attractBetweenChanges () : attractBetweenChanges2 ();

  for (std::vector<atom>::iterator iterator = atoms_.begin ();
       iterator != atoms_.end ();
       ++iterator)
    (*iterator).update ();
  for (std::vector<atom>::iterator iterator = atoms_.begin ();
       iterator != atoms_.end ();
       ++iterator)
    (*iterator).wrap (this);
  for (std::vector<atom>::iterator iterator = atoms_.begin ();
       iterator != atoms_.end ();
       ++iterator)
    (*iterator).show (this);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_406::avoidOverlapping ()
{
  static olc::vf2d center_s (olc::PixelGameEngine::ScreenWidth () / 2.0f,
                             olc::PixelGameEngine::ScreenHeight () / 2.0f);
  olc::vf2d mouse_position_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                              static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
  static int32_t radius_i = static_cast<int32_t> (ENGINE_PGE_406_DEFAULT_MOUSE_RADIUS / std::sqrt (2.0f));
  static olc::Pixel color_s (255, 255, 255, 128);
  olc::PixelGameEngine::FillCircle (mouse_position_s, radius_i, color_s);

  std::vector<atom*> all;
  for (int i = static_cast<int> (atoms_.size ()); i--; )
    all.push_back (&atoms_[i]);
  mouse_position_s -= center_s;
  atom mouse_atom (mouse_position_s);
  all.push_back (&mouse_atom);
  for (int i = static_cast<int> (all.size ()); i--; )
  {
    atom& A = *all[i];
    for (int j = i; j--; )
    {
      atom& B = *all[j];
      olc::vf2d delta = A.position - B.position;
      float distance = std::sqrt (delta.x * delta.x + delta.y * delta.y);
      float sumRadius = static_cast<float> (A.radius + B.radius);
      if (distance < sumRadius)
      {
        float k = 1.0f - distance / sumRadius;
        olc::vf2d force = delta * (k * k);
        A.acceleration += force;
        B.acceleration -= force;
      } // end IF
    } // end FOR
  } // end FOR
}

void
PGE_406::attractBetweenChanges ()
{
  for (int i = static_cast<int> (atoms_.size ()); i--; )
  {
    atom& A = atoms_[i];
    for (int j = i; j--; )
    {
      atom& B = atoms_[j];
      olc::vf2d delta = A.position - B.position;
      float distance = std::sqrt (delta.x * delta.x + delta.y * delta.y);
      float sumRadius = static_cast<float> (A.radius + B.radius);
      float k = distance / sumRadius || 1.0f;
      olc::vf2d force = delta / (k * k);
      force /= 500.0f;
      force *= (A.type == B.type ? -1.0f : 0.1f);
      A.acceleration += force;
      B.acceleration -= force;
    } // end FOR
  } // end FOR
}

void
PGE_406::attractBetweenChanges2 ()
{
  for (int i = static_cast<int> (atoms_.size()); i--; )
  {
    atom& A = atoms_[i];
    for (int j = i; j--; )
    {
      atom& B = atoms_[j];
      olc::vf2d delta = A.position - B.position;
      float distance = std::sqrt (delta.x * delta.x + delta.y * delta.y);
      float sumRadius = static_cast<float> (A.radius + B.radius);
      float k = distance / sumRadius || 1.0f;
      olc::vf2d force = delta / (k * k);
      force /= 3000.0f;
      force *= (A.type != B.type ? -3.0f : 1.0f);
      A.acceleration += force;
      B.acceleration -= force;
    } // end FOR
  } // end FOR
}
