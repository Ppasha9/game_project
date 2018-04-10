/*
 * Game project
 *
 * FILE: collision_generator.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: generation the contact data definition file
 */

#include "collision_generator.h"

using namespace phys;

/* Class constructor */
Contact::Contact(const math::Vec3f &Pos, const math::Vec3f &Normal, const float Penetration) : _position(Pos), _normal(Normal), _penetration(Penetration)
{
} /* End of constructor */

/* Response function */
ObjectContactsVector ContactGenerator::response(const CollidingObjectsVector &objectsVector) const
{
  return ObjectContactsVector();
} /* End of 'response' function */

/* END OF 'collision_generator.cpp' FILE */
