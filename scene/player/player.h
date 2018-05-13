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

/* Scene namespace */
namespace scene
{
  /* Player class */
  class Player
  {
  public:
    /* Command enumeration */
    enum struct COMMAND_TYPE
    {
      MoveForward,
      MoveLeft,
      MoveRight,
      MoveBack
    }; /* End of 'COMMAND_TYPE' enum */

  private:
    /* Impulse coefficient */
    static const float ImpulseCoeff;
    /* Rotation coefficient */
    static const float RotationCoeff;

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

    /* Damping coeff */


  public:
    /* Default class constructor */
    Player(void) = default;

    /* Class constructor */
    Player(const render::PrimPtr &Prim, phys::PhysObject *Obj, const math::Vec3f &DirVec, const std::string &Name);

    /* Action function */
    void action(const COMMAND_TYPE ComType);

    /* Draw function */
    void draw(void);

    /* Class destructor */
    ~Player(void);
  }; /* End of 'Player' class */
} /* End of 'scene' namespace */

/* END OF 'player.h' FILE */
