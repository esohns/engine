#include "stdafx.h"

#include "pge_218.h"

#include <complex>

#include "ace/Log_Msg.h"

#include "engine_common.h"

// initialize statics
std::atomic<int> PGE_218::workerComplete_ai = 0;

//////////////////////////////////////////

PGE_218::PGE_218 ()
 : olc::PixelGameEngine ()
 , fractal_ (NULL)
 , mode_ (ENGINE_PGE_218_DEFAULT_MODE)
 , iterations_ (ENGINE_PGE_218_DEFAULT_NUMBER_OF_ITERATIONS)
 , vOffset_ (0.0, 0.0)
 , vStartPan_ (0.0, 0.0)
 , vScale_ (0.0, 0.0)
{
  sAppName = ACE_TEXT_ALWAYS_CHAR ("Example 218");
}

bool
PGE_218::OnUserCreate ()
{
  //fractal_ = new int[ScreenWidth() * ScreenHeight()]{ 0 };

  // Using Vector extensions, align memory (not as necessary as it used to be)
  // MS Specific - see std::aligned_alloc for others
  fractal_ =
    (int*)_aligned_malloc (size_t(olc::PixelGameEngine::ScreenWidth () * olc::PixelGameEngine::ScreenHeight ()) * sizeof (int), 64);

  // center fractal
  vOffset_.x = -3.0;
  vOffset_.y = -1.5;
  vScale_.x = olc::PixelGameEngine::ScreenWidth ()  / 5.0;
  vScale_.y = olc::PixelGameEngine::ScreenHeight () / 3.0;

  initializeThreadPool ();

  return true;
}

bool
PGE_218::OnUserDestroy ()
{
  // Stop Worker threads
  for (int i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
  {
    workers[i].alive = false;         // Allow thread exit
    workers[i].cvStart.notify_one (); // Fake starting gun
  } // end FOR
  // Clean up worker threads
  for (int i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
    workers[i].thread.join ();

  // Clean up memory
  _aligned_free (fractal_);

  return true;
}

bool
PGE_218::OnUserUpdate (float fElapsedTime)
{
  // Get mouse location this frame
  olc::vd2d vMouse = { (double)olc::PixelGameEngine::GetMouseX(), (double)olc::PixelGameEngine::GetMouseY () };

  // Handle Pan & Zoom
  struct olc::HWButton mouse_state_s = olc::PixelGameEngine::GetMouse (2);
  if (mouse_state_s.bPressed)
    vStartPan_ = vMouse;
  if (mouse_state_s.bHeld)
  {
    vOffset_ -= (vMouse - vStartPan_) / vScale_;
    vStartPan_ = vMouse;
  } // end IF

  olc::vd2d vMouseBeforeZoom;
  ScreenToWorld (vMouse, vMouseBeforeZoom);

  if (olc::PixelGameEngine::GetKey (olc::Key::Q).bHeld || olc::PixelGameEngine::GetMouseWheel () > 0) vScale_ *= 1.1;
  if (olc::PixelGameEngine::GetKey (olc::Key::A).bHeld || olc::PixelGameEngine::GetMouseWheel () < 0) vScale_ *= 0.9;

  olc::vd2d vMouseAfterZoom;
  ScreenToWorld (vMouse, vMouseAfterZoom);
  vOffset_ += (vMouseBeforeZoom - vMouseAfterZoom);

  olc::vi2d pix_tl = { 0,0 };
  olc::vi2d pix_br = { olc::PixelGameEngine::ScreenWidth (), olc::PixelGameEngine::ScreenHeight () };
  olc::vd2d frac_tl = { -2.0, -1.0 };
  olc::vd2d frac_br = { 1.0, 1.0 };

  ScreenToWorld (pix_tl, frac_tl);
  ScreenToWorld (pix_br, frac_br);

  // Handle User Input
  if (olc::PixelGameEngine::GetKey (olc::K1).bPressed) mode_ = 0;
  if (olc::PixelGameEngine::GetKey (olc::K2).bPressed) mode_ = 1;
  if (olc::PixelGameEngine::GetKey (olc::K3).bPressed) mode_ = 2;
  if (olc::PixelGameEngine::GetKey (olc::K4).bPressed) mode_ = 3;
  if (olc::PixelGameEngine::GetKey (olc::K5).bPressed) mode_ = 4;
  if (olc::PixelGameEngine::GetKey (olc::K6).bPressed) mode_ = 5;
  if (olc::PixelGameEngine::GetKey (olc::UP).bPressed) iterations_ += 64;
  if (olc::PixelGameEngine::GetKey (olc::DOWN).bPressed) iterations_ -= 64;
  if (iterations_ < 64) iterations_ = 64;

  // START TIMING
  std::chrono::steady_clock::time_point tp1 = std::chrono::high_resolution_clock::now ();

  // Do the computation
  switch (mode_)
  {
    case 0: fractalBasic (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
    case 1: fractalPreCalculate (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
    case 2: fractalNoComplex (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
    case 3: fractalIntrinsics (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
    case 4: fractalThreads (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
    case 5: fractalThreadPool (pix_tl, pix_br, frac_tl, frac_br, iterations_); break;
  } // end SWITCH

  // STOP TIMING
  std::chrono::steady_clock::time_point tp2 = std::chrono::high_resolution_clock::now ();
  std::chrono::duration<double> elapsedTime = tp2 - tp1;

  // Render result to screen
  for (int y = 0; y < olc::PixelGameEngine::ScreenHeight (); y++)
    for (int x = 0; x < olc::PixelGameEngine::ScreenWidth (); x++)
    {
      int i = fractal_[y * olc::PixelGameEngine::ScreenWidth () + x];
      float n = (float)i;
      static float a = 0.1f;
      // Thank you @Eriksonn - Wonderful Magic Fractal Oddball Man
      olc::PixelGameEngine::Draw (x, y,
                                  olc::PixelF (0.5f * std::sin (a * n) + 0.5f,
                                               0.5f * std::sin (a * n + 2.094f) + 0.5f,
                                               0.5f * std::sin (a * n + 4.188f) + 0.5f));
    } // end FOR

  // Render UI
  switch (mode_)
  {
    case 0: olc::PixelGameEngine::DrawString (0, 0, "1) Naive Method", olc::WHITE, 3); break;
    case 1: olc::PixelGameEngine::DrawString (0, 0, "2) Precalculate Method", olc::WHITE, 3); break;
    case 2: olc::PixelGameEngine::DrawString (0, 0, "3) Hand-code Maths Method", olc::WHITE, 3); break;
    case 3: olc::PixelGameEngine::DrawString (0, 0, "4) Vector Extensions (AVX2) Method", olc::WHITE, 3); break;
    case 4: olc::PixelGameEngine::DrawString (0, 0, "5) Threads Method", olc::WHITE, 3); break;
    case 5: olc::PixelGameEngine::DrawString (0, 0, "6) ThreadPool Method", olc::WHITE, 3); break;
  } // end SWITCH

  olc::PixelGameEngine::DrawString (0, 30, "Time Taken: " + std::to_string (elapsedTime.count ()) + "s", olc::WHITE, 3);
  olc::PixelGameEngine::DrawString (0, 60, "Iterations: " + std::to_string (iterations_), olc::WHITE, 3);

  return !olc::PixelGameEngine::GetKey (olc::Key::ESCAPE).bPressed;
}

void
PGE_218::fractalBasic (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, const int iterations)
{
  double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
  double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

  for (int y = pix_tl.y; y < pix_br.y; y++)
    for (int x = pix_tl.x; x < pix_br.x; x++)
    {
      std::complex<double> c (x * x_scale + frac_tl.x, y * y_scale + frac_tl.y);
      std::complex<double> z (0.0, 0.0);

      int n = 0;
      while (std::abs (z) < 2.0 && n < iterations)
      {
        z = (z * z) + c;
        n++;
      } // end WHILE

      fractal_[y * olc::PixelGameEngine::ScreenWidth () + x] = n;
    } // end FOR
}

void
PGE_218::fractalPreCalculate (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
{
  double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
  double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

  double x_pos = frac_tl.x;
  double y_pos = frac_tl.y;

  int y_offset = 0;
  int row_size = pix_br.x - pix_tl.x;

  int x, y, n;
  std::complex<double> c, z;

  for (y = pix_tl.y; y < pix_br.y; y++)
  {
    x_pos = frac_tl.x;
    for (x = pix_tl.x; x < pix_br.x; x++)
    {
      c = { x_pos, y_pos };
      z = { 0,0 };

      n = 0;
      while (std::abs (z) < 2.0 && n < iterations)
      {
        z = (z * z) + c;
        n++;
      } // end WHILE

      fractal_[y_offset + x] = n;
      x_pos += x_scale;
    } // end FOR

    y_pos += y_scale;
    y_offset += row_size;
  } // end FOR
}

void
PGE_218::fractalNoComplex (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
{
  double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
  double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

  double x_pos = frac_tl.x;
  double y_pos = frac_tl.y;

  int y_offset = 0;
  int row_size = olc::PixelGameEngine::ScreenWidth ();

  int x, y, n;

  double cr = 0;
  double ci = 0;
  double zr = 0;
  double zi = 0;
  double re = 0;
  double im = 0;

  for (y = pix_tl.y; y < pix_br.y; y++)
  {
    x_pos = frac_tl.x;
    ci = y_pos;
    for (x = pix_tl.x; x < pix_br.x; x++)
    {
      cr = x_pos;
      zr = 0;
      zi = 0;

      n = 0;
      while ((zr * zr + zi * zi) < 4.0 && n < iterations)
      {
        re = zr * zr - zi * zi + cr;
        im = zr * zi * 2.0 + ci;
        zr = re;
        zi = im;
        n++;
      } // end WHILE

      fractal_[y_offset + x] = n;
      x_pos += x_scale;
    } // end FOR

    y_pos += y_scale;
    y_offset += row_size;
  } // end FOR
}

void
PGE_218::fractalIntrinsics (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
{
#if defined (HOST_CPU_HAS_AVX2)
#if defined (__AVX2__)
  double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
  double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

  double y_pos = frac_tl.y;

  int y_offset = 0;
  int row_size = olc::PixelGameEngine::ScreenWidth ();

  int x, y;

  __m256d _a, _b, _two, _four, _mask1;
  __m256d _zr, _zi, _zr2, _zi2, _cr, _ci;
  __m256d _x_pos_offsets, _x_pos, _x_scale, _x_jump;
  __m256i _one, _c, _n, _iterations, _mask2;

  _one = _mm256_set1_epi64x (1);
  _two = _mm256_set1_pd (2.0);
  _four = _mm256_set1_pd (4.0);
  _iterations = _mm256_set1_epi64x (iterations);

  _x_scale = _mm256_set1_pd (x_scale);
  _x_jump = _mm256_set1_pd (x_scale * 4);
  _x_pos_offsets = _mm256_set_pd (0, 1, 2, 3);
  _x_pos_offsets = _mm256_mul_pd (_x_pos_offsets, _x_scale);

  for (y = pix_tl.y; y < pix_br.y; y++)
  {
    // Reset x_position
    _a = _mm256_set1_pd (frac_tl.x);
    _x_pos = _mm256_add_pd (_a, _x_pos_offsets);

    _ci = _mm256_set1_pd (y_pos);

    for (x = pix_tl.x; x < pix_br.x; x += 4)
    {
      _cr = _x_pos;
      _zr = _mm256_setzero_pd ();
      _zi = _mm256_setzero_pd ();
      _n = _mm256_setzero_si256 ();

repeat:
      _zr2 = _mm256_mul_pd (_zr, _zr);
      _zi2 = _mm256_mul_pd (_zi, _zi);
      _a = _mm256_sub_pd (_zr2, _zi2);
      _a = _mm256_add_pd (_a, _cr);
      _b = _mm256_mul_pd (_zr, _zi);
      _b = _mm256_fmadd_pd (_b, _two, _ci);
      _zr = _a;
      _zi = _b;
      _a = _mm256_add_pd (_zr2, _zi2);
      _mask1 = _mm256_cmp_pd (_a, _four, _CMP_LT_OQ);
      _mask2 = _mm256_cmpgt_epi64 (_iterations, _n);
      _mask2 = _mm256_and_si256 (_mask2, _mm256_castpd_si256 (_mask1));
      _c = _mm256_and_si256 (_one, _mask2); // Zero out ones where n < iterations
      _n = _mm256_add_epi64 (_n, _c); // n++ Increase all n
      if (_mm256_movemask_pd (_mm256_castsi256_pd (_mask2)) > 0)
        goto repeat;

      fractal_[y_offset + x + 0] = int(_n.m256i_i64[3]);
      fractal_[y_offset + x + 1] = int(_n.m256i_i64[2]);
      fractal_[y_offset + x + 2] = int(_n.m256i_i64[1]);
      fractal_[y_offset + x + 3] = int(_n.m256i_i64[0]);
      _x_pos = _mm256_add_pd (_x_pos, _x_jump);
    } // end FOR

    y_pos += y_scale;
    y_offset += row_size;
  } // end FOR
#else
#error AVX2 support not enabled, aborting
#endif // __AVX2__
#endif // HOST_CPU_HAS_AVX2
}

void
PGE_218::fractalThreads (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
{
  int nSectionWidth = (pix_br.x - pix_tl.x) / ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS;
  double dFractalWidth = (frac_br.x - frac_tl.x) / double(ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS);

  std::thread t[ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS];

  for (int32_t i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
    t[i] =
      std::thread (&PGE_218::fractalIntrinsics,
                   this,
                   olc::vi2d (pix_tl.x + nSectionWidth * (i), pix_tl.y),
                   olc::vi2d (pix_tl.x + nSectionWidth * (i + 1), pix_br.y),
                   olc::vd2d (frac_tl.x + dFractalWidth * double(i), frac_tl.y),
                   olc::vd2d (frac_tl.x + dFractalWidth * double(i + 1), frac_br.y),
                   iterations);
  for (size_t i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
    t[i].join ();
}
