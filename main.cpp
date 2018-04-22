/*
 * Game project
 *
 * FILE: main.cpp
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: main project file
 */

#include "render\render.h"
#include "physics\phys_system.h"
#include "physics\forces\gravity\gravity.h"
#include "render\text\text.h"

void dummyResponse( void )
{
  render::Render &rnd =  render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();

  rnd.setSplitScreen(render::Render::SplitScreenMode::FULL);

  static float angle = 0;
  angle += 0.030f;

  static render::PrimPtr prim = rnd.getPrim("test_prim");
  static render::PrimPtr prim1 = rnd.getPrim("test_prim1");
  static render::PrimPtr primPlane = rnd.getPrim("plane");

  rnd.drawPrim(prim);
  rnd.drawPrim(prim1);
  rnd.drawPrim(primPlane);

  static phys::PhysObject *sphere = physSys.getObject("test_prim");
  static phys::PhysObject *sphere1 = physSys.getObject("test_prim1");
  sphere->addTorque({ 30, 69, 30 });
  sphere1->addTorque({ 30, 69, 30 });

  rnd.setPrimMatrix(prim, math::Matr4f().setIdentity());

  //rnd.setCamera(0, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(0, true, { -60, 10, 60 }, { 0, 1, 0 }, { 0, 1, 0 });

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
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();

  rnd.init(dummyResponse);

  // init other stuff here
  auto tex = rnd.createTexture("default");
  auto mtl = rnd.createMaterial("mtl", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0.69f, 0.69f, 1}, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, tex, 0);
  rnd.setMaterialTexture(mtl, tex, 1);
  rnd.setMaterialTexture(mtl, tex, 2);
  rnd.setMaterialTexture(mtl, tex, 3);
  rnd.createGeom("obj", geom::Geom().loadObj("remade"));
  rnd.createGeom("sphere", geom::Geom().createSphere({ 0, 0, 0 }, 5, 300, 300));
  //rnd.createGeom("obj", geom::Geom().createBox({0, 0, 0}, 3));
  rnd.createPrim("test_prim", "sphere", "mtl", "default", render::Prim::ProjMode::FRUSTUM, render::Prim::FillMode::SOLID);

  float rad = 5;
  math::Vec3f pos = { 0, 40, 0 };
  phys::Gravity Grav;
  phys::PhysObject *sphere = new phys::PhysObject(pos, 1.0F / 50.0F, 0.9F, 0.9F);
  physSys.registerObject("test_prim", sphere, phys::bounding_volume_type::SPHERE, &rad);
  physSys.applyForceToObj("test_prim", &Grav);

  rnd.createPrim("plane", rnd.createGeom("plane", geom::Geom().createPlane({ -180, 0, -180 }, { 180, 0, 180 }, { 0, 1, 0 })));
  struct dummy
  {
    math::Vec3f _normal;
    float _offset;
  } dumStruct;
  dumStruct._normal = { 0, 1, 0 };
  dumStruct._offset = -2;
  physSys.registerObject("plane", { 0, -2, 0 }, 0, 1, 1, phys::bounding_volume_type::PLANE, &dumStruct);

  pos = { -40, 30, 0 };
  rnd.createPrim("test_prim1", "sphere", "mtl", "default", render::Prim::ProjMode::FRUSTUM, render::Prim::FillMode::WIREFRAME);
  phys::PhysObject *sphere1 = new phys::PhysObject(pos, 1.0F / 50.0F, 0.9F, 0.9F);
  physSys.registerObject("test_prim1", sphere1, phys::bounding_volume_type::SPHERE, &rad);
  physSys.applyForceToObj("test_prim1", &Grav);
  sphere1->addImpulse({ 360, 0, 0 });

  rnd.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
