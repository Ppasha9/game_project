/*
* Game project
*
* FILE: menu_system.h
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 13.05.2018
* NOTE: menu system class declaration
*/

#ifndef _MENU_SYSTEM_H_INCLUDED__
#define _MENU_SYSTEM_H_INCLUDED__
#pragma once

#include <fstream>

#include "..\def.h"
#include "..\render\render.h"

#include "button\button.h"
#include "button\button_click\button_click.h"

using svec = std::vector<std::string>;
using bprimvector = std::vector<render::PrimPtr>;
using bvector = std::vector<Button *>;

/* Menu system class */
class MenuSystem
{
private:
  bvector Buttons;
  /* Checking line size */
  static bool empty( string & Line );
  /* Split line function */
  static svec split( string & Line );
public:
  bprimvector ButtonPrims;

  /* Menu system constructor */
  MenuSystem( std::ifstream & In );

  /* Response function */
  std::string response( bool Pressed, int X, int Y );
  /* Render function */
  void render();
}; /* End of 'MenuSystem' class */

#endif /* _MENU_SYSTEM_H_INCLUDED__ */
