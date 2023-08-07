#include "stdafx.h"

#include "pge_82.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "defines_3.h"
#include "engine_common.h"

PGE_82::PGE_82 ()
 : olc::PixelGameEngine ()
 , objects_ ()
 , x_motion_ (0.0)
 , y_motion_ (0.0)
 , color_ (olc::WHITE)
 , hue_ (Common_Tools::getRandomNumber (0.0f, 360.0f))
 , noise_ ()
 , trail_map_ ()
{
  sAppName = "Example 82";
}

bool
PGE_82::OnUserCreate ()
{
  olc::PixelGameEngine::SetPixelMode (olc::Pixel::ALPHA);

  olc::PixelGameEngine::Clear (olc::BLACK);

  //int32_t inc = 0;
  for (int32_t nx = 0; nx < ENGINE_PGE_82_DEFAULT_NNX; nx++)
  {
    float nmx = nx / static_cast<float> (ENGINE_PGE_82_DEFAULT_NNX);
    for (int32_t ny = 0; ny < ENGINE_PGE_82_DEFAULT_NNY; ny++)
    {
      int32_t n = nx + ny * ENGINE_PGE_82_DEFAULT_NNX;
      float nmy = ny / static_cast<float> (ENGINE_PGE_82_DEFAULT_NNY);
      float no = static_cast<float> (noise_.GetValue (nmx * nmy, 0.0, 0.0));
      //float ni = inc / (static_cast<float> (ENGINE_PGE_82_DEFAULT_NNX) * static_cast<float> (ENGINE_PGE_82_DEFAULT_NNY));

      object object_s;
      object_s.p.x =
        olc::PixelGameEngine::ScreenWidth () / 2.0f + std::sin (nmx * static_cast<float> (M_PI) * 2.0f) * (olc::PixelGameEngine::ScreenHeight () / 4.5f);
      object_s.p.y =
        olc::PixelGameEngine::ScreenHeight () / 2.0f + std::cos (nmx * static_cast<float> (M_PI) * 2.0f) * (olc::PixelGameEngine::ScreenHeight () / 4.5f);
      object_s.la = nmx * static_cast<float> (M_PI) * 2.0f;
      object_s.lv = 1.0f;
      object_s.d = 1.0f;
      object_s.n = no;
      object_s.so = 3.0f;
      object_s.sa = 45.0f;
      objects_.push_back (object_s);

      //inc++;
    } // end FOR
  } // end FOR

  for (int i = 0; i < olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight (); i++)
    trail_map_.push_back (0.0f);//i / (width * height);

  return true;
}

bool
PGE_82::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  compute (frame_count_i);
  x_motion_ += 0.05;
  y_motion_ += 0.0002;

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_82::compute (int frameCount_in)
{
  for (int nx = 0; nx < ENGINE_PGE_82_DEFAULT_NNX; nx++)
  {
    for (int ny = 0; ny < ENGINE_PGE_82_DEFAULT_NNY; ny++)
    {
      int32_t n = nx + ny * ENGINE_PGE_82_DEFAULT_NNX;

      float sensor_angle = objects_[n].sa * static_cast<float> (M_PI) / 180.0f;

      // agent sensors
      float lhx = std::round (std::sin (objects_[n].la - sensor_angle) * objects_[n].so);
      float lhy = std::round (std::cos (objects_[n].la - sensor_angle) * objects_[n].so);
      float fhx = std::round (std::sin (objects_[n].la) * objects_[n].so);
      float fhy = std::round (std::cos (objects_[n].la) * objects_[n].so);
      float rhx = std::round (std::sin (objects_[n].la + sensor_angle) * objects_[n].so);
      float rhy = std::round (std::cos (objects_[n].la + sensor_angle) * objects_[n].so);

      int32_t ailx = int(objects_[n].p.x + lhx);
      int32_t aily = int(objects_[n].p.y + lhy);
      int32_t aifx = int(objects_[n].p.x + fhx);
      int32_t aify = int(objects_[n].p.y + fhy);
      int32_t airx = int(objects_[n].p.x + rhx);
      int32_t airy = int(objects_[n].p.y + rhy);

      while (ailx < 0)
        ailx += olc::PixelGameEngine::ScreenWidth ();
      while (aily < 0)
        aily += olc::PixelGameEngine::ScreenHeight ();
      while (aifx < 0)
        aifx += olc::PixelGameEngine::ScreenWidth ();
      while (aify < 0)
        aify += olc::PixelGameEngine::ScreenHeight ();
      while (airx < 0)
        airx += olc::PixelGameEngine::ScreenWidth ();
      while (airy < 0)
        airy += olc::PixelGameEngine::ScreenHeight ();

      ailx %= olc::PixelGameEngine::ScreenWidth ();
      aily %= olc::PixelGameEngine::ScreenHeight ();
      aifx %= olc::PixelGameEngine::ScreenWidth ();
      aify %= olc::PixelGameEngine::ScreenHeight ();
      airx %= olc::PixelGameEngine::ScreenWidth ();
      airy %= olc::PixelGameEngine::ScreenHeight ();

      // sensor sample
      float s1 = trail_map_[ailx + aily * olc::PixelGameEngine::ScreenWidth ()]; // left
      float s2 = trail_map_[aifx + aify * olc::PixelGameEngine::ScreenWidth ()]; // forward
      float s3 = trail_map_[airx + airy * olc::PixelGameEngine::ScreenWidth ()]; // right

      // agent behavior based on sampled sensor
      if (s2 < s1 && s2 < s3)
      {
        // turn left or right randomly
        //obj.la += random(PI / 2, -PI / 2);
        // paper
        //obj.la += (random() > 0.5 ? PI / 16 : -PI / 16);
      } // end IF
      else if (s1 < s3)
      {
        // turn left
        objects_[n].la -= static_cast<float> (M_PI) / 8.0f;
      } // end ELSE IF
      else if (s3 < s1)
      {
        // turn right
        objects_[n].la += static_cast<float> (M_PI) / 8.0f;
      } // end ELSE IF

      objects_[n].p.x += objects_[n].lv * std::sin (objects_[n].la);
      objects_[n].p.y += objects_[n].lv * std::cos (objects_[n].la);

      if (objects_[n].p.x < 0.0f || objects_[n].p.x > static_cast<float> (olc::PixelGameEngine::ScreenWidth () - 1))
        continue;
      if (objects_[n].p.y < 0.0f || objects_[n].p.y > static_cast<float> (olc::PixelGameEngine::ScreenHeight () - 1))
        continue; 

      // deposit diffusion (3x3 mean kernel)
      // should probably go after deposit step though
      int32_t i = int(objects_[n].p.x + objects_[n].p.y * olc::PixelGameEngine::ScreenWidth ());
      float b = trail_map_[i] / 2.0f;

      int32_t lx = int(objects_[n].p.x);
      int32_t ly = int(objects_[n].p.y);

      int32_t xb = lx - 1;
      int32_t yb = ly - 1;
      int32_t xbl = xb < 0 ? olc::PixelGameEngine::ScreenWidth () - 1 : xb;
      int32_t ybl = yb < 0 ? olc::PixelGameEngine::ScreenHeight () - 1 : yb;
      int32_t xbu = (lx + 1) % olc::PixelGameEngine::ScreenWidth ();
      int32_t ybu = (ly + 1) % olc::PixelGameEngine::ScreenHeight ();

      trail_map_[xbl + ly * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[xbu + ly * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[xbl + ybl * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[xbl + ybu * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[xbu + ybl * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[xbu + ybu * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[lx + ybl * olc::PixelGameEngine::ScreenWidth ()] = b;
      trail_map_[lx + ybu * olc::PixelGameEngine::ScreenWidth ()] = b;

      // deposit
      trail_map_[i] = objects_[n].d;

      // agent display
      int ll = 400;
      float t =
        (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (objects_[n].n + x_motion_ * 4.0f, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 2.0f) * (std::pow ((1.0f - std::min (ll, frameCount_in) / static_cast<float> (ll)), 5.5f * (0.5f + (1 - (objects_[n].p.y / static_cast<float> (olc::PixelGameEngine::ScreenHeight ()))) / 2.0f)));
      float sph = (1.0f - std::abs ((0.5f - objects_[n].p.x / static_cast<float> (olc::PixelGameEngine::ScreenWidth ())) * 2.0f));
      uint8_t color =
        224 + static_cast<uint8_t> (Common_GL_Tools::map (static_cast<float> (noise_.GetValue (objects_[n].n + x_motion_ * 2.0, 0.0, 0.0)), -1.0f, 1.0f, 0.0f, 1.0f) * 31.0f * sph);
      float r, g, b_2;
      Common_Image_Tools::HSVToRGB (hue_, 200 / 255.0f, color / 255.0f, r, g, b_2);
      olc::Pixel color_s;
      color_s.r = static_cast<uint8_t> (r * 255.0f);
      color_s.g = static_cast<uint8_t> (g * 255.0f);
      color_s.b = static_cast<uint8_t> (b_2 * 255.0f);
      color_s.a = static_cast<uint8_t> ((t / 3.0f) * 255.0f);
      olc::PixelGameEngine::Draw (objects_[n].p, color_s);
    } // end FOR
  } // end FOR

  // deposit decay
  for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    {
      //float v = trail_map_[x + y * olc::PixelGameEngine::ScreenWidth ()];
      trail_map_[x + y * olc::PixelGameEngine::ScreenWidth ()] -= 0.1f;
      if (trail_map_[x + y * olc::PixelGameEngine::ScreenWidth ()] < 0.0f)
        trail_map_[x + y * olc::PixelGameEngine::ScreenWidth ()] = 0.0f;
    } // end FOR
}
