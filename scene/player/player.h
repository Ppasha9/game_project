/*
 * Game project
 *
 * FILE: player.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: scene player file declaration
 */

#pragma once

#include "../../physics/phys_system.h"
#include "../../render/render.h"
#include "../../math/math_def.h"

/* Scene namespace */
namespace scene
{
  static const double KICK_DELTA_TIME = 1.5;
  static const double JUMP_DELTA_TIME = 1.6;
  static const float JUMP_COEF = 400;
  /* Command enumeration */
  enum struct COMMAND_TYPE
  {
    MoveForward,
    MoveLeft,
    MoveRight,
    MoveBack,
    MoveJump,
    MoveKick
  }; /* End of 'COMMAND_TYPE' enum */

  using moveMap = std::map<UINT, COMMAND_TYPE>;

  using namespace math;
  /* Player class */
  class Player
  {
  public:
    static const float Radius;

  private:
    /* Impulse coefficient */
    static const float ImpulseCoeff;
    /* Rotation coefficient */
    static const float RotationCoeff;

    /* Starting orientation matrix */
    math::Matr4f _startOri;

    /* The direction vector */
    math::Vec3f _dirVec;
    /* The up vector */
    math::Vec3f _upVec;
    /* Car physic object */
    phys::PhysObject *_obj;
    /* Car primitive object */
    render::PrimPtr _prim;
    /* Primitive name */
    std::string _name;

    /* The rotation vector on previous frame */
    math::Vec3f _oldRot;
    /* Rotation vector current length */
    float _curW;

    // Time needed to gain maximum rotation
    const static float WMaxTime;

    // controls
    moveMap _moves;
    // Kick time
    double _kickLastTime;
    double _jumpLastTime;
  public:
    /* Default class constructor */
    Player(void) = default;

    /* Class constructor */
    Player(const render::PrimPtr &Prim, phys::PhysObject *Obj, const math::Vec3f &DirVec,
      const std::string &Name, const moveMap &Moves, const math::Matr4f &StartOri);

    /* Action function */
    int action(const COMMAND_TYPE ComType);

    Vec3f GetPos(void);

    void SetPos(const Vec3f &Pos);

    void AddForce(const Vec3f &Forse);

    /* Draw function */
    void draw(void);

    /* Updating function */
    void update(void);

    void SetCamera(UINT Id);

    /* Move by keyboard */
    int MoveKeyboard(const std::vector<UINT> &Moves);

    /* Class destructor */
    ~Player(void);
  }; /* End of 'Player' class */
} /* End of 'scene' namespace */

/* END OF 'player.h' FILE */
