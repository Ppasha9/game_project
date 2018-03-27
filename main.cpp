/*
 * Game project
 *
 * FILE: main.cpp
 * AUTHORS:
 *   Denisov Pavel,
 *   Lebed Pavel,
 *   Vasilyev Peter
 * LAST UPDATE: 27.03.2018
 * NOTE: main project file
 */

#include "render\render.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //MessageBoxA(NULL, "Top Game Development :-)", "Game App", 0);

  render::Render &rnd =  render::Render::getInstance();

  rnd.init();

  // init other stuff here

  rnd.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
