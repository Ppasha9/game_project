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
  /* Contact data class */
  class Contact
  {
  public:
    /* Position of the contact */
    math::Vec3f _position;
    /* Normal of point of the contact */
    math::Vec3f _normal;
    /* Value of penetration */
    float _penetration;

    /* Default class constructor */
    Contact(void) = default;

    /* Class constructor */
    Contact(const math::Vec3f &Pos, const math::Vec3f &Normal, const float Penetration);
  }; /* End of 'Contact' class */

  /* Physics objects pair */
  using PhysicsObjectsPair = std::pair<PhysObject &, PhysObject &>;
  /* Result of contact generator response function */
  using ObjectContactsVector = std::vector<std::pair<std::vector<Contact>, PhysicsObjectsPair>>;

  /* Contact generator class */
  class ContactGenerator
  {
  public:
    /* Default class constructor */
    ContactGenerator(void) = default;

    /* Response function */
    ObjectContactsVector response(const CollidingObjectsVector &objectsVector) const;
  }; /* End of 'ContactGenerator' class */
}; /* End of 'phys' namespace */

#endif /* _COLLISION_GENERATOR_H_INCLUDED__ */

/* END OF 'collision_generator.h' FILE */
