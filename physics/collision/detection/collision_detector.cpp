/*
 * Game project
 *
 * FILE: collision_detector.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: detection system definition file
 */

#include "collision_detector.h"

using namespace phys;

/* Adding new bounding volume function */
void ContactDetector::addVolume(BoundingVolume *Volume)
{
} /* End of 'addVolume' function */

/* Response function */
CollidingObjectsVector ContactDetector::response(void) const
{
  return CollidingObjectsVector();
} /* End of 'response' function */

/* Class destructor */
ContactDetector::~ContactDetector(void)
{
} /* End of destructor */

/* END OF 'collision_detector.cpp' FILE */
