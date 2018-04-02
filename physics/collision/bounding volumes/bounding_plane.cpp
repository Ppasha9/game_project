/*
 * Game project
 *
 * FILE: bounding_plane.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: plane bounding volume definition file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingPlane::BoundingPlane(PhysObject &Obj, const math::Vec3f &Normal, const float Offset) : BoundingVolume(Obj), _normal(Normal), _offset(Offset)
{
} /* End of constructor */

/* Getting plane normal function */
math::Vec3f BoundingPlane::getNormal(void) const
{
  return _normal;
} /* End of 'getNormal' function */

/* Getting plane offset function */
float BoundingPlane::getOffset(void) const
{
  return _offset;
} /* End of 'getOffset' function */

/* Collide plane with sphere function */
bool BoundingPlane::isCollide(const BoundingSphere *Sph) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding plane with box function */
bool BoundingPlane::isCollide(const BoundingBox *Box) const
{
  return false;
} /* End of 'isCollide' function */

/* END OF 'bounding_plane.cpp' FILE */
