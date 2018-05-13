/*
* Game project
*
* FILE: button_click.h
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 13.05.2018
* NOTE: button click handle class declaration
*/

#ifndef _BUTTON_CLICK_H_INCLUDED__
#define _BUTTON_CLICK_H_INCLUDED__
#pragma once

#include "..\..\..\def.h"
#include "..\button.h"

class ButtonClick : public Button
{
public:
  /* Class constructor */
  ButtonClick( const string & Name = "click", float X0 = 0.3, float Y0 = 0.3, float W = 0.07, float H = 0.03,
               const math::Vec4f & ColDef = math::Vec4f({20, 235, 134, 1}), const math::Vec4f & ColHover = math::Vec4f({220, 255, 154, 1}),
               const math::Vec4f & ColText = math::Vec4f({0, 0, 0, 1}) ) : Button(Name, X0, Y0, W, H, ColDef, ColHover, ColText)
  {
  } /* End of constructor */

  /* Class constructor by "rect" */
  ButtonClick( const string & Name = "click", const Rect & R = Rect(0.3f, 0.3f, 0.07f, 0.03f),
               const math::Vec4f & ColDef = math::Vec4f({20, 235, 134, 1}), const math::Vec4f & ColHover = math::Vec4f({220, 255, 154, 1}),
               const math::Vec4f & ColText = math::Vec4f({0, 0, 0, 1}) ) : Button(Name, R, ColDef, ColHover, ColText)
  {
  } /* End of constructor */
  void onHover()
  {
    _colCur = _colHover;
  } /* End of 'onHover' function */

  void onDef()
  {
    _colCur = _colDef;
  } /* End of 'onDef' function */
}; /* End of 'button_click' class */

#endif /* _BUTTON_CLICK_H_INCLUDED__ */
