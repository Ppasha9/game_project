/*
 * Game project
 *
 * FILE: ball.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: scene ball file declaration
 */

#pragma once

#include "../../physics/phys_system.h"
#include "../../render/render.h"
#include "../../math/math_def.h"

/* Scene namespace */
namespace scene
{
  using namespace math;
  /* Ball class */
  class Ball
  {
  private:
    /* Ball physic object */
    phys::PhysObject *_obj;
    /* Ball primitive object */
    render::PrimPtr _prim;
    /* Primitive name */
    std::string _name;

  public:
    const static float Radius;

    /* Default class constructor */
    Ball(void) = default;

    /* Class constructor */
    Ball(const std::string &Name, const render::PrimPtr &Prim, phys::PhysObject *Obj);

    /* Draw function */
    void draw(void);

    Vec3f GetPos(void);

    void SetPos(const Vec3f &Pos);

    void ApplyForce(const Vec3f &Force);

    /* Class destructor */
    ~Ball(void);
  }; /* End of 'Ball' class */
} /* End of 'scene' namespace */

/* END OF 'ball.h' FILE */
