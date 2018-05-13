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
#include "scene\player\player.h"
#include "input\input.h"

#include "sound\sound.h"

void dummyResponse( void )
{
  render::Render &rnd =  render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();
  input::Input &input = input::Input::GetInstance();

  rnd.setSplitScreen(render::Render::SplitScreenMode::FULL);

  physSys.response();
  input.KeyboardUpdate();

  static float angle = 0;
  angle += 0.030f;

  static render::PrimPtr prim = rnd.getPrim("test_prim");
  static render::PrimPtr prim1 = rnd.getPrim("test_prim1");
  static render::PrimPtr primPlane = rnd.getPrim("plane");
  static render::PrimPtr primBox = rnd.getPrim("test_box");
  static render::PrimPtr primBox1 = rnd.getPrim("test_box1");

  //physSys.debugDraw();

  //rnd.drawPrim(prim, physSys.getObjectMatrix("test_prim"));
  //rnd.drawPrim(prim1, physSys.getObjectMatrix("test_prim1"));
  rnd.drawPrim(primPlane, physSys.getObjectMatrix("plane"));
  //rnd.drawPrim(primBox, physSys.getObjectMatrix("test_box"));
  //rnd.drawPrim(primBox1, physSys.getObjectMatrix("test_box1"));

  //rnd.setCamera(0, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(0, true, { -60, 20, 60 }, { 0, 1, 0 }, { 0, 1, 0 });

  rnd.setCamera(1, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(2, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });
  rnd.setCamera(3, true, { 15 * (sin(angle / 3) + cos(angle / 3)), 10, 15 }, { 0, 0, -1 }, { 0, 1, 0 });

  rnd.setLight(0, render::LightSystem::LightSource({sin(angle) * 20, cos(angle) * 20, 20}, 300, {0.30F, 0.30F, 0.30F}));
  rnd.setLight(1, render::LightSystem::LightSource({sin(angle + 2) * 20, 0, 20}, 300, {0.30F, 0.30F, 0}));
  rnd.setLight(2, render::LightSystem::LightSource({sin(angle + 3) * 20, 20, cos(angle + 3) * 20}, 300, {0, 0.30F, 0}));
  rnd.setLight(3, render::LightSystem::LightSource({sin(angle + 4) * 20, cos(angle + 4) * 20, 20}, 300, {0.30F, 0, 0}));
  rnd.setLight(4, render::LightSystem::LightSource({sin(angle + 5) * 20, cos(angle + 5) * 20, 20}, 300, {0, 0.30F, 0.30F}));

  float rad = 3;
  rnd.createPrim("player_prim", "sphere", "mtl", "default");
  phys::PhysObject *plObj = new phys::PhysObject({ 0, 5, 0 }, 1.0F / 50.0F, 0.8F, 0.8F);
  physSys.registerObject("player_prim", plObj, phys::bounding_volume_type::SPHERE, &rad);

  static scene::Player pl = scene::Player(rnd.getPrim("player_prim"), plObj, { 0, 0, -1 }, "player_prim");

  if (input.KeyHit(DIK_W))
    pl.action(scene::Player::COMMAND_TYPE::MoveForward);
  if (input.KeyHit(DIK_D))
    pl.action(scene::Player::COMMAND_TYPE::MoveRight);
  if (input.KeyHit(DIK_A))
    pl.action(scene::Player::COMMAND_TYPE::MoveLeft);
  if (input.KeyHit(DIK_S))
    pl.action(scene::Player::COMMAND_TYPE::MoveBack);

  pl.draw();

  static snd::Sound sound = snd::Sound("rocky.wav");
  sound.set3DPos({ 0, 0, 0 });

  if (!sound.isPlayed())
    sound.play();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
  //MessageBoxA(NULL, "Top Game Development :-)", "Game App", 0);

  render::Render &rnd =  render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();
  input::Input::Initizalize();

  input::Input &input = input::Input::GetInstance();

  rnd.init(dummyResponse);
  input.KeyboardInit();

  // init other stuff here
  auto tex = rnd.createTexture("default");
  auto mtl = rnd.createMaterial("mtl", {{0.01f, 0.01f, 0.01f, 1}, {0.79f, 0.79f, 0.79f, 1}, {0.8f, 0.8f, 0.8f, 1}, 100});
  rnd.setMaterialTexture(mtl, tex, 0);
  rnd.setMaterialTexture(mtl, tex, 1);
  rnd.setMaterialTexture(mtl, tex, 2);
  rnd.setMaterialTexture(mtl, tex, 3);
  rnd.createGeom("obj", geom::Geom().loadObj("remade"));
  rnd.createGeom("sphere", geom::Geom().createSphere({ 0, 0, 0 }, 5, 300, 300));
  rnd.createGeom("sphere2", geom::Geom().createSphere({ 0, 0, 0 }, 2.5f, 300, 300));
  rnd.createGeom("box", geom::Geom().createBox({ 0, 0, 0 }, 5));

  rnd.createPrim("test_prim", "sphere", "mtl", "default");

  float rad = 5;
  math::Vec3f pos = { 0, 40, 0 };
  phys::Gravity Grav;
  phys::PhysObject *sphere = new phys::PhysObject(pos, 1.0F / 50.0F, 0.9F, 0.9F);
  //physSys.registerObject("test_prim", sphere, phys::bounding_volume_type::SPHERE, &rad);
  //physSys.applyForceToObj("test_prim", &Grav);
  //sphere->addRotation({ 0, 3, 0 });

  rnd.createPrim("plane", rnd.createGeom("plane", geom::Geom().createPlane({ -180, 0, -180 }, { 180, 0, 180 }, { 0, 1, 0 })));
  struct dummy
  {
    math::Vec3f _normal;
    math::Vec3f _fPoint;
    math::Vec3f _sPoint;
  } dumStruct;
  dumStruct._normal = { 0, 1, 0 };
  dumStruct._fPoint = { -180, -2, -180 };
  dumStruct._sPoint = { 180, -2, 180 };
  physSys.registerObject("plane", { 0, -2, 0 }, 0, 1, 1, phys::bounding_volume_type::PLANE, &dumStruct);

  pos = { -40, 30, 0 };
  rnd.createPrim("test_prim1", "sphere", "mtl", "default");
  phys::PhysObject *sphere1 = new phys::PhysObject(pos, 1.0F / 50.0F, 0.9F, 0.9F);
  //physSys.registerObject("test_prim1", sphere1, phys::bounding_volume_type::SPHERE, &rad);
  //physSys.applyForceToObj("test_prim1", &Grav);
  sphere1->addImpulse({ 480, 0, 0 });
  //sphere1->addRotation({ 0, 3, 0 });

  pos = { 40, 20, 0 };
  struct dummyBox
  {
    math::Vec3f _dirVec;
    math::Vec3f _rightVec;
    float _halfHeight;
  } boxStruct;
  rnd.createPrim("test_box", "box", "mtl", "default");
  boxStruct._dirVec = { 0, 0, -2.5F };
  boxStruct._rightVec = { 2.5F, 0, 0 };
  boxStruct._halfHeight = 2.5F;
  phys::PhysObject *box = new phys::PhysObject(pos, 1.0F / 30.0F, 0.9F, 0.9F);
  //physSys.registerObject("test_box", box, phys::bounding_volume_type::BOX, &boxStruct);
  //physSys.applyForceToObj("test_box", &Grav);
  box->addImpulse({ -400, 0, 0 });
  //box->addRotation({ 0, 3, 0 });

  pos = { 0, 35, 40 };
  rnd.createPrim("test_box1", "box", "mtl", "default");
  box = new phys::PhysObject(pos, 1.0F / 30.0F, 0.9F, 0.9F);
  //physSys.registerObject("test_box1", box, phys::bounding_volume_type::BOX, &boxStruct);
  //physSys.applyForceToObj("test_box1", &Grav);
  box->addImpulse({ 0, 0, -420 });

  rnd.run();

  return 0;
}

/* END OF 'main.cpp' FILE */
