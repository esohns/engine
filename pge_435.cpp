#include "stdafx.h"

#include "pge_435.h"

#include "ace/Log_Msg.h"

#include "common_tools.h"

#include "engine_common.h"

#include "defines_20.h"

PGE_435::PGE_435 ()
 : olc::PixelGameEngine ()
 , sprite_ (NULL)
 , rippleMap_ ()
 , lastMap_ ()
 , oldIndex_ (0)
 , newIndex_ (0)
 , rippleData_ ()
 , lastDrop_ ()
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 435");
}

PGE_435::~PGE_435 ()
{
  delete sprite_;
}

bool
PGE_435::OnUserCreate ()
{
  sprite_ = new olc::Sprite (ACE_TEXT_ALWAYS_CHAR ("bladen.png"), NULL);
  ACE_ASSERT (sprite_);
  resize (olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight ());

  int32_t map_size =
    olc::PixelGameEngine::ScreenWidth () * (olc::PixelGameEngine::ScreenHeight () + 2) * 2;
  rippleMap_.resize (map_size, 0);
  lastMap_.resize (map_size, 0);

  oldIndex_ = olc::PixelGameEngine::ScreenWidth ();
  newIndex_ = olc::PixelGameEngine::ScreenWidth () * (olc::PixelGameEngine::ScreenHeight () + 3);

  rippleData_ = sprite_->pColData;

  lastDrop_ = std::chrono::high_resolution_clock::now ();

  return true;
}

bool
PGE_435::OnUserUpdate (float fElapsedTime)
{
  // generate rain
  // compute elapsed time
#if defined (ACE_WIN32) || defined (ACE_WIN64)
  std::chrono::steady_clock::time_point tp2 =
    std::chrono::high_resolution_clock::now ();
#elif defined (ACE_LINUX)
  std::chrono::time_point<std::chrono::system_clock, std::chrono::nanoseconds> tp2 =
    std::chrono::high_resolution_clock::now ();
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
  std::chrono::duration<float> elapsed_time = tp2 - lastDrop_;
  std::chrono::milliseconds d =
    std::chrono::duration_cast<std::chrono::milliseconds> (elapsed_time);
  if (d.count () >= ENGINE_PGE_435_DEFAULT_RIPPLE_RATE_MS)
  {
    lastDrop_ = tp2;

    disturb (Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenWidth ()  - 1),
             Common_Tools::getRandomNumber (0, olc::PixelGameEngine::ScreenHeight () - 1));
  } // end IF

  //if (olc::PixelGameEngine::GetMouse (0).bHeld)
    disturb (olc::PixelGameEngine::GetMouseX (), olc::PixelGameEngine::GetMouseY ());

  int32_t data, oldData;
  int32_t currentPixel, newPixel;

  // Swap old and new indexes
  int32_t temp = oldIndex_;
  oldIndex_ = newIndex_;
  newIndex_ = temp;

  int32_t rippleMapIndex = oldIndex_;

  for (int32_t y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    for (int32_t x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    {
      // Calculate new ripple intensity based on neighboring pixels
      data = (
        rippleMap_[rippleMapIndex - olc::PixelGameEngine::ScreenWidth ()] +
        rippleMap_[rippleMapIndex + olc::PixelGameEngine::ScreenWidth ()] +
        rippleMap_[rippleMapIndex - 1] +
        rippleMap_[rippleMapIndex + 1]
        ) >> 1;

      data -= rippleMap_[newIndex_ + rippleMapIndex - oldIndex_];
      data -= data >> 5; // Damping effect

      rippleMap_[newIndex_ + rippleMapIndex - oldIndex_] = data;
      data = 1024 - data;

      oldData = lastMap_[rippleMapIndex - oldIndex_];
      lastMap_[rippleMapIndex - oldIndex_] = data;

      // Only update pixel if there's a change
      if (oldData != data)
      {
        static int32_t half_width_i = olc::PixelGameEngine::ScreenWidth () / 2;
        static int32_t half_height_i = olc::PixelGameEngine::ScreenHeight () / 2;

        int32_t offsetX =
          static_cast<int32_t> ((x - half_width_i)  * data / 1024.0f) + half_width_i;
        int32_t offsetY =
          static_cast<int32_t> ((y - half_height_i) * data / 1024.0f) + half_height_i;

        int32_t boundX =
          std::max (0, std::min (offsetX, olc::PixelGameEngine::ScreenWidth ()  - 1));
        int32_t boundY =
          std::max (0, std::min (offsetY, olc::PixelGameEngine::ScreenHeight () - 1));

        newPixel = boundX + (boundY * olc::PixelGameEngine::ScreenWidth ());
        currentPixel = rippleMapIndex - oldIndex_;

        // Transfer texture color to ripple effect data
        rippleData_[currentPixel] = sprite_->pColData[newPixel];
      } // end IF

      rippleMapIndex++;
    } // end FOR

  // Draw the updated ripple effect on the canvas
  olc::PixelGameEngine::GetDrawTarget ()->pColData = rippleData_;

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

bool
PGE_435::resize (int32_t newWidth, int32_t newHeight)
{
  if (newWidth > sprite_->width || newHeight > sprite_->height)
    return false;

  std::vector<olc::Pixel> colNewData (newWidth * newHeight, olc::BLACK);
  int x_ratio = (int)((sprite_->width << 16) / newWidth) + 1;
  int y_ratio = (int)((sprite_->height << 16) / newHeight) + 1;
  int x2, y2;
  for (int i = 0; i < newHeight; i++) 
    for (int j = 0; j < newWidth; j++)
    {
      x2 = ((j * x_ratio) >> 16);
      y2 = ((i * y_ratio) >> 16);
      colNewData[(i * newWidth) + j] = sprite_->pColData[(y2 * sprite_->width) + x2];
    } // end FOR

  sprite_->width = newWidth;
  sprite_->height = newHeight;
  sprite_->pColData = colNewData;

  return true;
}

void
PGE_435::disturb (int32_t x, int32_t y)
{
  for (int32_t j = y - ENGINE_PGE_435_DEFAULT_RIPPLE_RADIUS; j < y + ENGINE_PGE_435_DEFAULT_RIPPLE_RADIUS; j++)
    for (int32_t k = x - ENGINE_PGE_435_DEFAULT_RIPPLE_RADIUS; k < x + ENGINE_PGE_435_DEFAULT_RIPPLE_RADIUS; k++)
    {
      int index_i = oldIndex_ + (j * olc::PixelGameEngine::ScreenWidth ()) + k;
      if (index_i < 0 || index_i >= rippleMap_.size ())
        continue;

      rippleMap_[index_i] += 512;
    } // end FOR
}
