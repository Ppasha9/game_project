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

/* Does the pairs are equivalent or not */
bool ContactGenerator::isEqual(const PhysicsObjectsPair &FPair, const PhysicsObjectsPair &SPair) const
{
  return (FPair.first == SPair.first && FPair.second == SPair.second) ||
    (FPair.second == SPair.first && FPair.first == SPair.second);
} /* End of 'isEqual' function */

/* Does the vector contain certain pair */
bool ContactGenerator::contain(const ObjectContactsVector &Vector, const ObjectContactsPair &Pair) const
{
  for (auto elem : Vector)
    if (isEqual(elem.second, Pair.second) || isEqual(elem.second, PhysicsObjectsPair(Pair.second.second, Pair.second.first)))
      return true;

  return false;
} /* End of 'contain' function */

/* Response function */
ObjectContactsVector ContactGenerator::response(const CollidingObjectsVector &ObjectsVector) const
{
  ObjectContactsVector vector;

  for (auto &it : ObjectsVector)
  {
    ObjectContactsPair objCPair = ObjectContactsPair(it.first->getContactData(it.second), PhysicsObjectsPair(it.first, it.second));
    if (!contain(vector, objCPair))
      vector.push_back(objCPair);
  }

  return vector;
} /* End of 'response' function */

/* END OF 'collision_generator.cpp' FILE */
