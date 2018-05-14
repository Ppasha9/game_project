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
#include "../../render/timer/timer.h"
using namespace scene;

/* Impulse coefficient */
const float Player::ImpulseCoeff = 5.0f;
/* Rotation coefficient */
const float Player::RotationCoeff = 1.0f;
// Jump coeficent

const float Player::Radius = 3;
const float Player::WMaxTime = 0.30;

/* Class constructor */
Player::Player(const render::PrimPtr &Prim, phys::PhysObject *Obj, const math::Vec3f &DirVec,
               const std::string &Name, const moveMap &Moves, const math::Matr4f &StartOri) :
  _obj(Obj), _dirVec(DirVec), _prim(Prim), _name(Name), _upVec({ 0, 1, 0 }), _oldRot({0, 0, 0}), _curW(0),
  _moves(Moves), _kickLastTime(render::Timer::getInstance()._time), _jumpLastTime(render::Timer::getInstance()._time), _startOri(StartOri)
{
} /* End of constructor */

/* Class destructor */
Player::~Player(void)
{
  phys::PhysicsSystem::getInstance().removeObject(_name);
  delete _obj;
} /* End of destructor */

/* Action function */
int Player::action(const COMMAND_TYPE ComType)
{
  double curTime = render::Timer::getInstance()._time;
  float deltaTime = (float)render::Timer::getInstance()._deltaTime;

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
    if (_curW < Player::RotationCoeff)
      _curW += Player::RotationCoeff * deltaTime / WMaxTime;
    else
      _curW = Player::RotationCoeff;
    _obj->addRotation(-_oldRot);
    _obj->addRotation(upV3 * _curW);
    _oldRot = upV3 * _curW;
    break;
  case COMMAND_TYPE::MoveRight:
    if (_curW > -Player::RotationCoeff)
      _curW -= Player::RotationCoeff * deltaTime / WMaxTime;
    else
      _curW = -Player::RotationCoeff;
    _obj->addRotation(-_oldRot);
    _obj->addRotation(upV3 * _curW);
    _oldRot = upV3 * _curW;
    break;
  case COMMAND_TYPE::MoveJump:
    if (curTime - _jumpLastTime >= JUMP_DELTA_TIME)
    {
      _obj->addImpulse(_upVec * JUMP_COEF);
      _jumpLastTime = curTime;
    }
    break;
  case COMMAND_TYPE::MoveKick:
      if (curTime - _kickLastTime >= KICK_DELTA_TIME)
      {
        _kickLastTime = curTime;
        return 0;
      }
      break;
  }
  return -1;
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

  rnd.drawPrim(_prim, math::Matr4f().getScale({2 * Radius, 2 * Radius, 2 * Radius, 1}) *
    math::Matr4f(_startOri) * physSys.getObjectMatrix(_name));
} /* End of 'draw' function */

/* Updating function */
void Player::update(void)
{
  render::Timer &timer = render::Timer::getInstance();
  float damp = _obj->getAngDamping();
  _oldRot *= (float)pow(damp, 2 * timer._deltaTime);
  _curW *= (float)pow(damp, 2 * timer._deltaTime); 
} /* End of 'update' function */

void scene::Player::SetCamera(UINT Id)
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

  render::Render &rnd = render::Render::getInstance();
  Vec3f pos = GetPos();
  rnd.setCamera(Id, false, pos + (upV3 * 3.6f - dirV3 * 11) * 5.0f, dirV3 - upV3 * 0.2f, upV3);
} /* End of 'scene::Player::SetCamera' function */

int scene::Player::MoveKeyboard(const std::vector<UINT>& Moves)
{
  int res = -1;
  for (auto &it = Moves.begin(); it != Moves.end(); it++)
  {
    auto move = _moves.find(*it);
    if (move != _moves.end())
    {
      int t = action(move->second);
      if (t != -1)
        res = t;
    }
  }
  return res;
} /* End of 'scene::Player::MoveKeyboard' function */

/* END OF 'player.cpp' FILE */
