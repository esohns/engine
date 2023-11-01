#ifndef PGE_218_H
#define PGE_218_H

#include <immintrin.h>

#include <condition_variable>

#include "olcPixelGameEngine.h"

#include "ace/config-lite.h"

#include "defines_9.h"

class PGE_218
 : public olc::PixelGameEngine
{
 public:
  PGE_218 ();
  inline virtual ~PGE_218 () {}

  // implement olc::PixelGameEngine
  virtual bool OnUserCreate ();
  virtual bool OnUserDestroy ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  int* fractal_;
  int  mode_;
  int  iterations_;

  void fractalBasic (const olc::vi2d&, const olc::vi2d&, const olc::vd2d&, const olc::vd2d&, int);
  void fractalPreCalculate (const olc::vi2d&, const olc::vi2d&, const olc::vd2d&, const olc::vd2d&, int);
  void fractalNoComplex (const olc::vi2d&, const olc::vi2d&, const olc::vd2d&, const olc::vd2d&, int);
  void fractalIntrinsics (const olc::vi2d&, const olc::vi2d&, const olc::vd2d&, const olc::vd2d&, int);
  void fractalThreads (const olc::vi2d&, const olc::vi2d&, const olc::vd2d&, const olc::vd2d&, int);

  struct worker_thread
  {
    olc::vi2d pix_tl = { 0,0 };
    olc::vi2d pix_br = { 0,0 };
    olc::vd2d frac_tl = { 0,0 };
    olc::vd2d frac_br = { 0,0 };
    int iterations = 0;
    std::condition_variable cvStart;
    bool alive = true;
    std::mutex mux;
    int screen_width = 0;
    int* fractal = nullptr;

    std::thread thread;

    void start (const olc::vi2d& ptl, const olc::vi2d& pbr, const olc::vd2d& ftl, const olc::vd2d& fbr, int it)
    {
      pix_tl = ptl;
      pix_br = pbr;
      frac_tl = ftl;
      frac_br = fbr;
      iterations = it;
      std::unique_lock<std::mutex> lm (mux);
      cvStart.notify_one ();
    }

    void create ()
    {
      while (alive)
      {
        std::unique_lock<std::mutex> lm (mux);
        cvStart.wait (lm);

#if defined (HOST_CPU_HAS_AVX2)
#if defined (__AVX2__)
        double x_scale = (frac_br.x - frac_tl.x) / (double(pix_br.x) - double(pix_tl.x));
        double y_scale = (frac_br.y - frac_tl.y) / (double(pix_br.y) - double(pix_tl.y));

        double y_pos = frac_tl.y;

        int y_offset = 0;
        int row_size = screen_width;

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
            _zr = _mm256_setzero_pd();
            _zi = _mm256_setzero_pd();
            _n = _mm256_setzero_si256();

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

#if defined (ACE_WIN32) || defined (ACE_WIN64)
            fractal[y_offset + x + 0] = int(_n.m256i_i64[3]);
            fractal[y_offset + x + 1] = int(_n.m256i_i64[2]);
            fractal[y_offset + x + 2] = int(_n.m256i_i64[1]);
            fractal[y_offset + x + 3] = int(_n.m256i_i64[0]);
#elif defined (ACE_LINUX)
            fractal[y_offset + x + 0] = int(_n[3]);
            fractal[y_offset + x + 1] = int(_n[2]);
            fractal[y_offset + x + 2] = int(_n[1]);
            fractal[y_offset + x + 3] = int(_n[0]);
#else
#error missing implementation, aborting
#endif // ACE_WIN32 || ACE_WIN64 || ACE_LINUX
            _x_pos = _mm256_add_pd (_x_pos, _x_jump);
          } // end FOR

          y_pos += y_scale;
          y_offset += row_size;
        } // end FOR
#else
#error AVX2 support not enabled, aborting
#endif // __AVX2__
#endif // HOST_CPU_HAS_AVX2
        workerComplete_ai++;
      } // end WHILE
    }
  };

  static std::atomic<int> workerComplete_ai;
  struct worker_thread workers[ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS];

  void initializeThreadPool ()
  {
    for (int i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
    {
      workers[i].alive = true;
      workers[i].fractal = fractal_;
      workers[i].screen_width = olc::PixelGameEngine::ScreenWidth ();
      workers[i].thread = std::thread(&worker_thread::create, &workers[i]);
    } // end FOR
  }

  void fractalThreadPool (const olc::vi2d& pix_tl, const olc::vi2d& pix_br, const olc::vd2d& frac_tl, const olc::vd2d& frac_br, int iterations)
  {
    int nSectionWidth = (pix_br.x - pix_tl.x) / ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS;
    double dFractalWidth = (frac_br.x - frac_tl.x) / double(ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS);
    workerComplete_ai = 0;

    for (int32_t i = 0; i < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS; i++)
      workers[i].start (olc::vi2d (pix_tl.x + nSectionWidth * i, pix_tl.y), 
                        olc::vi2d (pix_tl.x + nSectionWidth * (i + 1), pix_br.y), 
                        olc::vd2d (frac_tl.x + dFractalWidth * double(i), frac_tl.y), 
                        olc::vd2d (frac_tl.x + dFractalWidth * double(i + 1), frac_br.y), 
                        iterations);
    while (workerComplete_ai < ENGINE_PGE_218_DEFAULT_MAX_NUMBER_OF_THREADS); // Wait for all workers to complete
  }

  // Pan & Zoom variables
  olc::vd2d vOffset_;
  olc::vd2d vStartPan_;
  olc::vd2d vScale_;

  // Convert coordinates from World Space --> Screen Space
  void WorldToScreen (const olc::vd2d& v, olc::vi2d& n)
  {
    n.x = (int)((v.x - vOffset_.x) * vScale_.x);
    n.y = (int)((v.y - vOffset_.y) * vScale_.y);
  }

  // Convert coordinates from Screen Space --> World Space
  void ScreenToWorld (const olc::vi2d& n, olc::vd2d& v)
  {
    v.x = (double)(n.x) / vScale_.x + vOffset_.x;
    v.y = (double)(n.y) / vScale_.y + vOffset_.y;
  }
};

#endif // PGE_218_H
