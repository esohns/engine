#include "stdafx.h"

#include "pge_400.h"

#include <random>

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

PGE_400::PGE_400 ()
 : olc::PixelGameEngine ()
 , currentSplatLayerIndex_ (0)
 , bgColor_ ()
 , palette_ ()
 , splats_ ()
{
  sAppName = "Example 400";
}

bool
PGE_400::OnUserCreate ()
{
  restart ();

  return true;
}

bool
PGE_400::OnUserUpdate (float fElapsedTime)
{
  olc::PixelGameEngine::Clear (bgColor_);

  if (olc::PixelGameEngine::GetMouse (0).bHeld)
  {
    olc::vf2d mouse_position_s (static_cast<float> (olc::PixelGameEngine::GetMouseX ()),
                                static_cast<float> (olc::PixelGameEngine::GetMouseY ()));
    olc::vf2d offset_s (Common_Tools::getRandomNumber (3.0f, 10.0f), Common_Tools::getRandomNumber (3.0f, 10.0f));
    if (Common_Tools::testRandomProbability (0.5))
      offset_s.x = -offset_s.x;
    if (Common_Tools::testRandomProbability (0.5))
      offset_s.y = -offset_s.y;
    float radius_f = Common_Tools::getRandomNumber (12.5f, 25.0f);
    createAndAddOneSplat (mouse_position_s + offset_s, radius_f, splats_[currentSplatLayerIndex_]);
  } // end IF
  if (olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
    restart ();

  for (std::vector< std::vector<struct splat> >::iterator iterator = splats_.begin ();
       iterator != splats_.end ();
       ++iterator)
    for (std::vector<struct splat>::iterator iterator_2 = (*iterator).begin ();
         iterator_2 != (*iterator).end ();
         ++iterator_2)
      drawSplat (*iterator_2);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_400::setupPalette ()
{
  static olc::Pixel bgColor_s (0x18, 0x03, 0x05, 255);
  static olc::Pixel palette_a[6] = {olc::Pixel (0xe8, 0x5b, 0x30, 255),
                                    olc::Pixel (0xef, 0x9e, 0x28, 255),
                                    olc::Pixel (0xc6, 0xac, 0x71, 255),
                                    olc::Pixel (0xe0, 0xc1, 0x91, 255),
                                    olc::Pixel (0x3f, 0x62, 0x79, 255),
                                    olc::Pixel (0xee, 0x85, 0x4e, 255)};

  bgColor_ = bgColor_s;

  palette_.clear ();
  for (int i = 0; i < 6; i++)
    palette_.push_back (palette_a[i]);
  static std::random_device rd;
  static std::default_random_engine rng (rd ());
  std::shuffle (palette_.begin (), palette_.end (), rng);
}

PGE_400::splat
PGE_400::createSplat (const olc::vf2d& position, float radius, olc::Pixel& colour)
{
  float angleStep =
    2.0f * static_cast<float> (M_PI) / static_cast<float> (ENGINE_PGE_400_DEFAULT_POINTS_PER_SPLAT);
  std::vector<olc::vf2d> points_a;
  for (float angle = 0.0f; angle < 2.0f * static_cast<float> (M_PI); angle += angleStep)
  {
    olc::vf2d offset (radius * std::cos (angle), radius * std::sin (angle));
    points_a.push_back (position + offset);
  } // end FOR

  struct splat splat_s;
  splat_s.center = position;
  splat_s.colour = colour;
  splat_s.initialRadius = radius;
  splat_s.points = points_a;

  return splat_s;
}

void
PGE_400::deformSplatBasedOn (struct splat& thisSplat, struct splat& otherSplat)
{
  for (std::vector<olc::vf2d>::iterator iterator = thisSplat.points.begin ();
       iterator != thisSplat.points.end ();
       ++iterator)
  {
    olc::vf2d pMinusC = (*iterator) - otherSplat.center;
    olc::vf2d movement =
      otherSplat.center + (pMinusC * (std::sqrt (1.0f + (otherSplat.initialRadius * otherSplat.initialRadius) / pMinusC.mag2 ())));
    (*iterator) = movement;
  } // end FOR
}

void
PGE_400::createAndAddOneSplat (const olc::vf2d& position, float radius, std::vector<struct splat>& splats)
{
  int index_i = Common_Tools::getRandomNumber (0, static_cast<int> (palette_.size ()) - 1);

  struct splat splat_s = createSplat (position, radius, palette_[index_i]);

  for (std::vector<struct splat>::iterator iterator = splats.begin ();
       iterator != splats.end ();
       ++iterator)
    deformSplatBasedOn ((*iterator), splat_s);

  splats.push_back (splat_s);
}

void
PGE_400::restart ()
{
  setupPalette ();

  splats_.clear ();
  for (int i = 0; i < 3; i++)
  {
    std::vector<struct splat> splats_a;
    splats_.push_back (splats_a);
  } // end FOR

  for (int i = 0; i < ENGINE_PGE_400_DEFAULT_INITIAL_SPLATS; i++)
  {
    olc::vf2d position_s (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                          Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1)));
    float radius_f = Common_Tools::getRandomNumber (50.0f, 200.0f);
    createAndAddOneSplat (position_s, radius_f, splats_[currentSplatLayerIndex_]);
  } // end FOR
}

void
PGE_400::drawSplat (struct splat& splat)
{
  olc::PixelGameEngine::DrawPolygonDecal (NULL, splat.points, splat.points, splat.colour);
}
