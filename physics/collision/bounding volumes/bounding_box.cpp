/*
 * Game project
 *
 * FILE: bounding_box.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: box bounding volume definition file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingBox::BoundingBox(PhysObject &Obj, const math::Vec3f &HalfSize) : BoundingVolume(Obj), _halfSize(HalfSize)
{
} /* End of constructor */

/* Getting size of box function */
math::Vec3f BoundingBox::getHalfSize(void) const
{
  return _halfSize;
} /* End of 'getHalfSize' function */

/* Setting new size of box function */
void BoundingBox::setHalfSize(const math::Vec3f HalfSize)
{
  _halfSize = HalfSize;
} /* End of 'setHalfSize' function */

/* Colliding box with sphere function */
bool BoundingBox::isCollide(const BoundingSphere *Sph) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding box with box function */
bool BoundingBox::isCollide(const BoundingBox *Box) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding box with plane function */
bool BoundingBox::isCollide(const BoundingPlane *Plane) const
{
  return false;
} /* End of 'isCollide' function */

/* END OF 'bounding_box.cpp' FILE */
