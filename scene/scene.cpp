/*
 * Game project
 *
 * FILE: scene.cpp
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 12.05.2018
 * NOTE: scene logic file
 */

#include "scene.h"
#include "../input/input.h"
#include "../render/text/text.h"
#include "../physics/forces/gravity/gravity.h"

using namespace scene;
using namespace input;

#define TO_STR(A) (#A)

Scene *s_Scene;

Scene::Scene(void) :
  _isGame(false)
{
  // Menu creation
  std::ifstream menuFile("bin//menu//main.menu");
  _menuSyst = new MenuSystem(menuFile);
  menuFile.close();
  // Static objects create
  BallCreate();
  EnviCreate();
  // input initializing
  Input::Initizalize();
  Input &inp = Input::GetInstance();
  inp.InitAll();
} /* End of 'Scene::Scene' function */

Scene::~Scene(void)
{
  delete _menuSyst;
  delete _ball;
  for (auto &it = _playersA.begin(); it != _playersA.end(); it++)
    delete *it;
  for (auto &it = _playersB.begin(); it != _playersB.end(); it++)
    delete *it;
} /* End of 'Scene::~Scene' function */

void Scene::Exit(void)
{
  delete s_Scene;
  render::Render::getInstance().exit();
} /* End of 'Scene::Exit' function */

void Scene::BallCreate(void)
{
  // Render types
  auto &rnd = render::Render::getInstance();

  float rad = 1.5;
  render::GeomPtr geom = rnd.createGeom("ball", geom::Geom().createSphere({0, 0, 0}, rad, 30, 30));
  render::MaterialPtr mtl = rnd.createMaterial("ball_mtl", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0.69f, 0.69f, 1}, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, rnd.createTexture("flat_color.tga"), 1);
  render::PrimPtr ball = rnd.createPrim("ball", geom, mtl);

  // Phys types
  phys::PhysObject *obj = new phys::PhysObject({0, 0, 0}, 1.0f / 30.0f, 0.8f, 0.8f);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.registerObject("ball", obj, phys::bounding_volume_type::SPHERE, &rad);
  instP.applyForceToObj("ball", &Grav);
  _ball = new Ball("ball", ball, obj);
} /* End of 'Scene::BallCreate' function */

void Scene::EnviCreate(void)
{

} /* End of 'EnviCreate' function */

void Scene::PlayersCreate(UINT OneTeamCount)
{
  // Create old data
  for (auto &it = _playersA.begin(); it != _playersA.end(); it++)
    delete *it;
  for (auto &it = _playersB.begin(); it != _playersB.end(); it++)
    delete *it;
  _playersA.clear();
  _playersB.clear();

  Vec3f center{0, 0, 0};
  float posRad = 10;
  double dangle = math::PI / OneTeamCount;
  Vec4f colorA{0.1, 0.5, 0.7, 1.0}, colorB{0.6, 0.2, 0.7, 1.0};
  for (UINT i = 0; i < OneTeamCount; i++)
  {
    string name = string("player") + std::to_string(i); 
    float cosa = (float)cos(dangle * i),
           sina = (float)sin(dangle * i);
    Vec3f dir{posRad * cosa, 0, posRad * sina};
    Vec3f pos = center + dir;
    _playersA.push_back(PlayerCreate(name + "A", pos, -dir, colorA));

    dir = -dir;
    pos = center + dir;
    _playersB.push_back(PlayerCreate(name + "B", pos, -dir, colorB));
  }
} /* End of 'Scene::PlayersCreate' function */

Player * Scene::PlayerCreate(const string &Name, const Vec3f &Pos, const Vec3f &Dir, Vec4f Color)
{
  // Render types
  auto &rnd = render::Render::getInstance();

  float rad = 1.5;
  render::GeomPtr geom = rnd.createGeom(Name + "_geom", geom::Geom().createSphere(Pos, rad, 30, 30));
  render::MaterialPtr mtl = rnd.createMaterial(Name + "_mtl", {{0.01f, 0.01f, 0.01f, 1}, Color, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, rnd.createTexture("flat_color.tga"), 1);
  render::PrimPtr pr = rnd.createPrim(Name, geom, mtl);

  // Phys types
  phys::PhysObject *obj = new phys::PhysObject(Pos, 1.0f / 30.0f, 0.8f, 0.8f);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.registerObject(Name, obj, phys::bounding_volume_type::SPHERE, &rad);
  instP.applyForceToObj(Name, &Grav);
  return new Player(pr, obj, Dir, Name);
} /* End of 'PlayerCreate' function */

void Scene::Response(void)
{
  Input &input = Input::GetInstance();
  // Input update
  input.UpdateAll();

  if (_isGame)
  {
    render::Render &rnd = render::Render::getInstance();
    // Response camera
    rnd.setCamera(0, true, { -60, 20, 60 }, { 0, 1, 0 }, { 0, 1, 0 });

    // Response scene
    // Phys response
    phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();
    physSys.response();

    // Buttons control
    if (input.KeyHit(DIK_ESCAPE))
      _isGame = false;
    // ball
    
  }
  else
  {
    // Response only menu
    bool leftButtPressed = input.MouseKeyHit(MOUSE_LEFT);
    Vec2f mousePos = input.MousePosf();
    string menuRes = _menuSyst->response(leftButtPressed, mousePos[0], mousePos[1]);

    if (menuRes.size() != 0)
    {
      if (menuRes == "exit")
      {
        render::Render::getInstance().exit();
        _isGame = true;
      }
      else if (menuRes == "continue")
      {
        _isGame = true;
      }
      else if (menuRes == "2 players")
      {
        _isGame = true;
        // Menu creation
        std::ifstream menuFile("bin//menu//game.menu");
        delete _menuSyst;
        _menuSyst = new MenuSystem(menuFile);
        menuFile.close();
        TwoPlayersCreate();
      } else if (menuRes == "4 players")
      {
        _isGame = true;
        // Menu creation
        std::ifstream menuFile("bin//menu//game.menu");
        delete _menuSyst;
        _menuSyst = new MenuSystem(menuFile);
        menuFile.close();
        FourPlayersCreate();
      }
    }
  }
} /* End of 'Scene::Response' function */

void Scene::Draw(void)
{
  if (_isGame)
  {
    // Draw scene
    _ball->draw();
    // Draw players
    for (auto &it = _playersA.begin(); it != _playersA.end(); it++)
      (*it)->draw();
    for (auto &it = _playersB.begin(); it != _playersB.end(); it++)
      (*it)->draw();
  }
  else
  {
    // Draw only menu
    _menuSyst->render();
  }
} /* End of 'Scene::Draw' function */

void Scene::TwoPlayersCreate(void)
{
  Vec3f center{0, 0, 0};
  PlayersCreate(1);
  _ball->SetPos(center);
} /* End of 'Scene::TwoPlayersCreate' function */

void Scene::FourPlayersCreate(void)
{
  Vec3f center{0, 0, 0};
  PlayersCreate(2);
  _ball->SetPos(center);
} /* End of 'Scene::FourPlayersCreate' function */

void Scene::Initialize(void)
{
  s_Scene = new Scene();
} /* End of 'Scene::Initialize' function */

void Scene::ResponseS(void)
{
  s_Scene->Response();
  s_Scene->Draw();
} /* End of 'Scene::ResponseS' function */

void Scene::DrawS(void)
{
  s_Scene->Draw();
} /* End of 'Scene::DrawS' function */
/* END OF 'scene.cpp' FILE */
