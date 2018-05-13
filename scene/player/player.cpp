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
#include "../../physics/forces/gravity/gravity.h"
using namespace scene;

/* Impulse coefficient */
const float Player::ImpulseCoeff = 6.0f;
/* Rotation coefficient */
const float Player::RotationCoeff = 0.005f;

/* Class constructor */
Player::Player(const render::PrimPtr &Prim, phys::PhysObject *Obj, const math::Vec3f &DirVec, const std::string &Name) :
  _obj(Obj), _dirVec(DirVec), _prim(Prim), _name(Name), _upVec({ 0, 1, 0 }), _oldRot({0, 0, 0})
{
} /* End of constructor */

/* Class destructor */
Player::~Player(void)
{
  phys::PhysicsSystem::getInstance().removeObject(_name);
  delete _obj;
} /* End of destructor */

/* Action function */
void Player::action(const COMMAND_TYPE ComType)
{
  math::Matr4f matr = _obj->getTransormMatrix();
  matr = matr.getTranspose();

  math::Vec4f dirV = { _dirVec[0], _dirVec[1], _dirVec[2], 0 };
  dirV = matr * dirV;
  dirV.normalize();
  math::Vec3f dirV3 = { dirV[0], dirV[1], dirV[2] };

  math::Vec4f upV = { _upVec[0], _upVec[1], _upVec[2], 0 };
  upV = matr * upV;
  upV.normalize();
  math::Vec3f upV3 = { upV[0], upV[1], upV[2] };

  switch (ComType)
  {
  case COMMAND_TYPE::MoveForward:
    _obj->addImpulse(dirV3 * ImpulseCoeff);
    break;
  case COMMAND_TYPE::MoveBack:
    _obj->addImpulse(-dirV3 * ImpulseCoeff);
    break;
  case COMMAND_TYPE::MoveLeft:
    _obj->addRotation(-_oldRot);
    _obj->addRotation(upV3 * RotationCoeff);
    _oldRot = upV3 * RotationCoeff;
    break;
  case COMMAND_TYPE::MoveRight:
    _obj->addRotation(-_oldRot);
    _obj->addRotation(-upV3 * RotationCoeff);
    _oldRot = -upV3 * RotationCoeff;
    break;
  }
} /* End of 'action' function */

Vec3f Player::GetPos(void)
{
  return _obj->getPos();
} /* End of 'Player::GetPos' function */

void Player::SetPos(const Vec3f &Pos)
{
  _obj->reset(Pos);
  phys::Gravity Grav;
  auto &instP = phys::PhysicsSystem::getInstance();
  instP.applyForceToObj(_name, &Grav);
} /* End of 'Player::SetPos' function */

void Player::AddForce(const Vec3f &Force)
{
  _obj->addForce(Force);
} /* End of 'Player::AddForce' function */

/* Draw function */
void Player::draw(void)
{
  render::Render &rnd = render::Render::getInstance();
  phys::PhysicsSystem &physSys = phys::PhysicsSystem::getInstance();

  rnd.drawPrim(_prim, physSys.getObjectMatrix(_name));
} /* End of 'draw' function */

/* Updating function */
void Player::update(void)
{
  render::Timer &timer = render::Timer::getInstance();
  float damp = _obj->getAngDamping();
  _oldRot *= (float)pow(damp, timer._deltaTime);
} /* End of 'update' function */

/* END OF 'player.cpp' FILE */
