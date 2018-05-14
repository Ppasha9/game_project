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
#include "environment/environment.h"
#include "../render/text/text.h"
#include <vector>

/* Scene namespace */
namespace scene
{
  using pvec = std::vector<Player *>;
  using presetVec = std::vector<moveMap>;

  const static UINT MAX_PLAYERS_COUNT = 4;
  const static float KICK_FORCE = 8.0f;
  const static float KICK_DIST = 50.0f;

  /* Scene class */
  class Scene
  {
  private:
    // Static objects
    Environment *_envi;
    /// Active objects
    Ball *_ball;
    pvec _playersA;
    pvec _playersB;
    /// Menu and logic
    bool _isGame;
    MenuSystem *_menuSyst;
    // Control presets
    presetVec _presets;

    // Scores
    math::Vec2i _score;
    render::Text _scoreText;

    // Creation part
    Scene(void);

    ~Scene(void);

    void Exit(void);

    void BallCreate(void);

    void EnviCreate(void);

    void LightUpdate(void);

    void PlayersCreate(UINT OneTeamCount);

    Player *PlayerCreate(const moveMap &Preset, const string &Name, const Vec3f &Pos, const Vec3f &Dir, Vec4f Color);

    void TwoPlayersCreate(void);

    void FourPlayersCreate(void);

    void ControlPresetsCreate(void);

    void Response(void);

    void Draw(void);

    int IsGoal(void) const;

  public:
    static void Initialize(void);

    static void ResponseS(void);

    static void DrawS(void);
  }; /* End of 'Scene' class */
} /* End of 'scene' namespace */

/* END OF 'scene.h' FILE */
