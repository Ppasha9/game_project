/*
 * Game project
 *
 * FILE: player.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 13.05.2018
 * NOTE: scene player file definition
 */

#include "player.h"
using namespace scene;

/* Impulse coefficient */
const float Player::ImpulseCoeff = 2.0f;
/* Rotation coefficient */
const float Player::RotationCoeff = 1.5f;

/* Class constructor */
Player::Player(const render::PrimPtr &Prim, phys::PhysObject *Obj, const std::string &Name) : _obj(Obj), _dirVec(0), _prim(Prim),
  _name(Name), _upVec(0)
{
} /* End of constructor */

/* Class destructor */
Player::~Player(void)
{
  delete _obj;
} /* End of destructor */

/* Action function */
void Player::action(const COMMAND_TYPE ComType)
{
  math::Matr4f matr = _obj->getTransormMatrix();
  math::Vec4f dirV = { _dirVec[0], _dirVec[1], _dirVec[2], 0 };
  dirV = matr * dirV;
  _dirVec = { dirV[0], dirV[1], dirV[2] };
  _dirVec.normalize();

  math::Vec4f upV = { _upVec[0], _upVec[1], _upVec[2], 0 };
  upV = matr * upV;
  _upVec = { upV[0], upV[1], upV[2] };
  _upVec.normalize();

  switch (ComType)
  {
  case COMMAND_TYPE::MoveForward:
    _obj->addImpulse(_dirVec * ImpulseCoeff);
    break;
  case COMMAND_TYPE::MoveBack:
    _obj->addImpulse(-_dirVec * ImpulseCoeff);
    break;
  case COMMAND_TYPE::MoveLeft:
    _obj->addRotation(_upVec * RotationCoeff);
    break;
  case COMMAND_TYPE::MoveRight:
    _obj->addRotation(-_upVec * RotationCoeff);
    break;
  }
} /* End of 'action' function */

/* Draw function */
void Player::draw(void)
{
  render::Render &rnd = render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();

  rnd.drawPrim(_prim, physSys.getObjectMatrix(_name));
} /* End of 'draw' function */

/* END OF 'player.cpp' FILE */
