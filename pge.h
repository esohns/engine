#ifndef PGE_H
#define PGE_H

#include <codecvt>
#include <vector>

#include "olcPixelGameEngine.h"

#include "common_tools.h"

class PGE
 : public olc::PixelGameEngine
{
 public:
  class symbol
  {
   public:
    symbol (int32_t x_in, int32_t y_in, bool first_in)
    {
      x = x_in; y = y_in;
      setRandom (0);
      switchInterval = Common_Tools::getRandomNumber (2, 20);
      first = first_in;
    }

    void setRandom (int32_t frameCount_in)
    {
      if ((frameCount_in % switchInterval) == 0)
      //value = 0x30A0 + (rand () % 97);
        value = 33 + (rand () % 94); // 'printable' characters
    }

    void update (int32_t height_in,
                 int32_t speed_in)
    {
      if (y >= height_in)
        y = 0;
      else
        y += speed_in;
    }

    int32_t x, y;
    uint16_t value;
    int32_t switchInterval;
    bool first;
  };

  class stream
  {
   public:
    stream ()
    {
      numberOfSymbols = Common_Tools::getRandomNumber (5, 30);
      speed = Common_Tools::getRandomNumber (3, 10);
    }

    void make (int32_t x_in,
               int32_t symbolSize_in)
    {
      for (std::vector<PGE::symbol*>::iterator iterator = symbols.begin ();
           iterator != symbols.end ();
           ++iterator)
        delete *iterator;
      symbols.clear ();

      int32_t y = Common_Tools::getRandomNumber (-250, 0);
      bool first_b = Common_Tools::getRandomNumber (0, 4) == 0;
      for (int32_t i = 0; i < numberOfSymbols; ++i)
      {
        PGE::symbol* symbol_p = new PGE::symbol (x_in, y, first_b);
        ACE_ASSERT (symbol_p);
        y -= symbolSize_in;
        symbols.push_back (symbol_p);
        first_b = false;
      } // end FOR
    }

    void reset (int32_t x_in,
                int32_t symbolSize_in)
    {
      numberOfSymbols = Common_Tools::getRandomNumber(5, 30);
      speed = Common_Tools::getRandomNumber(3, 10);
      make (x_in, symbolSize_in);
    }

    std::vector<PGE::symbol*> symbols;
    int32_t numberOfSymbols;
    int32_t speed;
  };

  PGE ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time

 private:
  void renderSymbol (symbol& symbol_in,
                     int32_t frameCount_in)
  {
    std::wstring wide_string (1, symbol_in.value);
    std::string c_string =
      std::wstring_convert<std::codecvt_utf8<wchar_t> >().to_bytes (wide_string);
    olc::PixelGameEngine::DrawString (symbol_in.x, symbol_in.y,
                                      c_string,
                                      (symbol_in.first ? olc::Pixel (180, 255, 180, 255)
                                                       : olc::Pixel (0, 255, 0, static_cast<uint8_t> (rand () % 256))),
                                      textScale);
    symbol_in.setRandom (frameCount_in);
  }

  void renderStream (stream& stream_in,
                     int32_t frameCount_in)
  {
    bool reset_b = false;
    int32_t x = (*stream_in.symbols.begin ())->x;
next:
    for (std::vector<PGE::symbol*>::iterator iterator = stream_in.symbols.begin ();
         iterator != stream_in.symbols.end ();
         ++iterator)
    {
      renderSymbol (**iterator, frameCount_in);
      (*iterator)->update (olc::PixelGameEngine::ScreenHeight (),
                           stream_in.speed);
      if ((*iterator)->y == 0)
      {
        delete *iterator;
        if (std::next (iterator) == stream_in.symbols.end ())
          reset_b = true;
        stream_in.symbols.erase (iterator);
        goto next;
      } // end IF
    } // end FOR
    if (reset_b)
      stream_in.reset (x,
                       olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")).y);
  }

  std::vector<PGE::stream*> streams;
  int32_t                   textScale;
};

#endif // PGE_H
