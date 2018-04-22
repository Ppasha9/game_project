/*
 * Game project
 *
 * FILE: bounding_sphere.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: sphere bounding volume declaration file
 */

#include "bounding_volume_def.h"

using namespace phys;

/* Class constructor */
BoundingSphere::BoundingSphere(PhysObject *Obj, const float Radius) : BoundingVolume(Obj), _radius(Radius)
{
  math::Matr3f inertiaTensor(0);

  inertiaTensor._values[0][0] = 2.0F / 5 * _body->getMass() * _radius * _radius;
  inertiaTensor._values[1][1] = inertiaTensor._values[0][0];
  inertiaTensor._values[2][2] = inertiaTensor._values[0][0];

  _body->setInertiaTensor(inertiaTensor);
} /* End of constructor */

/* Getting radius of the sphere function */
float BoundingSphere::getRadius(void) const
{
  return _radius;
} /* End of 'getRadius' function */

/* Getting position of the sphere function */
math::Vec3f BoundingSphere::getPos(void) const
{
  return _body->getPos();
} /* End of 'getPos' function */

/* Virtual function of colliding to bounding volumes */
bool BoundingSphere::isCollide(const BoundingVolume *Volume) const
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

/* Function of colliding sphere and another shape */
bool BoundingSphere::isCollide(const BoundingSphere *Sph) const
{
  float dist = (_body->getPos() - Sph->getPos()).length();
  if (dist <= _radius + Sph->getRadius())
    return true;
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with plane function */
bool BoundingSphere::isCollide(const BoundingPlane *Plane) const
{
  // Cache the sphere position.
  math::Vec3f position = _body->getPos();
  // Find the distance from the plane.
  float centerDistance = Plane->getNormal() * position - Plane->getOffset();

  // Check if we’re within radius.
  if (centerDistance * centerDistance > _radius * _radius)
    return false;
  return true;
} /* End of 'isCollide' function */

/* Colliding sphere with box function */
bool BoundingSphere::isCollide(const BoundingBox *Box) const
{
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with sphere function */
std::vector<Contact> BoundingSphere::getContactData(const BoundingSphere *Sph) const
{
  Contact contact;

  math::Vec3f midline = getPos() - Sph->getPos();
  float size = midline.length();
  contact._normal = midline.getNormalized();
  contact._position = getPos() + midline * 0.5F;
  contact._penetration = _radius + Sph->getRadius() - size;
  contact.calculateContactBasis();

  std::vector<Contact> contactVec;
  contactVec.push_back(contact);
  return contactVec;
} /* End of 'getContactData' function */

/* Colliding sphere with plane function */
std::vector<Contact> BoundingSphere::getContactData(const BoundingPlane *Plane) const
{
  // Cache the sphere position.
  math::Vec3f position = _body->getPos();
  // Find the distance from the plane.
  float centerDistance = Plane->getNormal() * position - Plane->getOffset();
  // Check which side of the plane we’re on.
  math::Vec3f normal = Plane->getNormal();
  float penetration = -centerDistance;
  if (centerDistance < 0)
  {
    normal *= -1;
    penetration = -penetration;
  }
  penetration += _radius;

  // Create the contact - it has a normal in the plane direction.
  Contact contact;
  contact._normal = normal;
  contact._penetration = penetration;
  contact._position = position - Plane->getNormal() * centerDistance;
  contact.calculateContactBasis();

  std::vector<Contact> contactVec;
  contactVec.push_back(contact);
  return contactVec;
} /* End of 'getContactData' function */

/* Colliding sphere with box function */
std::vector<Contact> BoundingSphere::getContactData(const BoundingBox *Box) const
{
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* Virtual function of getting the colliding data */
std::vector<Contact> BoundingSphere::getContactData(const BoundingVolume *Volume) const
{
  if (Volume->_type == bounding_volume_type::SPHERE)
    return getContactData((BoundingSphere *)Volume);
  else if (Volume->_type == bounding_volume_type::BOX)
    return getContactData((BoundingBox *)Volume);
  else if (Volume->_type == bounding_volume_type::PLANE)
    return getContactData((BoundingPlane *)Volume);
} /* End of 'getContactData' function */

/* Debug function for setting primitive */
//void BoundingSphere::setPrimitive(const std::string &Name, const render::Geom &Geom)
//{
//  //render::Render &Rnd = render::Render::getInstance();
//  //_primitive = Rnd.createPrim(Name, Rnd.createGeom("sphere", geom::Geom().createSphere({ 0, 0, 0 }, 1, 50, 50)));
//} /* End of 'setPrimitive' function */

/* END OF 'bounding_sphere.cpp' FILE */
