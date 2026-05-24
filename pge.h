#ifndef PGE_H
#define PGE_H

#include <codecvt>
#include <vector>

#include "olcPixelGameEngine.h"
//#include "olcPGEX_CustomFont.h"
//#include "olcPGEX_Font.h"

#include "common_tools.h"


#define SYMBOL_SCALE 2
#define SYMBOL_SIZE  16

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
      switchInterval = Common_Tools::getRandomNumber (2, 20);
      first = first_in;
      setRandom (switchInterval);
    }

    void setRandom (int32_t frameCount_in)
    {
      static char alphabet_a[] =
        ACE_TEXT_ALWAYS_CHAR ("0123456789abcdefghijklmnopqrstuvwxyz");

      if ((frameCount_in % switchInterval) == 0)
      {
        //value = 0x30A0 + (rand () % 97); // katakana
        //value = 33 + (rand () % 94); // 'printable' characters
        int index_i = Common_Tools::getRandomNumber (0, static_cast<int> (ACE_OS::strlen (alphabet_a) - 1));
        value = alphabet_a[index_i]; // alphabet characters
        ACE_ASSERT (value);
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
    {
      numberOfSymbols = Common_Tools::getRandomNumber (5, 30);
      speed = Common_Tools::getRandomNumber (3, 10);
    }
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
      numberOfSymbols = Common_Tools::getRandomNumber (5, 30);
      speed = Common_Tools::getRandomNumber (3, 10);
      make (x_in, symbolSize_in);
    }

    std::vector<PGE::symbol*> symbols;
    int32_t numberOfSymbols;
    int32_t speed;
  };

  PGE ();

  virtual bool OnUserCreate ();
  virtual bool OnUserUpdate (float); // elapsed time
  virtual bool OnUserDestroy ()
  {
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
    //font.DrawStringDecal (olc::vi2d (symbol_in.x, symbol_in.y),
    //                      c_string,
    //                      (symbol_in.first ? olc::Pixel (180, 255, 180, 255)
    //                                       : olc::Pixel (0, 255, 0, static_cast<uint8_t> (rand () % 256))),
    //                      olc::vf2d (static_cast<float> (textScale), static_cast<float> (textScale)));
    
    DrawProceduralString (symbol_in.x, symbol_in.y,
                          c_string,
                          textScale,
                          (symbol_in.first ? olc::Pixel(180, 255, 180, 255)
                                           : olc::Pixel(0, 255, 0, static_cast<uint8_t> (rand() % 256))));

    symbol_in.setRandom (frameCount_in);
  }

  void renderStream (stream& stream_in,
                     int32_t height_in,
                     int32_t frameCount_in)
  {
    static int32_t symbol_size_i = SYMBOL_SIZE;
      //olc::PixelGameEngine::GetTextSize (ACE_TEXT_ALWAYS_CHAR ("T")).y;
      //font.GetTextSize (ACE_TEXT_ALWAYS_CHAR ("a")).y * textScale;

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

  const uint8_t glyphData[36][5] =
  {
    {0x1F,0x11,0x11,0x11,0x1F}, // 0
    {0x00,0x02,0x1F,0x00,0x00}, // 1
    {0x1D,0x15,0x15,0x15,0x17}, // 2
    {0x15,0x15,0x15,0x15,0x1F}, // 3
    {0x07,0x04,0x04,0x04,0x1F}, // 4
    {0x17,0x15,0x15,0x15,0x1D}, // 5
    {0x1F,0x15,0x15,0x15,0x1D}, // 6
    {0x01,0x01,0x01,0x01,0x1F}, // 7
    {0x1F,0x15,0x15,0x15,0x1F}, // 8
    {0x17,0x15,0x15,0x15,0x1F}, // 9
    {0x1E,0x05,0x05,0x05,0x1E}, // a (Reversed style)
    {0x1F,0x15,0x15,0x11,0x0A}, // b
    {0x0E,0x11,0x11,0x11,0x11}, // c
    {0x1F,0x11,0x11,0x0A,0x04}, // d
    {0x1F,0x15,0x15,0x11,0x11}, // e
    {0x1F,0x05,0x05,0x01,0x01}, // f
    {0x0E,0x11,0x15,0x15,0x1D}, // g
    {0x1F,0x04,0x04,0x04,0x1F}, // h
    {0x11,0x11,0x1F,0x11,0x11}, // i
    {0x10,0x10,0x11,0x0F,0x01}, // j
    {0x1F,0x04,0x0A,0x11,0x00}, // k
    {0x1F,0x10,0x10,0x10,0x10}, // l
    {0x1F,0x02,0x04,0x02,0x1F}, // m
    {0x1F,0x02,0x04,0x08,0x1F}, // n
    {0x0E,0x11,0x11,0x11,0x0E}, // o
    {0x1F,0x05,0x05,0x05,0x02}, // p
    {0x0E,0x11,0x19,0x11,0x1E}, // q
    {0x1F,0x05,0x09,0x15,0x02}, // r
    {0x12,0x15,0x15,0x15,0x09}, // s
    {0x01,0x01,0x1F,0x01,0x01}, // t
    {0x0F,0x10,0x10,0x10,0x0F}, // u
    {0x03,0x0C,0x10,0x0C,0x03}, // v
    {0x0F,0x10,0x0C,0x10,0x0F}, // w
    {0x11,0x0A,0x04,0x0A,0x11}, // x
    {0x03,0x04,0x18,0x04,0x03}, // y
    {0x11,0x13,0x15,0x19,0x11}  // z
  };
  const std::string mapping = "0123456789abcdefghijklmnopqrstuvwxyz";

  void DrawProceduralGlyph (int x, int y, char c, int pixelSize, olc::Pixel color)
  {
    size_t index = mapping.find(c);
    if (index == std::string::npos) return; // Skip spaces or missing letters safely

    // Read our 5x5 byte columns
    for (int col = 0; col < 5; ++col)
    {
      uint8_t rowBits = glyphData[index][col];
      for (int row = 0; row < 5; ++row)
      {
        // Isolate individual data bit flags
        if (rowBits & (1 << row))
        {
          // Scaled rectangle printing loop
          FillRect(x + (col * pixelSize), 
                   y + (row * pixelSize), 
                   pixelSize, 
                   pixelSize, 
                   color);
        }
      }
    }
  }

  void DrawProceduralString(int x, int y, std::string msg, int glyphScale, olc::Pixel txtColor)
  {
    int currentX = x;
    int spacingWidth = 7 * glyphScale; // 5 bits wide + 2 bits separation spacing

    for (const char& c : msg)
    {
      if (c == ' ')
      {
        currentX += spacingWidth;
        continue;
      }
      DrawProceduralGlyph(currentX, y, c, glyphScale, txtColor);
      currentX += spacingWidth;
    }
  }

  //olc::CustomFont           font;
  //olc::Font                 font;
  std::vector<PGE::stream*> streams;
  int32_t                   textScale;
};

#endif // PGE_H
