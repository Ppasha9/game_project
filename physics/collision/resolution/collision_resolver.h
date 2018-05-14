/*
 * Game project
 *
 * FILE: collision_resolver.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: resolving the collisions declaration file
 */

#ifndef _COLLISION_RESOLVER_H_INCLUDED__
#define _COLLISION_RESOLVER_H_INCLUDED__
#pragma once

#include "../generation/collision_generator.h"

/* Physics namespace */
namespace phys
{
  /* Collisions resolver function */
  class ContactResolver
  {
  private:
    /* Resolve the pair of objects function */
    void resolve(PhysicsObjectsPair &Pair, Contact Contact, float DampingCoeff) const;

    /* Adding the change of velocity by impulse function */
    void addChange(PhysicsObjectsPair &Pair, const Contact &Contact, const float DeltaVel, math::Vec3f RelContactPos[2], float DampingCoeff) const;

  public:
    /* Default class constructor */
    ContactResolver(void) = default;

    /* Response function */
    void response(ObjectContactsVector &ObjContacts) const;
  }; /* End of 'ContactResolver' class */
}; /* End of 'phys' namespace */

#endif /* _COLLISION_RESOLVER_H_INCLUDED__ */

/* END OF 'collision_resolver.h' FILE */
