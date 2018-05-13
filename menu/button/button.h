/*
* Game project
*
* FILE: button.h
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 13.05.2018
* NOTE: button handle class declaration
*/

#ifndef _BUTTON_H_INCLUDED__
#define _BUTTON_H_INCLUDED__
#pragma once

#include "..\..\def.h"

/* Rectangle struct */
struct Rect
{
  float _x0, _y0, _w, _h;

  /* Struct constructor */
  Rect( float X0, float Y0, float W, float H ) : _x0(X0), _y0(Y0), _w(W), _h(H)
  {
  } /* End of constructor */
}; /* end of 'rect' struct */

/* Button class */
class Button
{
protected:
  Rect _r;
  math::Vec4f _colCur, _colDef, _colHover, _colText;
  string _name;

public:
  /* Class constructor */
  Button( const string & Name = "test", float X0 = 0.3, float Y0 = 0.3, float W = 0.07, float H = 0.03,
          const math::Vec4f & ColDef = math::Vec4f({200, 235, 134, 1}), const math::Vec4f & ColHover = math::Vec4f({220, 255, 154, 1}),
          const math::Vec4f & ColText = math::Vec4f({0, 0, 0, 1}) ) : _name(Name), _r(X0, Y0, W, H), _colDef(ColDef), _colCur(ColDef), _colHover(ColHover), _colText(ColText)
  {
  } /* End of constructor */

  /* Class constructor by "rect" */
  Button( const string & Name = "test", const Rect & R = Rect(0.3f, 0.3f, 0.07f, 0.03f),
          const math::Vec4f & ColDef = math::Vec4f({200, 235, 134, 1}), const math::Vec4f & ColHover = math::Vec4f({220, 255, 154, 1}),
          const math::Vec4f & ColText = math::Vec4f({0, 0, 0, 1}) ) : _name(Name), _r(R), _colDef(ColDef), _colCur(ColDef), _colHover(ColHover), _colText(ColText)
  {
  } /* End of constructor */

  /* Intersect mouse with button rectangle function */
  bool isInside( float X, float Y )
  {
    return (X > _r._x0 + _r._w || X < _r._x0 || Y > _r._y0 + _r._h || Y < _r._y0) == false;
  } /* End of 'isInside' function */

  /* Getting button name */
  std::string getName()
  {
    return _name;
  } /* End of 'getName' function */

  /* Getting button name */
  Rect getRect()
  {
    return _r;
  } /* End of 'getName' function */

  /* Getting button current color */
  math::Vec4f getColor()
  {
    return _colCur;
  } /* End of 'getName' function */

  /* Getting button current color */
  math::Vec4f getTextColor()
  {
    return _colText;
  } /* End of 'getName' function */

  virtual void onHover()
  {
  }

  virtual void onDef()
  {
  }
}; /* End of 'button' class */

#endif /* _BUTTON_H_INCLUDED__ */
