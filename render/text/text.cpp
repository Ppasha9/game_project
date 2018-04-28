/*
 * Game project
 *
 * FILE: text.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 28.04.2018
 * NOTE: text class definition module
 */

#include "text.h"
#include "../render.h"

using namespace render;

/* Static default fonts declaration */
Text::Font Text::Font::_fonts[7];
bool Text::Font::_isFontInit = false;

/* Class constructor */
Text::Font::Font(FONT_ID Id)
{
  if (Id == FONT_ID::CUSTOM)
    return;

  /* If fonts not initialized */
  if (!_isFontInit)
  {
    _isFontInit = TRUE;

    /* Load fonts */
    _fonts[0].load("arial");
    _fonts[0]._id = FONT_ID::ARIAL;

    _fonts[1].load("courier");
    _fonts[1]._id = FONT_ID::COURIER;

    _fonts[2].load("times");
    _fonts[2]._id = FONT_ID::TIMES;

    _fonts[3].load("default");
    _fonts[3]._id = FONT_ID::DEFAULT;

    _fonts[4].load("verdana");
    _fonts[4]._id = FONT_ID::VERDANA;

    _fonts[5].load("tahoma");
    _fonts[5]._id = FONT_ID::TAHOMA;

    _fonts[6].load("sans");
    _fonts[6]._id = FONT_ID::SANS;
  }
    else
      return;

  *this = _fonts[(int)Id - 1];
} /* End of constructor */

/* Load font function. */
bool Text::Font::load(const std::string &FontName)
{
  /* Input file */
  FILE *in;
  DWORD
    sign,       /* File is .fnt checker */
    blk_len;    /* Offset fail length */
  int
    blk_class,  /* Info letter type */
    ch;         /* Skip font face name character */

  std::string fileName = "bin/font/" + FontName + ".fnt";

  /* Fill class spaces by zero */
  memset(_fntChars, 0, sizeof(_fntChars));

  /* Check file for open */
  if ((in = fopen(fileName.c_str(), "rb")) == nullptr)
    return false;

  /* Check if the file is reallly a .fnt file */
  if (fread(&sign, 4, 1, in) != 1 || sign != ('B' | ('M' << 8) | ('F' << 16) | (3 << 24)))
  {
    fclose(in);
    return false;
  }

  /* Reading tags until eof */
  while ((blk_class = fgetc(in)) != EOF)
  {
    /* Reading emergency offset length */
    fread(&blk_len, 4, 1, in);
    if (blk_class == 1)
    {
      /* Read the font info after the tag */
      fread(&_fntInfo, sizeof(_fntInfo), 1, in);
      /* Skip font face name */
      while ((ch = fgetc(in)) != 0 && ch != EOF)
        ;
    }
    else if (blk_class == 2)
      fread(&_fntCommon, sizeof(_fntCommon), 1, in);
    else if (blk_class == 4)
    {
      int n = blk_len / 20;  // number of elements to get Id from
      CHAR_INFO c;           // char to get id to

      for (int i = 0; i < n; i++)
      {
        /* Reading CHAR_INFO for Id */
        fread(&c, sizeof(CHAR_INFO), 1, in);
        if (c._id >= 256)
          continue;
        _fntChars[c._id] = c;
      }
    }
    else
      /* Skip forward for offset */
      fseek(in, blk_len, SEEK_CUR);
  }

  fclose(in);

  render::Render &render = render::Render::getInstance();

  _material = render.createMaterial(FontName + "_font", { { 0.01f, 0.01f, 0.01f, 1 },{ 0.69f, 0.69f, 0.69f, 1 },{ 0.7f, 0.7f, 0.7f, 1 }, 100 });
  auto tex = render.createTexture("font\\" + FontName + ".tga");
  render.setMaterialTexture(_material, tex, 0);
  render.setMaterialTexture(_material, tex, 1);
  render.setMaterialTexture(_material, tex, 2);
  render.setMaterialTexture(_material, tex, 3);
  _id = FONT_ID::CUSTOM;

  return true;
} /* End of 'load' function */

/* Parse text for primitive size function. */
void Text::getPrimSize(void)
{
  _maxLetterHeight = _font._fntChars[_outStr[0]]._height;
  _maxLetterWidth = _font._fntChars[_outStr[0]]._width;
  _maxYOffset = _font._fntChars[_outStr[0]]._yOffset;

  for (unsigned int i = 1; i < _outStr.size(); i++)
  {
    if (_font._fntChars[_outStr[i]]._height > _maxLetterHeight)
      _maxLetterHeight = _font._fntChars[_outStr[i]]._height;

    if (_font._fntChars[_outStr[i]]._width > _maxLetterWidth)
      _maxLetterWidth = _font._fntChars[_outStr[i]]._width;

    if (_font._fntChars[_outStr[i]]._yOffset > _maxYOffset)
      _maxYOffset = _font._fntChars[_outStr[i]]._yOffset;
  }
} /* End of 'getPrimSize' function */

/* Getting text primitive width in pixels function. */
int Text::getWidth(void) const
{
  return _width;
} /* End of 'getWidth' function */

/* Getting text primitive height in pixels function. */
int Text::getHeight(void) const
{
  return _height;
} /* End of 'getHeight' function */

/* Setting primitve parameters function. */
Text & Text::setPrim(void)
{
  /* Get max letter size */
  getPrimSize();

  float scale = (float)_fontSize / (_maxLetterHeight + _maxYOffset);
  float
    posx = 0,           /* Screen X position */
    posy = 0,           /* Screen Y position */
    size = _fontSize;   /* Font size */

  float maxx = 0, maxy = 0;
  unsigned int
    shiftX = 0,                       /* Current letters count */
    shift = 0,                        /* Shift in vertex array */
    vertexCount = 4 * _outStr.size(), /* Vertex points count */
    indexCount = 6 * _outStr.size();  /* Index count */

  /* Vertices and indices */
  std::vector<geom::Vertex> vertices(vertexCount);
  std::vector<unsigned long> indices(indexCount);

  /* While there is text to convert */
  for (unsigned int i = 0; i < _outStr.size(); i++)
  {
    /* Character information pointer */
    Text::Font::CHAR_INFO *cd = &_font._fntChars[_outStr[i]];

    if (_outStr[i] == '\n' && shiftX < _outStr.size())
    {
      posx = 0;
      posy += size;
      continue;
    }

    /* Check if there is a letter */
    if (cd->_id != 0)
    {
      /* Advances, offsets */
      float cx, cy, dx, dy, xoff, yoff, xadv;

      /* Vertex offsets */
      int
        vpos0 = shift + 0,  /* Vertex offset for left top point */
        vpos1 = shift + 1,  /* Vertex offset for right top point */
        vpos2 = shift + 2,  /* Vertex offset for left bottom point */
        vpos3 = shift + 3;  /* Vertex offset for right bottom point */

      /* Setting advances, offsets */
      xadv = cd->_xAdvance * scale;
      xoff = (float)cd->_xOffset / _maxLetterHeight;
      yoff = (float)cd->_yOffset / _maxLetterHeight;
      cx = (float)cd->_x / _font._fntCommon._scaleW;
      cy = (float)cd->_y / _font._fntCommon._scaleH;
      dx = (float)cd->_width / _font._fntCommon._scaleW;
      dy = (float)cd->_height / _font._fntCommon._scaleH;

      /* Set screen coordinates */
      vertices[vpos0] = geom::Vertex({ posx + cd->_xOffset * scale, posy + cd->_yOffset * scale, 0 }, { 0, 0, 1 }, { cx, 1 - cy });
      vertices[vpos1] = geom::Vertex({ posx + (cd->_xOffset + cd->_width) * scale, posy + cd->_yOffset * scale, 0 }, { 0, 0, 1 }, { cx + dx, 1 - cy });
      vertices[vpos2] = geom::Vertex({ posx + cd->_xOffset * scale, posy + (cd->_height + cd->_yOffset) * scale, 0 }, { 0, 0, 1 }, { cx, 1 - (cy + dy) });
      vertices[vpos3] = geom::Vertex({ posx + (cd->_xOffset + cd->_width) * scale, posy + (cd->_height + cd->_yOffset) * scale, 0 }, { 0, 0, 1 }, { cx + dx, 1 - (cy + dy) });

      if (vertices[vpos3]._pos[0] > maxx)
        maxx = vertices[vpos3]._pos[0];
      if (vertices[vpos3]._pos[1] > maxy)
        maxy = vertices[vpos3]._pos[1];

      posx += xadv;
      shiftX++;
      shift = (shiftX << 2);

      /* Current index */
      int ipos = i * 6;
      indices[ipos + 0] = vpos0;
      indices[ipos + 1] = vpos2;
      indices[ipos + 2] = vpos3;
      indices[ipos + 3] = vpos3;
      indices[ipos + 4] = vpos1;
      indices[ipos + 5] = vpos0;
    }
  }

  render::Render &render = render::Render::getInstance();
  render.releasePrimGeom(_prim);
  render.setPrimGeom(_prim, render.createGeom(_name + "_geom", geom::Geom().createTrimesh(vertices, indices)));

  _width = (int)maxx;
  _height = (int)maxy;

  return *this;
} /* End of 'setPrim' function */

/* Creating default primitive function */
void Text::createDefPrim(void)
{
  render::Render &render = render::Render::getInstance();
  render.releasePrim(_prim);
  _prim = render.createPrim(_name, render.createGeom(_name + "_geom", geom::Geom()), nullptr, render.createShader("text"), Prim::ProjMode::SCREENSPACE_PIXEL);
  render.setMaterialCoeffs(_font._material, { _color });
  render.setPrimMaterial(_prim, _font._material);
} /* End of 'createDefPrim' function */

/* Class constructor */
Text::Text(const std::string &Name) : _name(Name), _placement(PLACEMENT::USER)
{
  createDefPrim();
} /* End of constructor */

/* Class constructor */
Text::Text(const std::string &Name, const std::string &OutText, const int X, const int Y, const Font &Font, const float FontSize,
  const math::Colorf &Color, const PLACEMENT Placement) : _name(Name), _outStr(OutText), _x(X), _y(Y),
  _fontSize(FontSize), _font(Font), _color(Color), _placement(Placement)
{
  createDefPrim();
  setPrim();
} /* End of constructor */

/* Class constructor */
Text::Text(const std::string &Name, const std::string &OutText, const PLACEMENT Placement, const Font &Font, const float FontSize,
  const math::Colorf &Color, const int X, const int Y) : _name(Name), _outStr(OutText), _x(X), _y(Y),
  _fontSize(FontSize), _font(Font), _color(Color), _placement(Placement)
{
  createDefPrim();
  setPrim();
} /* End of constructor */

/* Setting font function. */
Text & Text::setFont(const Font &NewFont)
{
  _font = NewFont;
  return *this;
} /* End of 'setFont' function */

/* Setting color function. */
Text & Text::setColor(const math::Colorf &NewColor)
{
  _color = NewColor;
  return *this;
} /* End of 'setColor' function */

/* Set font size function. */
Text & Text::setFontSize(const float NewFontSize)
{
  _fontSize = NewFontSize;
  return *this;
} /* End of 'setFontSize' function */

/* Set out text function. */
Text & Text::setOutText(const std::string &NewOutText)
{
  _outStr = NewOutText;
  return *this;
} /* End of 'setOutText' function */

/* Set text left upper corner coordinates function. */
Text & Text::setLeftTopPos(const int NewX, const int NewY)
{
  _x = NewX;
  _y = NewY;
  return *this;
} /* End of 'setLeftTopPos' function */

/* Set text placement function. */
Text & Text::setPlacement(const PLACEMENT NewPlacement)
{
  _placement = NewPlacement;
  return *this;
} /* End of 'setPlacement' function */

/* Draw text function. */
void Text::draw(void)
{
  render::Render &render = render::Render::getInstance();

  switch (_placement)
  {
  case PLACEMENT::USER:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate((float)_x, (float)_y, 0));
    break;
  case PLACEMENT::LEFT_UP:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(0, 0, 0));
    break;
  case PLACEMENT::LEFT_CENTER:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(0, ((float)render.getHeight() - _height) / 2, 0));
    break;
  case PLACEMENT::LEFT_DOWN:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(0, (float)render.getHeight() - _height, 0));
    break;
  case PLACEMENT::RIGHT_UP:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate((float)render.getWidth() - _width, 0, 0));
    break;
  case PLACEMENT::RIGHT_CENTER:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate((float)render.getWidth() - _width,
      ((float)render.getHeight() - _height) / 2, 0));
    break;
  case PLACEMENT::RIGHT_DOWN:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate((float)render.getWidth() - _width,
      (float)render.getHeight() - _height, 0));
    break;
  case PLACEMENT::CENTER_UP:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(((float)render.getWidth() - _width) / 2, 0, 0));
    break;
  case PLACEMENT::CENTER:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(((float)render.getWidth() - _width) / 2,
      ((float)render.getHeight() - _height) / 2, 0));
    break;
  case PLACEMENT::CENTER_DOWN:
    render.setPrimMatrix(_prim, math::Matr4f::getTranslate(((float)render.getWidth() - _width) / 2,
      (float)render.getHeight() - _height, 0));
    break;
  }

  render.drawPrim(_prim);
} /* End of 'draw' function */

/* END OF 'text.cpp' FILE */
