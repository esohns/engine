#include "stdafx.h"

#include "pge_438.h"

#include "ace/Log_Msg.h"

#include "engine_common.h"

PGE_438::PGE_438 ()
 : olc::PixelGameEngine ()
 , formationType_ (0)
 , particles_ ()
{
  sAppName = "Example 438";
}

bool
PGE_438::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  for (int i = 0; i < ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES; i++)
    particles_.push_back (particle (Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1)),
                                    Common_Tools::getRandomNumber (0.0f, static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))));

  // Define different mathematical formations
  static auto op1 = [](int i) // Spiral
  {
    float angle =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, 2.0f * static_cast<float> (M_PI) * 5.0f);
    float radius =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, std::min (ENGINE_PGE_438_DEFAULT_WIDTH, ENGINE_PGE_438_DEFAULT_HEIGHT) / 3.0f);
    return olc::vf2d (ENGINE_PGE_438_DEFAULT_WIDTH  / 2.0f + radius * std::cos (angle),
                      ENGINE_PGE_438_DEFAULT_HEIGHT / 2.0f + radius * std::sin (angle));
  };
  static auto op2 = [](int i) // Flower
  {
    float angle =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, 2.0f * static_cast<float> (M_PI) * 8.0f);
    float radius =
      std::sin (4.0f * angle) * std::min (ENGINE_PGE_438_DEFAULT_WIDTH, ENGINE_PGE_438_DEFAULT_HEIGHT) / 4.0f;
    return olc::vf2d (ENGINE_PGE_438_DEFAULT_WIDTH  / 2.0f + radius * std::cos (angle),
                      ENGINE_PGE_438_DEFAULT_HEIGHT / 2.0f + radius * std::sin (angle));
  };
  static auto op3 = [](int i) // Lissajous Curve
  {
    float a = Common_Tools::getRandomNumber (1.0f, 5.0f);
    float b = Common_Tools::getRandomNumber (1.0f, 5.0f);
    float delta = Common_Tools::getRandomNumber (0.0f, 2.0f * static_cast<float> (M_PI));
    float angle =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, 2.0f * static_cast<float> (M_PI) * 5.0f);
    float x = ENGINE_PGE_438_DEFAULT_WIDTH  / 2.0f + (ENGINE_PGE_438_DEFAULT_WIDTH  / 3.0f) * std::sin (a * angle + delta);
    float y = ENGINE_PGE_438_DEFAULT_HEIGHT / 2.0f + (ENGINE_PGE_438_DEFAULT_HEIGHT / 3.0f) * std::sin (b * angle);
    return olc::vf2d (x, y);
  };
  static auto op4 = [](int i) // Circle
  {
    float angle =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, 2.0f * static_cast<float> (M_PI));
    float radius = std::min (ENGINE_PGE_438_DEFAULT_WIDTH, ENGINE_PGE_438_DEFAULT_HEIGHT) / 3.0f;
    return olc::vf2d (ENGINE_PGE_438_DEFAULT_WIDTH  / 2.0f + radius * std::cos (angle),
                      ENGINE_PGE_438_DEFAULT_HEIGHT / 2.0f + radius * std::sin (angle));
  };
  static auto op5 = [](int i) // Heart
  {
    float angle =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), static_cast<float> (-M_PI), static_cast<float> (M_PI));
    float x = 16.0f * static_cast<float> (std::pow (std::sin (angle), 3));
    float y = 13.0f * std::cos (angle) - 5.0f * std::cos (2.0f * angle) - 2.0f * std::cos (3.0f * angle) - std::cos (4.0f * angle);
    return olc::vf2d (ENGINE_PGE_438_DEFAULT_WIDTH  / 2.0f + x * 10.0f,
                      ENGINE_PGE_438_DEFAULT_HEIGHT / 2.0f - y * 10.0f);
  };
  static auto op6 = [](int i) // Random Bezier Curve
  {
    olc::vf2d p0 (Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_WIDTH - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_HEIGHT - 1)));
    olc::vf2d p1 (Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_WIDTH - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_HEIGHT - 1)));
    olc::vf2d p2 (Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_WIDTH - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_HEIGHT - 1)));
    olc::vf2d p3 (Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_WIDTH - 1)),
                  Common_Tools::getRandomNumber (0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_HEIGHT - 1)));
    float t =
      Common_GL_Tools::map (static_cast<float> (i), 0.0f, static_cast<float> (ENGINE_PGE_438_DEFAULT_NUMBER_OF_PARTICLES - 1), 0.0f, 1.0f);

    // cubic bezier curve
    olc::vf2d a = p0 * static_cast<float> (std::pow (1.0f - t, 3));
    olc::vf2d b = p1 * (3.0f * t * static_cast<float> (std::pow (1.0f - t, 2)));
    olc::vf2d c = p2 * (3.0f * static_cast<float> (std::pow (t, 2)) * (1.0f - t));
    olc::vf2d d = p3 * static_cast<float> (std::pow (t, 3));
    return a + b + c + d;
  };
  formations_ = { op1, op2, op3, op4, op5, op6 };

  setFormation ();

  return true;
}

bool
PGE_438::OnUserUpdate (float fElapsedTime)
{
  int pixels =
    olc::PixelGameEngine::GetDrawTargetWidth () * olc::PixelGameEngine::GetDrawTargetHeight ();
  olc::Pixel* p = olc::PixelGameEngine::GetDrawTarget ()->GetData ();
  for (int i = 0; i < pixels; i++)
    p[i].a = (p[i].a > ENGINE_PGE_438_DEFAULT_ALPHA_DECAY ? p[i].a - ENGINE_PGE_438_DEFAULT_ALPHA_DECAY : 0);

  if (olc::PixelGameEngine::GetMouse (0).bPressed ||
      olc::PixelGameEngine::GetKey (olc::Key::SPACE).bPressed)
   {
    int prev_formation_type_i = formationType_;
    while (prev_formation_type_i == formationType_)
      formationType_ = Common_Tools::getRandomNumber (0, static_cast<int> (formations_.size ()) - 1);
    //formationType_ = (formationType_ + 1) % formations_.size ();
    setFormation ();
  } // end IF

  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator)
  {
    (*iterator).update ();
    (*iterator).show (this);
  } // end FOR

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_438::setFormation ()
{
  int i = 0;
  for (std::vector<particle>::iterator iterator = particles_.begin ();
       iterator != particles_.end ();
       ++iterator, ++i)
  {
    //(*iterator).color.a = 0;
    (*iterator).target = formations_[formationType_](i);
    //(*iterator).velocity *= 0.0f;
  } // end FOR
}
