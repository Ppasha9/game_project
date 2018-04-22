/*
 * Game project
 *
 * FILE: bounding_plane.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: plane bounding volume definition file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingPlane::BoundingPlane(PhysObject *Obj, const math::Vec3f &Normal, const float Offset) : BoundingVolume(Obj, bounding_volume_type::PLANE), _normal(Normal.getNormalized()), _offset(Offset)
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

/* Virtual function of colliding to bounding volumes */
bool BoundingPlane::isCollide(const BoundingVolume *Volume) const
{
  if (!_body->hasFiniteMass())
    return false;

  if (Volume->_type == bounding_volume_type::SPHERE)
    return isCollide((BoundingSphere *)Volume);
  else if (Volume->_type == bounding_volume_type::BOX)
    return isCollide((BoundingBox *)Volume);
  else if (Volume->_type == bounding_volume_type::PLANE)
    return isCollide((BoundingPlane *)Volume);
} /* End of 'isCollide' function */

/* Collide plane with sphere function */
bool BoundingPlane::isCollide(const BoundingSphere *Sph) const
{
  // Cache the sphere position.
  math::Vec3f position = Sph->getPos();
  // Find the distance from the plane.
  float centerDistance = _normal * position - _offset;

  // Check if we’re within radius.
  if (centerDistance * centerDistance > Sph->getRadius() * Sph->getRadius())
    return false;
  return true;
} /* End of 'isCollide' function */

/* Colliding plane with box function */
bool BoundingPlane::isCollide(const BoundingBox *Box) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding plane with box function */
bool BoundingPlane::isCollide(const BoundingPlane *Plane) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with sphere function */
std::vector<Contact> BoundingPlane::getContactData(const BoundingSphere *Sph) const
{
  // Cache the sphere position.
  math::Vec3f position = Sph->getPos();
  // Find the distance from the plane.
  float centerDistance = _normal * position - _offset;
  // Check which side of the plane we’re on.
  math::Vec3f normal = _normal;
  float penetration = -centerDistance;
  if (centerDistance < 0)
  {
    normal *= -1;
    penetration = -penetration;
  }
  penetration += Sph->getRadius();

  // Create the contact - it has a normal in the plane direction.
  Contact contact;
  contact._normal = normal;
  contact._penetration = penetration;
  contact._position = position - _normal * centerDistance;
  contact.calculateContactBasis();

  std::vector<Contact> contactVec;
  contactVec.push_back(contact);
  return contactVec;
} /* End of 'getContactData' function */

/* Colliding sphere with plane function */
std::vector<Contact> BoundingPlane::getContactData(const BoundingPlane *Plane) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Colliding sphere with box function */
std::vector<Contact> BoundingPlane::getContactData(const BoundingBox *Box) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Virtual function of getting the colliding data */
std::vector<Contact> BoundingPlane::getContactData(const BoundingVolume *Volume) const
{
  if (Volume->_type == bounding_volume_type::SPHERE)
    return getContactData((BoundingSphere *)Volume);
  else if (Volume->_type == bounding_volume_type::BOX)
    return getContactData((BoundingBox *)Volume);
  else if (Volume->_type == bounding_volume_type::PLANE)
    return getContactData((BoundingPlane *)Volume);
} /* End of 'getContactData' function */

/* END OF 'bounding_plane.cpp' FILE */
