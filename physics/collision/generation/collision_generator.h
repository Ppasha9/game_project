/*
 * Game project
 *
 * FILE: collision_generator.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: generation the contact data declaration file
 */

#ifndef _COLLISION_GENERATOR_H_INCLUDED__
#define _COLLISION_GENERATOR_H_INCLUDED__
#pragma once

#include "../detection/collision_detector.h"

/* Physics namespace */
namespace phys
{
  /* Physics objects pair */
  using PhysicsObjectsPair = std::pair<BoundingVolume *, BoundingVolume *>;
  /* Object contacts pair */
  using ObjectContactsPair = std::pair<std::vector<Contact>, PhysicsObjectsPair>;
  /* Result of contact generator response function */
  using ObjectContactsVector = std::vector<ObjectContactsPair>;

  /* Contact generator class */
  class ContactGenerator
  {
  private:
    /* Does the pairs are equivalent or not */
    bool isEqual(const PhysicsObjectsPair &FPair, const PhysicsObjectsPair &SPair) const;

    /* Does the vector contain certain pair */
    bool contain(const ObjectContactsVector &Vector, const ObjectContactsPair &Pair) const;

  public:
    /* Default class constructor */
    ContactGenerator(void) = default;

    /* Response function */
    ObjectContactsVector response(const CollidingObjectsVector &ObjectsVector) const;
  }; /* End of 'ContactGenerator' class */
}; /* End of 'phys' namespace */

#endif /* _COLLISION_GENERATOR_H_INCLUDED__ */

/* END OF 'collision_generator.h' FILE */
