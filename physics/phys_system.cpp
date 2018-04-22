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
    tmp = 2.0 / 3 * Obj->getMass() * pow(*((float *)Params), 2);
    tensor.setDiag(tmp);
    Obj->setInertiaTensor(tensor);
    _detector.addVolume(Name, new BoundingSphere(Obj, *((float *)Params)));
    break;
  case bounding_volume_type::BOX:
    _detector.addVolume(Name, new BoundingBox(Obj, *((const math::Vec3f *)Params)));
    break;
  case bounding_volume_type::PLANE:
    struct dummy
    {
      math::Vec3f _normal;
      float _offset;
    } *params;
    params = (dummy *)Params;
    _detector.addVolume(Name, new BoundingPlane(Obj, params->_normal, params->_offset));
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

/* END OF 'phys_system.cpp' FILE */
