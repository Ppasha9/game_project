/*
 * Game project
 *
 * FILE: text.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: text class declaration module
 */

#pragma once

#include <Windows.h>
#include "../material.h"
#include "../prim.h"

using SWORD = signed short int;

/* Render namespace */
namespace render
{
  /* Text primitive class */
  class Text
  {
  public:

    /* ----------------------------------------------------------- */
    /* Text font class */
    class Font
    {
      friend class Text;

    private:
      static Font _fonts[7];   /* Default fonts array */
      static bool _isFontInit; /* Font initialization flag */

#pragma pack(push, 1)
      /* Font information structure */
      struct INFO
      {
        /* Base letter height */
        WORD _height;

        /* Info bits -
         *   bit 0: smooth - is 1 if smoothing is turned on,
         *   bit 1: unicode - is 1 if it is the unicode charset,
         *   bit 2: italic - is 1 if the font is italic,
         *   bit 3: bold - is 1 if the font is bold,
         *   bit 4: fixedHeigth,
         *   bits 5-7: reserved.
         */
        BYTE _bitField;

        /* Name of the charset being used. */
        BYTE _charSet;
        /* The font height stretch in percentage. 100% means no stretch. */
        WORD _stretchH;
        /* The supersampling level used. */
        BYTE _antiAlias;

        /* The padding for each character (up, right, down, left). */
        BYTE
          _paddingUp,
          _paddingRight,
          _paddingDown,
          _paddingLeft;

        /* Spacing for each character (horizontal and vertical). */
        BYTE
          _spacingHoriz,
          _spacingVert;

        /* The outline thickness for the characters in pixels */
        BYTE _outline;
      }; /* End of 'INFO' structure */

      /* Font information structure */
      struct COMMON
      {
        /* Distance in pixels between each line of text. */
        WORD _lineHeight;
        /* The number of pixels from the absolute top of the line to the base of the characters. */
        WORD _base;

        /* Size of the texture, normally used to scale the x pos of the character image. */
        WORD _scaleW;
        WORD _scaleH;

        /* The number of texture pages included in the font. */
        WORD _pages;

        /* Info bits - bits 0-6: reserved,
         *    bit 7: packed - set to 1 if the monochrome characters have been packed into each
         *                    of the texture channels. In this case alphaChnl describes what
         *                    is stored in each channel.
         */
        BYTE _bitField;

        /* By channels info:
         *   set to 0 if the channel holds the glyph data, 1 if it holds the outline, 2 if it holds
         *   the glyph and the outline, 3 if its set to zero, and 4 if its set to one.
         */
        BYTE
          _alphaChannel,
          _redChannel,
          _greenChannel,
          _blueChannel;
      }; /* End of 'COMMON' structure */

      /* Texture characters description structure */
      struct CHAR_INFO
      {
        /* The character id. */
        DWORD _id;

        /* The left top position of the character image in the texture. */
        WORD _x;
        WORD _y;

        /* The size of the character image in the texture. */
        WORD _width;
        WORD _height;

        /* The offset of the character inmage in the texture. */
        SWORD _xOffset;
        SWORD _yOffset;
        SWORD _xAdvance;

        /* Dummy byte for unknown purpose */
        BYTE _dummy;

        /* The texture channel where the character image is found (1 = blue, 2 = green, 4 = red, 8 = alpha, 15 = all channels). */
        BYTE _channel;
      }; /* End of 'CHAR_INFO' structure */

      /* Kerning pairs description structure */
      struct KERNING_PAIRS
      {
        /* The first character id. */
        DWORD _first;
        /* The second character id. */
        DWORD _second;
        /* How much the x position should be adjusted when drawing the second character immediately following the first. */
        WORD _amount;
      }; /* End of 'KERNING_PAIRS' structure */
#pragma pack(pop)

    public:
      /* Forward declaration */
      enum struct FONT_ID;

    private:
      /* Font id */
      FONT_ID _id;

      CHAR_INFO _fntChars[256];  /* Character descriptions */

      MaterialPtr _material;    /* Font material */
      INFO _fntInfo;             /* Font information */
      COMMON _fntCommon;         /* Font common information */

    public:
      /* Font id enumeral type */
      enum struct FONT_ID
      {
        CUSTOM,
        ARIAL,
        COURIER,
        TIMES,
        VERDANA,
        TAHOMA,
        SANS,
        DEFAULT,
      }; /* End of 'FONT_ID' enum */

      /* Create font function. */
      Font(void)
      {
        memset(_fntChars, 0, sizeof(_fntChars));
        memset(&_fntInfo, 0, sizeof(_fntInfo));
      } /* End of constructor */

      /* Class constructor */
      Font(FONT_ID Id);

      /* Load font function. */
      bool load(const std::string &FontName);
    }; /* End of 'Font' class */
    /* ----------------------------------------------------------- */

    /* Get text primitive. */
    PrimPtr getPrim(void)
    {
      return _prim;
    } /* End of 'getPrim' function */

  public:
    /* Text placement enumeral type */
    enum struct PLACEMENT
    {
      USER,         /* No placement */
      LEFT_UP,      /* Screen left upper corner */
      LEFT_CENTER,  /* Screen left middle */
      LEFT_DOWN,    /* Screen left lower corner */
      RIGHT_UP,     /* Screen right upper corner */
      RIGHT_CENTER, /* Screen right middle */
      RIGHT_DOWN,   /* Screen right lower corner */
      CENTER_UP,    /* Screen upper center */
      CENTER,       /* Screen center */
      CENTER_DOWN   /* Screen lower center */
    }; /* End of 'PLACEMENT' enumeral */

  private:
    std::string _name;    /* Text id name */
    std::string _outStr;  /* Out text */
    float _fontSize;      /* Text font size */
    Font _font;           /* Text font */
    math::Colorf _color;  /* Text color */
    int _x, _y;           /* Text left upper corner coordinates in pixels */
    int _width, _height;  /* Text primitive size in pixels */
    PLACEMENT _placement; /* Text placement */

    /* Maximum leter size in text */
    int
      _maxLetterWidth,  /* Maximum letter width */
      _maxLetterHeight, /* Maximum letter Height */
      _maxYOffset;      /* Maximum letter Y offset */

    /* Trimesh text primitive */
    PrimPtr _prim;

    /* Parse text for primitive size function. */
    void getPrimSize(void);

    /* Creating default primitive function */
    void createDefPrim(void);

  public:
    /* Get text primitive width in pixels function. */
    int getWidth(void) const;

    /* Get text primitive height in pixels function. */
    int getHeight(void) const;

    /* Class constructor */
    Text(const std::string &Name);

    /* Class constructor */
    Text(const std::string &Name, const std::string &OutText, const int X, const int Y, const Font &Font, const float FontSize = 30,
      const math::Colorf &Color = { 1, 1, 1, 1 }, const PLACEMENT Placement = PLACEMENT::USER);

    /* Class constructor */
    Text(const std::string &Name, const std::string &OutText, const PLACEMENT Placement, const Font &Font, const float FontSize = 30,
      const math::Colorf &Color = { 1, 1, 1, 1 }, const int X = 0, const int Y = 0);

    /* Setting font function */
    Text & setFont(const Font &NewFont);

    /* Setting color function */
    Text & setColor(const math::Colorf &NewColor);

    /* Setting font size function. */
    Text & setFontSize(const float NewFontSize);

    /* Setting out text function. */
    Text & setOutText(const std::string &NewOutText);

    /* Setting text left upper corner coordinates function. */
    Text & setLeftTopPos(const int NewX, const int NewY);

    /* Setting text placement function. */
    Text & setPlacement(const PLACEMENT NewPlacement);

    /* Setting primitve parameters function. */
    Text & setPrim(void);

    /* Draw text function. */
    void draw(void);
  }; /* End of 'Text' class */
} /* End of 'render' namespace */

/* END OF 'text.h' FILE */
