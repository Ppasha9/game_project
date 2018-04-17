/*
 * Game project
 *
 * FILE: main.cpp
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 17.04.2018
 * NOTE: main project file
 */

#include "render\render.h"

void dummyResponse( void )
{
  render::Render &rnd =  render::Render::getInstance();

  rnd.setSplitScreen(render::Render::SplitScreenMode::FULL);

  static float angle = 0;
  angle += 0.030f;

  static render::PrimPtr prim = rnd.getPrim("test_prim");

  rnd.setPrimMatrix(prim, math::Matr4f().setIdentity());

  rnd.setCamera(0, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(1, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(2, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(3, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });

  rnd.setLight(0, render::LightSystem::LightSource({sin(angle) * 20, cos(angle) * 20, 20}, 300, {0.30F, 0.30F, 0.30F}));
  rnd.setLight(1, render::LightSystem::LightSource({sin(angle + 2) * 20, 0, 20}, 300, {0.30F, 0.30F, 0}));
  rnd.setLight(2, render::LightSystem::LightSource({sin(angle + 3) * 20, 20, cos(angle + 3) * 20}, 300, {0, 0.30F, 0}));
  rnd.setLight(3, render::LightSystem::LightSource({sin(angle + 4) * 20, cos(angle + 4) * 20, 20}, 300, {0.30F, 0, 0}));
  rnd.setLight(4, render::LightSystem::LightSource({sin(angle + 5) * 20, cos(angle + 5) * 20, 20}, 300, {0, 0.30F, 0.30F}));
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //MessageBoxA(NULL, "Top Game Development :-)", "Game App", 0);

  render::Render &rnd =  render::Render::getInstance();

  rnd.init(dummyResponse);

  // init other stuff here
  auto tex = rnd.createTexture("model_1.tga");
  auto mtl = rnd.createMaterial("mtl", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0.69f, 0.69f, 1}, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, tex, 0);
  rnd.setMaterialTexture(mtl, tex, 1);
  rnd.setMaterialTexture(mtl, tex, 2);
  rnd.setMaterialTexture(mtl, tex, 3);
  //rnd.createGeom("obj", geom::Geom().loadObj("remade"));
  rnd.createGeom("obj", geom::Geom().createBox({0, 0, 0}, 3));
  rnd.createPrim("test_prim", "obj"/*, "mtl"*/);

  rnd.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
