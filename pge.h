#ifndef PGE_H
#define PGE_H

#include <codecvt>
#include <vector>

#include "olcPixelGameEngine.h"
#include "olcPGEX_Font.h"

#include "common_tools.h"

#define SYMBOL_SCALE 1

class PGE
 : public olc::PixelGameEngine
{
 typedef olc::PixelGameEngine inherited;

 public:
  class symbol
  {
   public:
    symbol (int32_t x_in, int32_t y_in, bool first_in)
    {
      x = x_in; y = y_in;
      switchInterval = Common_Tools::getRandomNumber (15, 20);
      first = first_in;
      setRandom (switchInterval);
    }

    void setRandom (int32_t frameCount_in)
    {
      if ((frameCount_in % switchInterval) == 0)
      {
        //value = 0x30A0 + (rand () % 97); // katakana
        //value = 33 + (rand () % 94); // 'printable' characters
        value = Common_Tools::getRandomNumber (32, 127);
      } // end IF
    }

    void update (int32_t height_in,
                 int32_t speed_in)
    {
      if (y >= height_in)
        y = std::numeric_limits<int32_t>::max ();
      else
        y += speed_in;
    }

    int32_t x, y;
    //uint16_t value;
    char value;
    int32_t switchInterval;
    bool first;
  };

  class stream
  {
   public:
    stream ()
     : symbols ()
     , speed (Common_Tools::getRandomNumber (1, 3))
    {}
    ~stream ()
    {
      for (std::vector<PGE::symbol*>::iterator iterator = symbols.begin ();
           iterator != symbols.end ();
           ++iterator)
        delete *iterator;
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
      int32_t numberOfSymbols = Common_Tools::getRandomNumber (5, 30);
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
      speed = Common_Tools::getRandomNumber (1, 3);
      make (x_in, symbolSize_in);
    }

    std::vector<PGE::symbol*> symbols;
    int32_t speed;
  };

  PGE ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
  virtual bool OnUserDestroy ()
  {
    delete font;

    for (std::vector<PGE::stream*>::iterator iterator = streams.begin ();
         iterator != streams.end ();
         ++iterator)
      delete *iterator;
    return true;
  }

 private:
   void renderSymbol (symbol& symbol_in,
                     int32_t frameCount_in)
  {
    //std::wstring wide_string (1, symbol_in.value);
    //std::string c_string =
      //std::wstring_convert<std::codecvt_utf8<wchar_t> >().to_bytes (wide_string);
    //olc::PixelGameEngine::DrawString (symbol_in.x, symbol_in.y,
    //                                  c_string,
    //                                  (symbol_in.first ? olc::Pixel (180, 255, 180, 255)
    //                                                   : olc::Pixel (0, 255, 0, static_cast<uint8_t> (rand () % 256))),
    //                                  textScale);
    std::string c_string (1, symbol_in.value);
    //font.DrawString      (olc::vi2d (symbol_in.x, symbol_in.y),
    //                      c_string,
    //                      (symbol_in.first ? olc::Pixel (180, 255, 180, 255)
    //                                       : olc::Pixel (0, 255, 0, static_cast<uint8_t> (rand () % 256))),
    //                      textScale);
    font->DrawStringDecal (olc::vi2d (symbol_in.x, symbol_in.y),
                           c_string,
                           (symbol_in.first ? olc::Pixel (180, 255, 180, 255)
                                            : olc::Pixel (0, 255, 0, static_cast<uint8_t> (rand () % 256))),
                           olc::vf2d (static_cast<float> (textScale), static_cast<float> (textScale)));

    //DrawProceduralString (symbol_in.x, symbol_in.y,
    //                      c_string,
    //                      textScale,
    //                      (symbol_in.first ? olc::Pixel(180, 255, 180, 255)
    //                                       : olc::Pixel(0, 255, 0, static_cast<uint8_t> (rand() % 256))));

    symbol_in.setRandom (frameCount_in);
  }

  void renderStream (stream& stream_in,
                     int32_t height_in,
                     int32_t frameCount_in)
  {
    static int32_t symbol_size_i =// SYMBOL_SIZE;
      //olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")).y;
      font->GetTextSize (ACE_TEXT_ALWAYS_CHAR ("a")).y * textScale;

    bool reset_b = false;
    int32_t x = (*stream_in.symbols.begin ())->x;
next:
    for (std::vector<PGE::symbol*>::iterator iterator = stream_in.symbols.begin ();
         iterator != stream_in.symbols.end ();
         ++iterator)
    {
      renderSymbol (**iterator, frameCount_in);
      (*iterator)->update (height_in,
                           stream_in.speed);
      if ((*iterator)->y == std::numeric_limits<int32_t>::max ())
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
                       symbol_size_i);
  }

  olc::Font*                font;
  std::vector<PGE::stream*> streams;
  int32_t                   textScale;
};

#endif // PGE_H
