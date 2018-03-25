/*
 * Game project
 *
 * FILE: main.cpp
 * AUTHORS:
 *   Denisov Pavel,
 *   Lebed Pavel,
 *   Vasilyev Peter
 * LAST UPDATE: 14.03.2018
 * NOTE: main project file
 */

#include "win\win.h"
#include "render\render.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //MessageBoxA(NULL, "Top Game Development :-)", "Game App", 0);

  Win win;

  win.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
