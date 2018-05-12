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

/* Scene namespace */
namespace scene
{
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
    /* Default class constructor */
    Ball(void) = default;

    /* Class constructor */
    Ball(const render::PrimPtr &Prim, phys::PhysObject *Obj, const std::string &Name);

    /* Draw function */
    void draw(void);

    /* Class destructor */
    ~Ball(void);
  }; /* End of 'Ball' class */
} /* End of 'scene' namespace */

/* END OF 'ball.h' FILE */
