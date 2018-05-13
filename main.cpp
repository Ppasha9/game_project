/*
 * Game project
 *
 * FILE: main.cpp
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 02.05.2018
 * NOTE: main project file
 */

#include "render\render.h"
#include "physics\phys_system.h"
#include "physics\forces\gravity\gravity.h"
#include "render\text\text.h"
#include "scene\scene.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  render::Render &rnd = render::Render::getInstance();
  rnd.init(scene::Scene::ResponseS);
  scene::Scene::Initialize();
  rnd.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
