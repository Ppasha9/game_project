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

using namespace scene;
using namespace input;

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

void Scene::BallCreate(void)
{

} /* End of 'Scene::BallCreate' function */

void Scene::EnviCreate(void)
{

} /* End of 'EnviCreate' function */

void Scene::PlayersCreate(UINT OneTeamCount)
{

} /* End of 'Scene::PlayersCreate' function */

void Scene::Response(void)
{
  Input &input = Input::GetInstance();
  // Input update
  input.UpdateAll();

  if (_isGame)
  {
    // Response scene
  }
  else
  {
    // Response only menu
    bool leftButtPressed = input.MouseKeyHit(MOUSE_LEFT);
    Vec2f mousePos = input.MousePosf();
    string menuRes = _menuSyst->response(leftButtPressed, mousePos[0], mousePos[1]);

    if (menuRes == "exit")
      exit(0);

    static render::Text mouseText = render::Text("mouse_pos_text", "Mouse Pos: 0 0", 100, 200, render::Text::Font::FONT_ID::ARIAL, 30, {1, 1, 1, 1});
    static Vec2f oldMousePos = 0;

    if (!(oldMousePos == mousePos))
    {
      char txt[300];
      sprintf(txt, "Mouse Pos: %f %f", mousePos[0], mousePos[1]);
      mouseText.setOutText(txt).setPrim();
      oldMousePos = mousePos;
    }

    mouseText.draw();

    if (menuRes.size() != 0)
    {
      _isGame = true;
    }
  }
} /* End of 'Scene::Response' function */

void Scene::Draw(void)
{
  if (_isGame)
  {
    // Draw scene
  }
  else
  {
    // Draw only menu
    _menuSyst->render();
  }
} /* End of 'Scene::Draw' function */

void Scene::TwoPlayersCreate(void)
{
} /* End of 'Scene::TwoPlayersCreate' function */

void Scene::FourPlayersCreate(void)
{
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
