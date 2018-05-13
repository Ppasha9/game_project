/*
 * Game project
 *
 * FILE: collision_resolver.ñpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 02.05.2018
 * NOTE: resolving the collisions definition file
 */

#include "collision_resolver.h"

using namespace phys;

/* Adding the change of velocity by impulse function */
void ContactResolver::addChange(PhysicsObjectsPair &Pair, const Contact &Contact, const float DeltaVel, const math::Vec3f RelContactPos[2], float DampingCoeff) const
{
  PhysObject *FObj = Pair.first->getPhysObjectPointer();
  PhysObject *SObj = Pair.second->getPhysObjectPointer();

  math::Vec3f velocity = FObj->getRotation() & RelContactPos[0];
  velocity += FObj->getVelocity();

  math::Vec3f contactVelocity = Contact._basisMatrix.getTranspose() * velocity;

  // TODO: add restitution coefficient to contact!
  float desiredDeltaVelocity = -contactVelocity[0] * (1 + 0.85F);
  if (fabs(desiredDeltaVelocity) < math::Threshold / 100.0F)
    desiredDeltaVelocity = 0;

  // Calculate the required size of the impulse.
  math::Vec3f impulseContact;
  impulseContact._coords[0] = desiredDeltaVelocity / DeltaVel;
  impulseContact._coords[1] = 0;
  impulseContact._coords[2] = 0;

  // Convert impulse to world coordinates.
  math::Vec3f impulse = Contact._basisMatrix * impulseContact;

  // Calculate change of linear velocity vector
  math::Vec3f velocityChange = impulse * FObj->getInverseMass();
  // Calculate change of angular velocity vector
  math::Vec3f impulsiveTorque = impulse & RelContactPos[0];
  math::Vec3f rotationChange = FObj->getIITWorld() * impulsiveTorque;

  // Add change to first object
  FObj->addVelocity(velocityChange * DampingCoeff);
  //FObj->addRotation(rotationChange * DampingCoeff);

  // Resolving the penetration and friction
  if (FObj->hasFiniteMass())
  {
    //FObj->addTorque(FObj->getIITWorld().getInverse() * rotationChange);
    FObj->addPosition(Contact._normal * Contact._penetration);
  }

  /* The same for the second object */

  if (!SObj->hasFiniteMass())
    return;

  impulse *= -1;
  // Calculate change of linear velocity vector
  velocityChange = impulse * SObj->getInverseMass();
  // Calculate change of angular velocity vector
  impulsiveTorque = impulse & RelContactPos[1];
  rotationChange = SObj->getIITWorld() * impulsiveTorque;

  // Add change to first object
  SObj->addVelocity(velocityChange * DampingCoeff);
  //SObj->addRotation(rotationChange * DampingCoeff);

  // Resolving the penetration and friction
  //SObj->addTorque(SObj->getIITWorld().getInverse() * rotationChange);
  SObj->addPosition(-Contact._normal * Contact._penetration);
} /* End of 'addChange' function */

/* Resolve the pair of objects function */
void ContactResolver::resolve(PhysicsObjectsPair &Pair, Contact Contact, float DampingCoeff) const
{
  PhysObject *FObj = Pair.first->getPhysObjectPointer();
  PhysObject *SObj = Pair.second->getPhysObjectPointer();

  math::Vec3f relativeContactPosition[2] = { Contact._position - FObj->getPos(), Contact._position - SObj->getPos() };
  math::Matr3f inverseInertiaTensor[2] = { FObj->getIITWorld(), SObj->getIITWorld() };

  // Build a vector that shows the change in velocity in
  // world space for a unit impulse in the direction of the contact
  // normal.

  math::Vec3f deltaVelWorld = relativeContactPosition[0] & Contact._normal;
  deltaVelWorld = inverseInertiaTensor[0] * deltaVelWorld;
  deltaVelWorld = deltaVelWorld & relativeContactPosition[0];

  // Work out the change in velocity in contact coordinates.
  float deltaVelocity = deltaVelWorld * Contact._normal;
  // Add the linear component of velocity change.
  deltaVelocity += FObj->getInverseMass();

  if (SObj->hasFiniteMass())
  {
    // Go through the same transformation sequence again.
    deltaVelWorld = relativeContactPosition[1] & Contact._normal;
    deltaVelWorld = inverseInertiaTensor[1] * deltaVelWorld;
    deltaVelWorld = deltaVelWorld & relativeContactPosition[1];

    // Add the change in velocity due to rotation.
    deltaVelocity += deltaVelWorld * Contact._normal;
    // Add the change in velocity due to linear motion.
    deltaVelocity += SObj->getInverseMass();
  }

  // Adding change velocity to objects
  addChange(Pair, Contact, deltaVelocity, relativeContactPosition, DampingCoeff);
} /* End of 'resolve' function */

/* Response function */
void ContactResolver::response(ObjectContactsVector &ObjContacts) const
{
  for (auto &elem : ObjContacts)
  {
    float damp = 1.0F;
    for (unsigned int i(0); i < elem.first.size(); i++)
      resolve(elem.second, elem.first[i], damp);
  }
} /* End of 'response' function */

/* END OF 'collision_resolver.ñpp' FILE */
