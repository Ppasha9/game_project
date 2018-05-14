/*
 * Game project
 *
 * FILE: bounding_sphere.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 02.05.2018
 * NOTE: sphere bounding volume declaration file
 */

#include "bounding_volume_def.h"
#include "../../../render/render.h"

using namespace phys;

/* Class constructor */
BoundingSphere::BoundingSphere(PhysObject *Obj, const float Radius, const std::string &ObjName) : BoundingVolume(Obj, ObjName), _radius(Radius)
{
  render::Render &rnd = render::Render::getInstance();
  rnd.createPrim(_primName, rnd.createGeom(ObjName + "_sphere", geom::Geom().createSphere({ 0, 0, 0 }, Radius, 12, 12)), rnd.getMaterial("mtl"), rnd.getShader("default"),
    render::Prim::ProjMode::FRUSTUM, render::Prim::FillMode::WIREFRAME);
} /* End of constructor */

/* Getting radius of the sphere function */
float BoundingSphere::getRadius(void) const
{
  return _radius;
} /* End of 'getRadius' function */

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
  return false;
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
  // Transform the center of the sphere into box coordinates.
  math::Vec3f center = _body->getPos();
  math::Matr4f invTrans = Box->getMatr().getInverse();

  float x = center[0] * invTrans._values[0][0] + center[1] * invTrans._values[1][0] + center[2] * invTrans._values[2][0] + invTrans._values[3][0];
  float y = center[0] * invTrans._values[0][1] + center[1] * invTrans._values[1][1] + center[2] * invTrans._values[2][1] + invTrans._values[3][1];
  float z = center[0] * invTrans._values[0][2] + center[1] * invTrans._values[1][2] + center[2] * invTrans._values[2][2] + invTrans._values[3][2];
  math::Vec3f relCenter = { x, y, z };

  // Early-out check to see if we can exclude the contact.
  float maxX = Box->getRightHalfVec().length();
  float maxY = Box->getHalfHeight();
  float maxZ = Box->getDirHalfVec().length();

  if (fabs(relCenter[0]) - _radius > maxX ||
      fabs(relCenter[1]) - _radius > maxY ||
      fabs(relCenter[2]) - _radius > maxZ)
    return false;

  math::Vec3f closestPt = { 0, 0, 0 };
  float dist;

  // Clamp each coordinate to the box.
  dist = relCenter[0];
  if (dist > maxX)
    dist = maxX;
  if (dist < -maxX)
    dist = -maxX;
  closestPt._coords[0] = dist;

  dist = relCenter[1];
  if (dist > maxY)
    dist = maxY;
  if (dist < -maxY)
    dist = -maxY;
  closestPt._coords[1] = dist;

  dist = relCenter[2];
  if (dist > maxZ)
    dist = maxZ;
  if (dist < -maxZ)
    dist = -maxZ;
  closestPt._coords[2] = dist;

  // Check we’re in contact.
  dist = (closestPt - relCenter).norm();
  if (dist > _radius * _radius)
    return false;
  return true;
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
  // Transform the center of the sphere into box coordinates.
  math::Vec3f center = _body->getPos();
  math::Matr4f invTrans = Box->getMatr().getInverse();

  float x = center[0] * invTrans._values[0][0] + center[1] * invTrans._values[1][0] + center[2] * invTrans._values[2][0] + invTrans._values[3][0];
  float y = center[0] * invTrans._values[0][1] + center[1] * invTrans._values[1][1] + center[2] * invTrans._values[2][1] + invTrans._values[3][1];
  float z = center[0] * invTrans._values[0][2] + center[1] * invTrans._values[1][2] + center[2] * invTrans._values[2][2] + invTrans._values[3][2];
  math::Vec3f relCenter = { x, y, z };

  // Early-out check to see if we can exclude the contact.
  float maxX = Box->getRightHalfVec().length();
  float maxY = Box->getHalfHeight();
  float maxZ = Box->getDirHalfVec().length();

  math::Vec3f closestPt = { 0, 0, 0 };
  float dist;

  // Clamp each coordinate to the box.
  dist = relCenter[0];
  if (dist > maxX)
    dist = maxX;
  if (dist < -maxX)
    dist = -maxX;
  closestPt._coords[0] = dist;

  dist = relCenter[1];
  if (dist > maxY)
    dist = maxY;
  if (dist < -maxY)
    dist = -maxY;
  closestPt._coords[1] = dist;

  dist = relCenter[2];
  if (dist > maxZ)
    dist = maxZ;
  if (dist < -maxZ)
    dist = -maxZ;
  closestPt._coords[2] = dist;

  dist = (closestPt - relCenter).norm();

  // Compile the contact.
  math::Matr4f trans = Box->getMatr();
  x = closestPt[0] * trans._values[0][0] + closestPt[1] * trans._values[1][0] + closestPt[2] * trans._values[2][0] + trans._values[3][0];
  y = closestPt[0] * trans._values[0][1] + closestPt[1] * trans._values[1][1] + closestPt[2] * trans._values[2][1] + trans._values[3][1];
  z = closestPt[0] * trans._values[0][2] + closestPt[1] * trans._values[1][2] + closestPt[2] * trans._values[2][2] + trans._values[3][2];
  math::Vec3f closestPtWorld = { x, y, z };

  Contact contact;
  contact._normal = (center - closestPtWorld).getNormalized();
  contact._position = closestPtWorld;
  contact._penetration = _radius - sqrt(dist);
  contact.calculateContactBasis();

  // Write the appropriate data.
  std::vector<Contact> res;
  res.push_back(contact);
  return res;
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
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* END OF 'bounding_sphere.cpp' FILE */
