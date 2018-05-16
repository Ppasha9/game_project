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
#include "../physics/phys_system.h"
#include "../sound/sound.h"

using namespace scene;
using namespace input;

Scene *s_Scene;

Scene::Scene(void) :
  _isGame(false), _ballsTouch(new snd::Sound("balls.wav")),
  _ballWallTouch(new snd::Sound("walls.wav")) ,
  _ulta(new snd::Sound("ulta.wav")), _goal(new snd::Sound("goal.wav"))
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
  // Presets create
  ControlPresetsCreate();
  _score = {0, 0};
  _scoreText = render::Text("score_text", "0:0", render::Text::PLACEMENT::CENTER_UP, render::Text::Font::FONT_ID::COURIER, 50, {1, 1, 1, 1});
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

  float rad = Ball::Radius;
  render::GeomPtr geom = rnd.createGeom("ball");
  render::MaterialPtr mtl = rnd.createMaterial("ball_mtl", {{0.01f, 0.01f, 0.01f, 1}, {0.69f, 0.69f, 0.69f, 1}, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, rnd.createTexture("ball.tga"), 1);
  render::PrimPtr ball = rnd.createPrim("ball", geom, mtl);

  // Phys types
  phys::PhysObject *obj = new phys::PhysObject({0, 0, 0}, 1.0f / 2.0f, 0.8f, 0.8f);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.registerObject("ball", obj, phys::bounding_volume_type::SPHERE, &rad);
  instP.applyForceToObj("ball", &Grav);
  _ball = new Ball("ball", ball, obj);
} /* End of 'Scene::BallCreate' function */

void Scene::EnviCreate(void)
{
  _envi = new Environment();
} /* End of 'EnviCreate' function */

void scene::Scene::LightUpdate(void)
{
  auto &rnd = render::Render::getInstance();
  rnd.setLight(0, render::LightSystem::LightSource({0, 69, 0}, 300, {1, 1, 1}));

  int i = 1;

  for (auto &p : _playersA)
    rnd.setLight(i++, render::LightSystem::LightSource(p->GetPos() + math::Vec3f{0.0f, 2.0f, 0.0f}, Environment::_radius, {1, 1, 0}));
  for (auto &p : _playersB)
    rnd.setLight(i++, render::LightSystem::LightSource(p->GetPos() + math::Vec3f{0.0f, 2.0f, 0.0f}, Environment::_radius, {1, 0, 1}));
  for (; i < 5; i++)
    rnd.setLight(i, render::LightSystem::LightSource({0, 0, 0}, 0, {0, 0, 0}));
} /* End of 'scene::Scene::LightUpdate' function */

void Scene::PlayersCreate(UINT OneTeamCount)
{
  // Create old data
  for (auto &it = _playersA.begin(); it != _playersA.end(); it++)
    delete *it;
  for (auto &it = _playersB.begin(); it != _playersB.end(); it++)
    delete *it;
  _playersA.clear();
  _playersB.clear();

  float posRad = Environment::_radius;
  Vec3f center = Environment::_center + Vec3f{0, 1.7f, 0};
  double dangle = math::PI_2 / OneTeamCount;
  Vec4f colorA{0.69, 0.69, 0.30, 1.0}, colorB{0.69, 0.30, 0.69, 1.0};
  for (UINT i = 0; i < OneTeamCount; i++)
  {
    string name = string("player") + std::to_string(i); 
    float cosa = (float)cos(math::PI_2 + dangle * i),
           sina = (float)sin(math::PI_2 + dangle * i);
    Vec3f dir{posRad * cosa, 0, posRad * sina};
    Vec3f pos = center + dir;

    UINT index = min(i * 2, _presets.size());
    const moveMap &preset = _presets[index];

    _playersA.push_back(PlayerCreate(preset, name + "A", pos, -dir, colorA));

    dir = -dir;
    pos = center + dir;
    index = min(i * 2 + 1, _presets.size());
    const moveMap &presetB = _presets[index];
    _playersB.push_back(PlayerCreate(presetB, name + "B", pos, -dir, colorB));
  }
} /* End of 'Scene::PlayersCreate' function */

Player * Scene::PlayerCreate(const moveMap &Preset, const string &Name, const Vec3f &Pos, const Vec3f &Dir, Vec4f Color)
{
  // Render types
  auto &rnd = render::Render::getInstance();

  float rad = Player::Radius;

  // render::GeomPtr geom = rnd.createGeom("name w/o .obj");
  // render::MaterialPtr mtl = rnd.createMaterial(Name + "_mtl", {{0.01f, 0.01f, 0.01f, 1}, Color, {0.7f, 0.7f, 0.7f, 1}, 100});
  // rnd.setMaterialTexture(mtl, rnd.createTexture("name of texture.tga"), 1);
  // render::PrimPtr pr = rnd.createPrim(Name, geom, mtl);


  //render::GeomPtr geom = rnd.createGeom(Name + "_geom", geom::Geom().createSphere({0, 0, 0}, rad, 30, 30));
  render::GeomPtr geom = rnd.createGeom("drone");
  render::MaterialPtr mtl = rnd.createMaterial(Name + "_mtl", {{0.01f * Color[0], 0.01f * Color[1], 0.01f * Color[2], 1}, Color, {0.7f, 0.7f, 0.7f, 1}, 100});
  rnd.setMaterialTexture(mtl, rnd.createTexture("drone.tga"), 1);
  render::PrimPtr pr = rnd.createPrim(Name, geom, mtl);

  // Phys types
  phys::PhysObject *obj = new phys::PhysObject(Pos, 1.0f / 20.0f, 0.8f, 0.8f);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.registerObject(Name, obj, phys::bounding_volume_type::SPHERE, &rad);
  instP.applyForceToObj(Name, &Grav);
  return new Player(pr, obj, Dir, Name, Preset,
    math::Matr4f().getRotateY(atan2(Dir * math::Vec3f{1, 0, 0}, Dir * math::Vec3f{0, 0, -1}), false));
} /* End of 'PlayerCreate' function */

void Scene::Response(void)
{
  Input &input = Input::GetInstance();
  // Input update
  input.UpdateAll();
  auto &rnd = render::Render::getInstance();

  if (_isGame)
  {
    render::Render &rnd = render::Render::getInstance();
    // Response camera
    UINT i = 0;
    std::vector<UINT> keys;
    std::vector<UINT> joys;
    input.KeysHited(keys);
    input.JoyHits(joys);
    for (auto &it = _playersA.begin(); it != _playersA.end(); it++, i++)
    {
      (*it)->SetCamera(i);
      (*it)->update();
      int res = (*it)->MoveKeyboard(keys);
      if (res == 0)
      {
        Vec3f ballPos = _ball->GetPos();
        Vec3f playerPos = (*it)->GetPos();
        Vec3f Dir = ballPos - playerPos;
        float Dist = Dir.length();
        Dir.norm();
        if (Dist >= KICK_DIST)
          continue;
        _ball->ApplyForce(Dir * (KICK_DIST / Dist / Dist) * KICK_FORCE);
        // Sound play
        _ulta->stop();
        _ulta->play();
      }
      // Check gamepad
      res = (*it)->MoveKeyboard(joys);
      if (res == 0)
      {
        Vec3f ballPos = _ball->GetPos();
        Vec3f playerPos = (*it)->GetPos();
        Vec3f Dir = ballPos - playerPos;
        float Dist = Dir.length();
        Dir.norm();
        if (Dist >= KICK_DIST)
          continue;
        _ball->ApplyForce(Dir * (KICK_DIST / Dist / Dist) * KICK_FORCE);
        // Sound play
        _ulta->stop();
        _ulta->play();
      }

    }
    for (auto &it = _playersB.begin(); it != _playersB.end(); it++, i++)
    {
      (*it)->SetCamera(i);
      (*it)->update();
      int res = (*it)->MoveKeyboard(keys);
      if (res == 0)
      {
        Vec3f ballPos = _ball->GetPos();
        Vec3f playerPos = (*it)->GetPos();
        Vec3f Dir = ballPos - playerPos;
        float Dist = Dir.length();
        Dir.norm();
        if (Dist >= KICK_DIST)
          continue;
        _ball->ApplyForce(Dir * (KICK_DIST / Dist / Dist) * KICK_FORCE);
        // Sound play
        _ulta->stop();
        _ulta->play();
      }

      // Check gamepad
      res = (*it)->MoveKeyboard(joys);
      if (res == 0)
      {
        Vec3f ballPos = _ball->GetPos();
        Vec3f playerPos = (*it)->GetPos();
        Vec3f Dir = ballPos - playerPos;
        float Dist = Dir.length();
        Dir.norm();
        if (Dist >= KICK_DIST)
          continue;
        _ball->ApplyForce(Dir * (KICK_DIST / Dist / Dist) * KICK_FORCE);
        // Sound play
        _ulta->stop();
        _ulta->play();
      }
    }
    // Light response
    LightUpdate();

    // Response scene
    // Phys response
    phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();
    physSys.response(PhysResponse);

    // Buttons control
    if (input.KeyHit(DIK_ESCAPE))
    {
      _isGame = false;
    }
    // ball
    
    int isG = IsGoal();
    if (isG != -1)
    {
      // Play goal sound
      _goal->stop();
      _goal->play();

      _score._coords[isG]++;

      char txt[300];
      sprintf(txt, "%i:%i", _score[0], _score[1]);
      _scoreText.setOutText(txt).setPrim();

      if (_playersA.size() == 2)
        FourPlayersCreate();
      else if (_playersA.size() == 1)
        TwoPlayersCreate();
    }
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
        rnd.setSplitScreen(render::Render::SplitScreenMode::HALVES);
        TwoPlayersCreate();
        _score = {0, 0};
        _scoreText.setOutText("0:0").setPrim();
      } else if (menuRes == "4 players")
      {
        _isGame = true;
        // Menu creation
        std::ifstream menuFile("bin//menu//game.menu");
        delete _menuSyst;
        _menuSyst = new MenuSystem(menuFile);
        menuFile.close();
        rnd.setSplitScreen(render::Render::SplitScreenMode::QUARTERS);
        FourPlayersCreate();
        _score = {0, 0};
        _scoreText.setOutText("0:0").setPrim();
      }
    }
  }
} /* End of 'Scene::Response' function */

void Scene::Draw(void)
{
  render::Timer &timer = render::Timer::getInstance();
  static render::Text fpsText = render::Text("fps", "FPS: 0.0", 0, 0, render::Text::Font::FONT_ID::ARIAL, 30, { 1, 0, 0, 1 });
  static float oldfps = 0;

  if (oldfps != timer._fps)
  {
    char txt[300];
    sprintf(txt, "FPS: %f", timer._fps);
    fpsText.setOutText(txt).setPrim();
    oldfps = (float)timer._fps;
  }

  fpsText.draw();

  if (_isGame)
  {
    phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();
    //physSys.debugDraw();

    // Draw scene
    _ball->draw();
    // Draw players
    for (auto &it = _playersA.begin(); it != _playersA.end(); it++)
      (*it)->draw();
    for (auto &it = _playersB.begin(); it != _playersB.end(); it++)
      (*it)->draw();
    _envi->draw();

    _scoreText.draw();
  }
  else
  {
    // Draw only menu
    _menuSyst->render();
  }
} /* End of 'Scene::Draw' function */

void Scene::TwoPlayersCreate(void)
{
  Vec3f center = Environment::_center;
  PlayersCreate(1);
  _ball->SetPos(Environment::_center + Vec3f{0, 5, 0});
} /* End of 'Scene::TwoPlayersCreate' function */

void Scene::FourPlayersCreate(void)
{

  Vec3f center = Environment::_center;
  PlayersCreate(2);
  _ball->SetPos(Environment::_center + Vec3f{0, 5, 0});
} /* End of 'Scene::FourPlayersCreate' function */

void scene::Scene::ControlPresetsCreate(void)
{
  // First key preset
  moveMap preset;
  Input &inp = Input::GetInstance();
  uvec ids;
  inp.GetJoyIds(ids);

  if (ids.size() > 0)
  {
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_DPAD_UP, COMMAND_TYPE::MoveForward));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_DPAD_LEFT, COMMAND_TYPE::MoveLeft));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_DPAD_RIGHT, COMMAND_TYPE::MoveRight));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_DPAD_DOWN, COMMAND_TYPE::MoveBack));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_A, COMMAND_TYPE::MoveJump));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(XINPUT_GAMEPAD_X, COMMAND_TYPE::MoveKick));
  }
  else
  {
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_W, COMMAND_TYPE::MoveForward));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_A, COMMAND_TYPE::MoveLeft));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_D, COMMAND_TYPE::MoveRight));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_S, COMMAND_TYPE::MoveBack));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_Q, COMMAND_TYPE::MoveJump));
    preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_E, COMMAND_TYPE::MoveKick));
    _presets.push_back(preset);
    preset.clear();
  }
  // Second key preset
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_UPARROW, COMMAND_TYPE::MoveForward));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_LEFTARROW, COMMAND_TYPE::MoveLeft));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_RIGHTARROW, COMMAND_TYPE::MoveRight));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_DOWNARROW, COMMAND_TYPE::MoveBack));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_RCONTROL, COMMAND_TYPE::MoveJump));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_RSHIFT, COMMAND_TYPE::MoveKick));
  _presets.push_back(preset);
  preset.clear();
  // Third key preset
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_T, COMMAND_TYPE::MoveForward));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_F, COMMAND_TYPE::MoveLeft));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_H, COMMAND_TYPE::MoveRight));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_G, COMMAND_TYPE::MoveBack));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_R, COMMAND_TYPE::MoveJump));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_Y, COMMAND_TYPE::MoveKick));

  _presets.push_back(preset);
  preset.clear();
  // Fourth key preset
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD8, COMMAND_TYPE::MoveForward));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD4, COMMAND_TYPE::MoveLeft));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD6, COMMAND_TYPE::MoveRight));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD5, COMMAND_TYPE::MoveBack));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD7, COMMAND_TYPE::MoveJump));
  preset.insert(std::pair<UINT, COMMAND_TYPE>(DIK_NUMPAD9, COMMAND_TYPE::MoveKick));
  _presets.push_back(preset);
  preset.clear();
} /* End of 'scene::Scene::ControlPresetsCreate' function */

int Scene::IsGoal(void) const
{
  math::Vec3f ballPos = _ball->GetPos();

  if (ballPos[2] >= Environment::_length / 2 && ballPos[0] <= Environment::_goalWidth / 2 && ballPos[0] >= -Environment::_goalWidth / 2 &&
      ballPos[1] >= 0 && ballPos[1] <= Environment::_goalHeight)
    return 0;

  if (ballPos[2] <= -Environment::_length / 2 && ballPos[0] <= Environment::_goalWidth / 2 && ballPos[0] >= -Environment::_goalWidth / 2 &&
      ballPos[1] >= 0 && ballPos[1] <= Environment::_goalHeight)
    return 1;

  return -1;
} /* End of 'IsGoal' function */

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

void Scene::PhysResponse(const math::Vec3f &Pos, const phys::bounding_volume_type FType, const phys::bounding_volume_type SType)
{
  if (FType == phys::bounding_volume_type::SPHERE && SType == phys::bounding_volume_type::SPHERE)
  {
    s_Scene->_ballsTouch->stop();
    s_Scene->_ballsTouch->play();
  }
  else if (FType == phys::bounding_volume_type::SPHERE && SType == phys::bounding_volume_type::BOX ||
           SType == phys::bounding_volume_type::SPHERE && FType == phys::bounding_volume_type::BOX)
  {
    s_Scene->_ballWallTouch->stop();
    s_Scene->_ballWallTouch->play();
  }
} /* End of 'PhysResponse' function */

/* END OF 'scene.cpp' FILE */
