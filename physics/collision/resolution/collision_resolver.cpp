/*
 * Game project
 *
 * FILE: collision_resolver.ñpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: resolving the collisions definition file
 */

#include "collision_resolver.h"

using namespace phys;

/* Adding the change of velocity by impulse function */
void ContactResolver::addChange(PhysicsObjectsPair &Pair, const Contact &Contact, const float DeltaVel, const math::Vec3f RelContactPos[2]) const
{
  PhysObject *FObj = Pair.first->getPhysObjectPointer();
  PhysObject *SObj = Pair.second->getPhysObjectPointer();

  math::Vec3f velocity = FObj->getRotation() & RelContactPos[0];
  velocity += FObj->getVelocity();

  math::Vec3f contactVelocity = Contact._basisMatrix.getTranspose() * velocity;

  // TODO: add restitution coefficient to contact!
  float desiredDeltaVelocity = -contactVelocity[0] * (1 + 0.85F);
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
  math::Vec3f rotationChange = FObj->getInverseInertia() * impulsiveTorque;

  // Add change to first object
  FObj->addVelocity(velocityChange);
  FObj->addRotation(rotationChange);
  FObj->addTorque(FObj->getInverseInertia().getInverse() * rotationChange);

  // Resolving the penetration
  //while (Pair.first->isCollide(Pair.second))
  if (FObj->hasFiniteMass())
    FObj->addPosition(Contact._normal * Contact._penetration);

  /* The same for the second object */

  impulse *= -1;
  // Calculate change of linear velocity vector
  velocityChange = impulse * SObj->getInverseMass();
  // Calculate change of angular velocity vector
  impulsiveTorque = impulse & RelContactPos[1];
  rotationChange = SObj->getInverseInertia() * impulsiveTorque;

  // Add change to first object
  SObj->addVelocity(velocityChange);
  SObj->addRotation(rotationChange);
  SObj->addTorque(SObj->getInverseInertia().getInverse() * rotationChange);

  if (SObj->hasFiniteMass())
    SObj->addPosition(-Contact._normal * Contact._penetration);
} /* End of 'addChange' function */

/* Resolve the pair of objects function */
void ContactResolver::resolve(ObjectContactsPair &Pair) const
{
#if 1
  Contact contact = Pair.first[0];
  PhysicsObjectsPair physPair = Pair.second;
  PhysObject *FObj = physPair.first->getPhysObjectPointer();
  PhysObject *SObj = physPair.second->getPhysObjectPointer();

  math::Vec3f relativeContactPosition[2] = { contact._position - FObj->getPos(), contact._position - SObj->getPos() };
  math::Matr3f inverseInertiaTensor[2] = { FObj->getInverseInertia(), SObj->getInverseInertia() };

  // Build a vector that shows the change in velocity in
  // world space for a unit impulse in the direction of the contact
  // normal.

  math::Vec3f deltaVelWorld = relativeContactPosition[0] & contact._normal;
  deltaVelWorld = inverseInertiaTensor[0] * deltaVelWorld;
  deltaVelWorld = deltaVelWorld & relativeContactPosition[0];

  // Work out the change in velocity in contact coordinates.
  float deltaVelocity = deltaVelWorld * contact._normal;
  // Add the linear component of velocity change.
  deltaVelocity += FObj->getInverseMass();

  if (SObj->hasFiniteMass())
  {
    // Go through the same transformation sequence again.
    deltaVelWorld = relativeContactPosition[1] & contact._normal;
    deltaVelWorld = inverseInertiaTensor[1] * deltaVelWorld;
    deltaVelWorld = deltaVelWorld & relativeContactPosition[1];

    // Add the change in velocity due to rotation.
    deltaVelocity += deltaVelWorld * contact._normal;
    // Add the change in velocity due to linear motion.
    deltaVelocity += SObj->getInverseMass();
  }

  // Adding change velocity to objects
  addChange(physPair, contact, deltaVelocity, relativeContactPosition);
#endif

#if 0
  render::Timer &timer = render::Timer::getInstance();
  Contact contact = Pair.first[0];
  PhysicsObjectsPair physPair = Pair.second;
  PhysObject *FObj = physPair.first->getPhysObjectPointer();
  PhysObject *SObj = physPair.second->getPhysObjectPointer();

  float m1 = FObj->getInverseMass(), m2 = SObj->getInverseMass(), D = contact._penetration;
  math::Vec3f
    r1 = contact._position - FObj->getPos(),
    r2 = contact._position - SObj->getPos(),
    n = contact._normal,
    P1, P2;
  math::Matr3f ITen1 = FObj->getIITWorld(), ITen2 = SObj->getIITWorld();

  P1 = FObj->getVelocity() + FObj->getRotation() & (-r1);
  P2 = SObj->getVelocity() + SObj->getRotation() & (-r2);

  math::Vec3f V = n * ((P1 - P2) * n);
  math::Vec3f Vbias = math::Vec3f(0);// n * 0.3 / timer._deltaTime * std::max(0.0, D - 0.1);

  // TODO: 0.6F - restitution coefficient, 0.8F - friction coefficient

  math::Vec3f JNorm = (-V * (1 + 1.0F) + Vbias) / (m1 + m2 + ((ITen1 * ((r1 & n) & r1)) * n) + ((ITen2 * ((r2 & n) & r2)) * n));

  /*** Tangential impuls ***/
  math::Vec3f t = ((P1 - P2) - V).getNormalized();//((P1 - P2) & n) == 0 ? (P1 - P2 - V).Normalizing() : ((Body2->Forces & n) == 0 ? Body2->Forces - n * (Body2->Forces & n) : vec(0));
  float x = ((P1 - P2) * std::min(1 / m1, 1 / m2)) * t;
  math::Vec3f JTan = -t * std::min(x, JNorm.length() * 0.0F);

  math::Vec3f JRes = JNorm + JTan;

  /* Impuls variant */
  FObj->addImpulse(JRes);
  SObj->addImpulse(-JRes);
  FObj->addRotation(ITen1 * (-r1 & JRes));
  SObj->addRotation(ITen2 * (r2 & JRes));
  /* Force variant * /
  Body1->AddForce(taz::phys::FORCE(JRes / DeltaTime, r1));
  Body2->AddForce(taz::phys::FORCE(-JRes / DeltaTime, r2));
  /**/

  FObj->addPosition(-n * D);

#endif
} /* End of 'resolve' function */

/* Response function */
void ContactResolver::response(ObjectContactsVector &ObjContacts) const
{
  for (auto &elem : ObjContacts)
    resolve(elem);
} /* End of 'response' function */

/* END OF 'collision_resolver.ñpp' FILE */
