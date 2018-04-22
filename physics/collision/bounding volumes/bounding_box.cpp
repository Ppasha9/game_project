/*
 * Game project
 *
 * FILE: bounding_box.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: box bounding volume definition file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingBox::BoundingBox(PhysObject *Obj, const math::Vec3f &HalfSize) : BoundingVolume(Obj, bounding_volume_type::BOX), _halfSize(HalfSize)
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

/* Virtual function of colliding to bounding volumes */
bool BoundingBox::isCollide(const BoundingVolume *Volume) const
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

/* Colliding sphere with sphere function */
std::vector<Contact> BoundingBox::getContactData(const BoundingSphere *Sph) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Colliding sphere with plane function */
std::vector<Contact> BoundingBox::getContactData(const BoundingPlane *Plane) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Colliding sphere with box function */
std::vector<Contact> BoundingBox::getContactData(const BoundingBox *Box) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Virtual function of getting the colliding data */
std::vector<Contact> BoundingBox::getContactData(const BoundingVolume *Volume) const
{
  if (Volume->_type == bounding_volume_type::SPHERE)
    return getContactData((BoundingSphere *)Volume);
  else if (Volume->_type == bounding_volume_type::BOX)
    return getContactData((BoundingBox *)Volume);
  else if (Volume->_type == bounding_volume_type::PLANE)
    return getContactData((BoundingPlane *)Volume);
} /* End of 'getContactData' function */

/* END OF 'bounding_box.cpp' FILE */
