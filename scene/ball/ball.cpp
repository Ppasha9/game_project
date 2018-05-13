/*
 * Game project
 *
 * FILE: ball.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: scene ball file definition
 */

#include "ball.h"
using namespace scene;

 /* Class constructor */
Ball::Ball(const render::PrimPtr &Prim, phys::PhysObject *Obj, const std::string &Name) : _obj(Obj), _prim(Prim),
  _name(Name)
{
} /* End of constructor */

/* Draw function */
void Ball::draw(void)
{
  render::Render &rnd = render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();

  rnd.drawPrim(_prim, physSys.getObjectMatrix(_name));
} /* End of 'draw' function */

/* Class destructor */
Ball::~Ball(void)
{
  delete _obj;
} /* End of destructor */

/* END OF 'ball.cpp' FILE */
