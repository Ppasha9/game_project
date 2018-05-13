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
#include "../../physics/forces/gravity/gravity.h"
using namespace scene;

 /* Class constructor */
Ball::Ball(const std::string &Name, const render::PrimPtr &Prim, phys::PhysObject *Obj) : _obj(Obj), _prim(Prim),
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

Vec3f Ball::GetPos(void)
{
  return _obj->getPos();
} /* End of 'Ball::GetPos' function */

void Ball::SetPos(const Vec3f &Pos)
{
  _obj->reset(Pos);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.applyForceToObj(_name, &Grav);
} /* End of 'Ball::SetPos' function */

void Ball::ApplyForce(const Vec3f &Force)
{
  _obj->addForce(Force);
} /* End of 'Ball::ApplyForce' function */

/* Class destructor */
Ball::~Ball(void)
{
  phys::PhysicsSystem::getInstance().removeObject(_name);
  delete _obj;
} /* End of destructor */

/* END OF 'ball.cpp' FILE */
