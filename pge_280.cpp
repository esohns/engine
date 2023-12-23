#include "stdafx.h"

#include "pge_280.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "common_gl_tools.h"

#include "common_image_tools.h"

#include "engine_common.h"

#include "defines_13.h"

PGE_280::PGE_280 ()
 : olc::PixelGameEngine ()
 , squares_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 280");
}

bool
PGE_280::OnUserCreate ()
{
  for (int k = 0; k < 1000; k++)
  {
    struct square newSqu = generateSquare ();
    bool canAdd = true;
    for (std::vector<struct square>::iterator iterator = squares_.begin ();
         iterator != squares_.end ();
         ++iterator)
      if (((newSqu.i <= (*iterator).i && newSqu.i + newSqu.s > (*iterator).i) || ((*iterator).i <= newSqu.i && (*iterator).i + (*iterator).s > newSqu.i)) &&
          ((newSqu.j <= (*iterator).j && newSqu.j + newSqu.s > (*iterator).j) || ((*iterator).j <= newSqu.j && (*iterator).j + (*iterator).s > newSqu.j)))
      {
        canAdd = false;
        break;
      }
    if (canAdd)
      squares_.push_back (newSqu);
  } // end FOR

  // filling the holes
  for (int i = 0; i < ENGINE_PGE_280_DEFAULT_M; i++)
    for (int j = 0; j < ENGINE_PGE_280_DEFAULT_N; j++)
    {
      struct square newSqu = generateSquare ();
      newSqu.i = i;
      newSqu.j = j;
      newSqu.s = 1;

      bool canAdd = true;
      for (std::vector<struct square>::iterator iterator = squares_.begin ();
           iterator != squares_.end ();
           ++iterator)
        if (((newSqu.i <= (*iterator).i && newSqu.i + newSqu.s > (*iterator).i) || ((*iterator).i <= newSqu.i && (*iterator).i + (*iterator).s > newSqu.i)) &&
            ((newSqu.j <= (*iterator).j && newSqu.j + newSqu.s > (*iterator).j) || ((*iterator).j <= newSqu.j && (*iterator).j + (*iterator).s > newSqu.j)))
        {
          canAdd = false;
          break;
        }
      if (canAdd)
        squares_.push_back (newSqu);
    } // end FOR

  return true;
}

bool
PGE_280::OnUserUpdate (float fElapsedTime)
{
  static int frame_count_i = 1;

  olc::PixelGameEngine::Clear (olc::BLACK);

  for (std::vector<struct square>::iterator iterator = squares_.begin ();
       iterator != squares_.end ();
       ++iterator)
    drawSquare ((*iterator).i * ENGINE_PGE_280_DEFAULT_U, (*iterator).j * ENGINE_PGE_280_DEFAULT_U, (*iterator).s * ENGINE_PGE_280_DEFAULT_U, (*iterator).step,
                frame_count_i);

  ++frame_count_i;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

PGE_280::square
PGE_280::generateSquare ()
{
  struct square square_s;
  square_s.s = Common_Tools::getRandomNumber (2, 5);
  square_s.i = Common_Tools::getRandomNumber (0, ENGINE_PGE_280_DEFAULT_M - square_s.s + 1);
  square_s.j = Common_Tools::getRandomNumber (0, ENGINE_PGE_280_DEFAULT_N - square_s.s + 1);
  static int array_a[3] = {2, 4, 8};
  int index = Common_Tools::getRandomNumber (0, 2);
  square_s.step = array_a[index] * ENGINE_PGE_280_DEFAULT_U / 8.0f;

  return square_s;
}

void
PGE_280::drawSquare (int x, int y, int s, float step, int frameCount)
{
  static int half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
  static int half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;
  static olc::vf2d center_s (static_cast<float> (half_width_i), static_cast<float> (half_height_i));
  static float hue_f = 52.0f;
  static float r, g, b, r_2, g_2, b_2, r_3, g_3, b_3;
  static bool is_first_b = true;
  if (is_first_b)
  {
    is_first_b = false;
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 1.0f, r, g, b);
    Common_Image_Tools::HSVToRGB (hue_f, 0.2f, 1.0f, r_2, g_2, b_2);
    Common_Image_Tools::HSVToRGB (hue_f, 1.0f, 0.2f, r_3, g_3, b_3);
  } // end IF
  static olc::Pixel colors_a[3] =
    {olc::Pixel (static_cast<uint8_t> (r * 255.0f),   static_cast<uint8_t> (g * 255.0f),   static_cast<uint8_t> (b * 255.0f)),
     olc::Pixel (static_cast<uint8_t> (r_2 * 255.0f), static_cast<uint8_t> (g_2 * 255.0f), static_cast<uint8_t> (b_2 * 255.0f)),
     olc::Pixel (static_cast<uint8_t> (r_3 * 255.0f), static_cast<uint8_t> (g_3 * 255.0f), static_cast<uint8_t> (b_3 * 255.0f))};

  olc::PixelGameEngine::FillRect (x, y, s, s, colors_a[0]);

  float t =
    2.0f * static_cast<float> (M_PI) * (frameCount % ENGINE_PGE_280_DEFAULT_NUMBER_OF_FRAMES) / static_cast<float> (ENGINE_PGE_280_DEFAULT_NUMBER_OF_FRAMES);
  olc::vf2d position (x + s / 2.0f, y + s / 2.0f);
  bool inSpiral =
    std::sin (position.dist (center_s) / 20.0f - t - std::atan2 (position.y - center_s.y, position.x - center_s.x)) < 0.0f;
  olc::Pixel color = colors_a[1];
  if (!inSpiral)
    color = colors_a[2];

  int i = 0;
  for (float x1 = static_cast<float> (x); x1 < static_cast<float> (x + s); x1 += step)
  {
    int j = 0;
    for (float y1 = static_cast<float> (y); y1 < static_cast<float> (y + s); y1 += step)
    {
      if ((i + j) % 2 == 0)
        olc::PixelGameEngine::FillRect (static_cast<int32_t> (x1), static_cast<int32_t> (y1),
                                        static_cast<int32_t> (step), static_cast<int32_t> (step),
                                        color);
      j++;
    } // end FOR
    i++;
  } // end FOR
}
