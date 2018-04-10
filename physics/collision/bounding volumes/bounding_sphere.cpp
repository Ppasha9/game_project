/*
 * Game project
 *
 * FILE: bounding_sphere.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: sphere bounding volume declaration file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingSphere::BoundingSphere(PhysObject &Obj, const float Radius) : BoundingVolume(Obj), _radius(Radius)
{
} /* End of constructor */

/* Getting radius of the sphere function */
float BoundingSphere::getRadius(void) const
{
  return _radius;
} /* End of 'getRadius' function */

/* Function of colliding sphere and another shape */
bool BoundingSphere::isCollide(const BoundingSphere *Sph) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with plane function */
bool BoundingSphere::isCollide(const BoundingPlane *Plane) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with box function */
bool BoundingSphere::isCollide(const BoundingBox *Box) const
{
  return false;
} /* End of 'isCollide' function */

/* END OF 'bounding_sphere.cpp' FILE */
