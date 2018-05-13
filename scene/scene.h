/*
 * Game project
 *
 * FILE: scene.h
 * AUTHORS:
 *   Vasilyev Peter
 *   Denisov Pavel,
 *   Kozlov Ilya,
 *   Lebed Pavel
 * LAST UPDATE: 12.05.2018
 * NOTE: scene logic file
 */

#include "ball/ball.h"
#include "player/player.h"
#include "../menu/menu_system.h"
#include <vector>

/* Scene namespace */
namespace scene
{
  using pvec = std::vector<Player *>;

  const static UINT MAX_PLAYERS_COUNT = 4;

  /* Scene class */
  class Scene
  {
  private:
    /// Active objects
    Ball *_ball;
    pvec _player;
    /// Menu and logic
    bool _isGame;
    MenuSystem *_menuSyst;

    // Creation part
    Scene(void);

    void BallCreate(void);

    void EnviCreate(void);

    void PlayersCreate(UINT OneTeamCount);

    void TwoPlayersCreate(void);

    void FourPlayersCreate(void);

    void Response(void);

    void Draw(void);
  public:
    static void Initialize(void);

    static void ResponseS(void);

    static void DrawS(void);
  }; /* End of 'Scene' class */
} /* End of 'scene' namespace */

/* END OF 'scene.h' FILE */
