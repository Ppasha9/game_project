/*
 * Game project
 *
 * FILE: phys_system.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: physics system simulation definition file
 */

#include "phys_system.h"
#include "collision\bounding volumes\bounding_volume_def.h"

using namespace phys;

/* Getting instance to class function */
PhysicsSystem & PhysicsSystem::getInstance(void)
{
  static PhysicsSystem instance;
  return instance;
} /* End of 'getInstance' function */

/* Register new object function */
void PhysicsSystem::registerObject(const std::string &Name, const math::Vec3f &Pos, const float InverseMass, const float LinDamping,
  const float AngDamping, const bounding_volume_type VolumeType, void *Params)
{
  PhysObject *obj = new PhysObject(Pos, InverseMass, LinDamping, AngDamping);
  registerObject(Name, obj, VolumeType, Params);
} /* End of 'registerObject' function */

/* Register new object function */
void PhysicsSystem::registerObject(const std::string &Name, PhysObject *Obj, const bounding_volume_type VolumeType, void *Params)
{
  float tmp;
  math::Matr3f tensor;

  switch (VolumeType)
  {
  case bounding_volume_type::SPHERE:
    tmp = 2.0F / 5 * Obj->getMass() * pow(*((float *)Params), 2);
    tensor.setDiag(tmp);
    Obj->setInertiaTensor(tensor);
    _detector.addVolume(Name, new BoundingSphere(Obj, *((float *)Params), Name));
    break;
  case bounding_volume_type::BOX:
    struct dummyBox
    {
      math::Vec3f _dirVec;
      math::Vec3f _rightVec;
      float _halfHeight;
    } *paramsBox;
    paramsBox = (dummyBox *)Params;

    tensor = { 1.0F / 12 * (pow(2 * paramsBox->_rightVec.length(), 2) + pow(2 * paramsBox->_halfHeight, 2)) * Obj->getMass(), 0, 0,
               0, 1.0F / 12 * (pow(2 * paramsBox->_dirVec.length(), 2) + pow(2 * paramsBox->_halfHeight, 2)) * Obj->getMass(), 0,
               0, 0, 1.0F / 12 * (pow(2 * paramsBox->_dirVec.length(), 2) + pow(2 * paramsBox->_rightVec.length(), 2)) * Obj->getMass() };

    Obj->setInertiaTensor(tensor);
    _detector.addVolume(Name, new BoundingBox(Obj, paramsBox->_dirVec, paramsBox->_rightVec, paramsBox->_halfHeight, Name));
    break;
  case bounding_volume_type::PLANE:
    struct dummyPlane
    {
      math::Vec3f _normal;
      math::Vec3f _fPoint;
      math::Vec3f _sPoint;
    } *paramsPlane;
    paramsPlane = (dummyPlane *)Params;
    _detector.addVolume(Name, new BoundingPlane(Obj, paramsPlane->_normal, paramsPlane->_fPoint, paramsPlane->_sPoint, Name));
    break;
  }
} /* End of 'registerObject' function */

/* Response function */
void PhysicsSystem::response(void)
{
  render::Timer &timer = render::Timer::getInstance();
  if (timer._deltaTime > render::Timer::MAX_ENABLED_DELTA_TIME)
    return;

  _resolver.response(_generator.response(_detector.response()));
  _detector.integrate();
} /* End of 'response' function */

/* Getting physics object transformation matrix for rendering function */
const math::Matr4f PhysicsSystem::getObjectMatrix(const std::string &Name) const
{
  return _detector.getObjectMatrix(Name);
} /* End of 'getObjectMatrix' function */

/* Apply force function */
void PhysicsSystem::applyForceToObj(const std::string &ObjName, const Force *Force)
{
  _detector.applyForceToObj(ObjName, Force);
} /* End of 'applyForceToObj' function */

/* Class destructor */
PhysicsSystem::~PhysicsSystem(void)
{
} /* End of destructor */

/* Getting the pointer to physics object function */
PhysObject * PhysicsSystem::getObject(const std::string &Name)
{
  return _detector.getObject(Name);
} /* End of 'getObject' function */

/* Drawing debug primitives function */
void PhysicsSystem::debugDraw(void) const
{
  _detector.debugDraw();
} /* End of 'debugDraw' function */

/* Removing physic object function */
void PhysicsSystem::removeObject(const std::string &Name)
{
  _detector.remove(Name);
} /* End of 'removeObject' function */

/* END OF 'phys_system.cpp' FILE */
