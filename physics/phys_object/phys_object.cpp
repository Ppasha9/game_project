/*
 * Game project
 *
 * FILE: phys_object.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 27.03.2018
 * NOTE: physics object handle definition file
 */

#include <exception>
#include "phys_object.h"

using namespace phys;

/*
 * Inline function that creates the transform matrix
 * from position and orientation.
 */
static inline void s_calculateTransformMatrix(math::Matr4f &TransformMatr, const math::Vec3f &Pos, const math::Quatf &Orientation)
{
  TransformMatr._values[0][0] = 1 - 2 * Orientation._j * Orientation._j - 2 * Orientation._k * Orientation._k;
  TransformMatr._values[0][1] = 2 * Orientation._i * Orientation._j - 2 * Orientation._r * Orientation._k;
  TransformMatr._values[0][2] = 2 * Orientation._i * Orientation._k + 2 * Orientation._r * Orientation._j;
  TransformMatr._values[0][3] = Pos[0];
  TransformMatr._values[1][0] = 2 * Orientation._i * Orientation._j + 2 * Orientation._r * Orientation._k;
  TransformMatr._values[1][1] = 1 - 2 * Orientation._i * Orientation._i - 2 * Orientation._k * Orientation._k;
  TransformMatr._values[1][2] = 2 * Orientation._j * Orientation._k - 2 * Orientation._r * Orientation._i;
  TransformMatr._values[1][3] = Pos[1];
  TransformMatr._values[2][0] = 2 * Orientation._i * Orientation._k - 2 * Orientation._r * Orientation._j;
  TransformMatr._values[2][1] = 2 * Orientation._j * Orientation._k + 2 * Orientation._r * Orientation._i;
  TransformMatr._values[2][2] = 1 - 2 * Orientation._i * Orientation._i - 2 * Orientation._j * Orientation._j;
  TransformMatr._values[2][3] = Pos[2];
} /* End of 's_calculateTransformMatrix' function */

/* Internal function to do an inertia tensor transform by a quaternion. */
static inline void s_transformInertiaTensor(math::Matr3f &IITWorld, const math::Matr3f &IITBody, const math::Matr4f &RotMat)
{
  float t4 = RotMat._values[0][0] * IITBody._values[0][0] + RotMat._values[0][1] * IITBody._values[1][0] + RotMat._values[0][2] * IITBody._values[2][0];
  float t9 = RotMat._values[0][0] * IITBody._values[0][1] + RotMat._values[0][1] * IITBody._values[1][1] + RotMat._values[0][2] * IITBody._values[2][1];
  float t14 = RotMat._values[0][0] * IITBody._values[0][2] + RotMat._values[0][1] * IITBody._values[1][2] + RotMat._values[0][2] * IITBody._values[2][2];
  float t28 = RotMat._values[1][0] * IITBody._values[0][0] + RotMat._values[1][1] * IITBody._values[1][0] + RotMat._values[1][2] * IITBody._values[2][0];
  float t33 = RotMat._values[1][0] * IITBody._values[0][1] + RotMat._values[1][1] * IITBody._values[1][1] + RotMat._values[1][2] * IITBody._values[2][1];
  float t38 = RotMat._values[1][0] * IITBody._values[0][2] + RotMat._values[1][1] * IITBody._values[1][2] + RotMat._values[1][2] * IITBody._values[2][2];
  float t52 = RotMat._values[2][0] * IITBody._values[0][0] + RotMat._values[2][1] * IITBody._values[1][0] + RotMat._values[2][2] * IITBody._values[2][0];
  float t57 = RotMat._values[2][0] * IITBody._values[0][1] + RotMat._values[2][1] * IITBody._values[1][1] + RotMat._values[2][2] * IITBody._values[2][1];
  float t62 = RotMat._values[2][0] * IITBody._values[0][2] + RotMat._values[2][1] * IITBody._values[1][2] + RotMat._values[2][2] * IITBody._values[2][2];

  IITWorld._values[0][0] = t4 * RotMat._values[0][0] + t9 * RotMat._values[0][1] + t14 * RotMat._values[0][2];
  IITWorld._values[0][1] = t4 * RotMat._values[1][0] + t9 * RotMat._values[1][1] + t14 * RotMat._values[1][2];
  IITWorld._values[0][2] = t4 * RotMat._values[2][0] + t9 * RotMat._values[2][1] + t14 * RotMat._values[2][2];
  IITWorld._values[1][0] = t28 * RotMat._values[0][0] + t33 * RotMat._values[0][1] + t38 * RotMat._values[0][2];
  IITWorld._values[1][1] = t28 * RotMat._values[1][0] + t33 * RotMat._values[1][1] + t38 * RotMat._values[1][2];
  IITWorld._values[1][2] = t28 * RotMat._values[2][0] + t33 * RotMat._values[2][1] + t38 * RotMat._values[2][2];
  IITWorld._values[2][0] = t52 * RotMat._values[0][0] + t57 * RotMat._values[0][1] + t62 * RotMat._values[0][2];
  IITWorld._values[2][1] = t52 * RotMat._values[1][0] + t57 * RotMat._values[1][1] + t62 * RotMat._values[1][2];
  IITWorld._values[2][2] = t52 * RotMat._values[2][0] + t57 * RotMat._values[2][1] + t62 * RotMat._values[2][2];
} /* End of 's_transformInertiaTensor' function */

/* Class constructor */
PhysObject::PhysObject(int Id, const math::Vec3f &Pos, const float InverseMass, const float LinDamping, const float AngDamping) : _position(Pos), _inverseMass(InverseMass),
  _orientation(), _velocity(0), _acceleration(0), _rotation(0), _forceAccum(0), _torqueAccum(0), _angularDamping(AngDamping), _linearDamping(LinDamping), _id(Id)
{
} /* End of constructor */

/* Calculating internal data from state data function */
void PhysObject::calculateDerivedData(void)
{
  /* Calculate the transform matrix */
  s_calculateTransformMatrix(_transformMatrix, _position, _orientation);
  /* Calculate inverse inertia tensor in world space */
  s_transformInertiaTensor(_iitWorld, _inverseInertiaTensor, _transformMatrix);
} /* End of 'calculateDerivedData' function */

/* Setting inverse inertia tensor function */
void PhysObject::setInertiaTensor(const math::Matr3f &InertiaTensor)
{
  _inverseInertiaTensor = math::Matr3f(InertiaTensor.getInverse());
} /* End of 'setInertiaTensor' function */

/* Adding force to center mass of object function */
void PhysObject::addForce(const math::Vec3f &Force)
{
  _forceAccum += Force;
} /* End of 'addForce' function */

/* Adding force to a point of object function */
void PhysObject::addForceAtPoint(const math::Vec3f &Force, const math::Vec3f &Point)
{
  addTorque(Point & Force);
  addForce(Force);
} /* End of 'addForceAtPoint' function */

/* Getting point of body in world space function */
math::Vec3f PhysObject::getPointInWorldSpace(const math::Vec3f &Point) const
{
  math::Vec4f point4f = { Point[0], Point[1], Point[2], 1.0f };
  math::Vec4f res = _transformMatrix * point4f;
  math::Vec3f r = { res[0], res[1], res[2] };
  return r;
} /* End of 'getPointInWorldSpace' function */

/*
 * Adds the given force to the given point on the rigid body.
 * The direction of the force is given in world coordinates,
 * but the application point is given in body space. This is
 * useful for spring forces, or other forces fixed to the body.
 */
void PhysObject::addForceAtBodyPoint(const math::Vec3f &Force, const math::Vec3f &Point)
{
  math::Vec3f pt = getPointInWorldSpace(Point);
  addForceAtPoint(Force, pt);
} /* End of 'addForceAtBodyPoint' function */

/* Adding torque to body function. */
void PhysObject::addTorque(const math::Vec3f &Torque)
{
  _torqueAccum += Torque;
} /* End of 'addTorque' function */

/* Clear accumulators function */
void PhysObject::clearAccums(void)
{
  _torqueAccum = _forceAccum = 0;
} /* End of 'clearAccums' function */

/* Does the object have finite mass or not */
bool PhysObject::hasFiniteMass(void) const
{
  return _inverseMass != 0;
} /* End of 'hasFiniteMass' function */

/* Getting object mass function */
float PhysObject::getMass(void) const
{
  return (1.0f / _inverseMass);
} /* End of 'getMass' function */

/* Getting object inverse mass function */
float PhysObject::getInverseMass(void) const
{
  return _inverseMass;
} /* End of 'getInverseMass' function */

/* Setting linear damping function */
void PhysObject::setLinearDamping(const float LinearDamping)
{
  _linearDamping = LinearDamping;
} /* End of 'setLinearDamping' function */

/* Setting angular damping function */
void PhysObject::setAngularDamping(const float AngularDamping)
{
  _angularDamping = AngularDamping;
} /* End of 'setAngularDamping' function */

/* Setting object inverse mass function */
/* NOTE: it can throw an exception */
void PhysObject::setInverseMass(const float InverseMass)
{
  if (InverseMass < 0)
    throw std::exception("InverseMass must be positive number!");

  _inverseMass = InverseMass;
} /* End of 'setMass' function */

/* Getting transformation matrix of object for rendering function */
math::Matr4f PhysObject::getTransormMatrix(void) const
{
  return _transformMatrix;
} /* End of 'getTransormMatrix' function */

/* Getting object id function */
int PhysObject::getId(void) const
{
  return _id;
} /* End of 'getId' function */

/* Integration function */
void PhysObject::integrate(float Duration)
{
  // Calculate linear acceleration from force inputs.
  _acceleration += _forceAccum * _inverseMass;

  // Calculate angular acceleration from torque inputs.
  math::Vec3f angularAcceleration = _iitWorld * _torqueAccum;

  // Adjust velocities
  // Update linear velocity from both acceleration and impulse.
  _velocity += _acceleration * Duration;

  // Update angular velocity from both acceleration and impulse.
  _rotation += angularAcceleration * Duration;

  // Impose drag.
  _velocity *= pow(_linearDamping, Duration);
  _rotation *= pow(_angularDamping, Duration);

  // Adjust positions
  // Update linear position.
  _position += _velocity * Duration;

  // Update angular position.
  _orientation.addScaledVector(_rotation, Duration);

  // Impose drag.
  //_velocity *= pow(_linearDamping, Duration);
  //_rotation *= pow(_angularDamping, Duration);

  // Normalize the orientation, and update the matrices with the new position and orientation.
  calculateDerivedData();

  // Clear accumulators.
  clearAccums();
} /* End of 'integrate' function */

/* END OF 'phys_object.cpp' FILE */
