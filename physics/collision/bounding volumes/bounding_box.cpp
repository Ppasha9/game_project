/*
 * Game project
 *
 * FILE: bounding_box.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 02.05.2018
 * NOTE: box bounding volume definition file
 */

#include "bounding_volume_def.h"
#include "../../../render/render.h"

using namespace phys;

/* Setting the vertices to start position function */
void BoundingBox::setStartVertices(void)
{
  math::Vec3f objPos = { 0, 0, 0 };
  math::Vec3f norm = (_dirHalfVec & _rightHalfVec).getNormalized();

  _vertices[0] = objPos - _dirHalfVec - _rightHalfVec + norm * _halfHeight;
  _vertices[1] = objPos - _dirHalfVec - _rightHalfVec - norm * _halfHeight;
  _vertices[2] = objPos - _dirHalfVec + _rightHalfVec - norm * _halfHeight;
  _vertices[3] = objPos - _dirHalfVec + _rightHalfVec + norm * _halfHeight;

  _vertices[4] = objPos + _dirHalfVec - _rightHalfVec + norm * _halfHeight;
  _vertices[5] = objPos + _dirHalfVec - _rightHalfVec - norm * _halfHeight;
  _vertices[6] = objPos + _dirHalfVec + _rightHalfVec - norm * _halfHeight;
  _vertices[7] = objPos + _dirHalfVec + _rightHalfVec + norm * _halfHeight;
} /* End of 'setStartVertices' function */

/* Class constructor */
BoundingBox::BoundingBox(PhysObject *Obj, const math::Vec3f &DirHalfVec, const math::Vec3f &RightHalfVec, const float HalfHeight, const std::string &ObjName)
  : BoundingVolume(Obj, ObjName, bounding_volume_type::BOX), _dirHalfVec(DirHalfVec), _rightHalfVec(RightHalfVec), _halfHeight(HalfHeight)
{
  updateVertices();

  render::Render &rnd = render::Render::getInstance();
  rnd.createPrim(_primName, rnd.createGeom(ObjName + "_box", geom::Geom().createBox({ 0, 0, 0 }, 2.0f * HalfHeight)), rnd.getMaterial("mtl"), rnd.getShader("default"),
    render::Prim::ProjMode::FRUSTUM, render::Prim::FillMode::WIREFRAME);
} /* End of constructor */

/* Update current vertices position function */
void BoundingBox::updateVertices(void)
{
  setStartVertices();

  math::Matr4f trans = _body->getTransormMatrix();
  for (int i = 0; i < 8; i++)
  {
    // _vertices[i] * trans
    float x = _vertices[i][0] * trans._values[0][0] + _vertices[i][1] * trans._values[1][0] + _vertices[i][2] * trans._values[2][0] + trans._values[3][0];
    float y = _vertices[i][0] * trans._values[0][1] + _vertices[i][1] * trans._values[1][1] + _vertices[i][2] * trans._values[2][1] + trans._values[3][1];
    float z = _vertices[i][0] * trans._values[0][2] + _vertices[i][1] * trans._values[1][2] + _vertices[i][2] * trans._values[2][2] + trans._values[3][2];

    _vertices[i] = { x, y, z };
  }
} /* End of 'updateVertices' function */

/* Getting direction half-size of box function */
math::Vec3f BoundingBox::getDirHalfVec(void) const
{
  return _dirHalfVec;
} /* End of 'getDirHalfVec' function */

/* Getting right half-size of box function */
math::Vec3f BoundingBox::getRightHalfVec(void) const
{
  return _rightHalfVec;
} /* End of 'getRightHalfVec' function */

/* Getting half of the height of the box */
float BoundingBox::getHalfHeight(void) const
{
  return _halfHeight;
} /* End of 'getHalfHeight' function */

/* Integrate volume's physics object */
void BoundingBox::integrate(const float Duration)
{
  _body->integrate(Duration);
  updateVertices();
} /* End of 'integrate' function */

/* Getting the closest vertex to certain point function */
math::Vec3f BoundingBox::getClosestVertexToPoint(const math::Vec3f &Point) const
{
  math::Vec3f res = _vertices[0];
  float dist = (Point - _vertices[0]).length();
  for (int i = 1; i < 8; i++)
    if (dist > (Point - _vertices[i]).length())
    {
      dist = (Point - _vertices[i]).length();
      res = _vertices[i];
    }

  return res;
} /* End of 'getClosestVertexToPoint' function */

/* Getting boundary vertex position function */
math::Vec3f BoundingBox::getVertexPos(const int Index) const
{
  return _vertices[Index];
} /* End of 'getVertexPos' function */

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
  return false;
} /* End of 'isCollide' function */

/* Colliding box with sphere function */
bool BoundingBox::isCollide(const BoundingSphere *Sph) const
{
  // Transform the center of the sphere into box coordinates.
  math::Vec3f center = Sph->getPos();
  math::Matr4f invTrans = _body->getTransormMatrix().getInverse();

  float x = center[0] * invTrans._values[0][0] + center[1] * invTrans._values[1][0] + center[2] * invTrans._values[2][0] + invTrans._values[3][0];
  float y = center[0] * invTrans._values[0][1] + center[1] * invTrans._values[1][1] + center[2] * invTrans._values[2][1] + invTrans._values[3][1];
  float z = center[0] * invTrans._values[0][2] + center[1] * invTrans._values[1][2] + center[2] * invTrans._values[2][2] + invTrans._values[3][2];
  math::Vec3f relCenter = { x, y, z };

  // Early-out check to see if we can exclude the contact.
  float maxX = _rightHalfVec.length();
  float maxY = _halfHeight;
  float maxZ = _dirHalfVec.length();

  if (fabs(relCenter[0]) - Sph->getRadius() > maxX ||
      fabs(relCenter[1]) - Sph->getRadius() > maxY ||
      fabs(relCenter[2]) - Sph->getRadius() > maxZ)
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
  if (dist > Sph->getRadius() * Sph->getRadius())
    return false;
  return true;
} /* End of 'isCollide' function */

/* Collidding with the point */
bool BoundingBox::isCollide(const math::Vec3f &Point, math::Vec3f &Normal, float &Penetration) const
{
  // Transform the point into box coordinates.
  math::Matr4f invTrans = _body->getTransormMatrix().getInverse();
  float x = Point[0] * invTrans._values[0][0] + Point[1] * invTrans._values[1][0] + Point[2] * invTrans._values[2][0] + invTrans._values[3][0];
  float y = Point[0] * invTrans._values[0][1] + Point[1] * invTrans._values[1][1] + Point[2] * invTrans._values[2][1] + invTrans._values[3][1];
  float z = Point[0] * invTrans._values[0][2] + Point[1] * invTrans._values[1][2] + Point[2] * invTrans._values[2][2] + invTrans._values[3][2];

  math::Vec3f relPt = { x, y, z };
  math::Vec3f normal;

  // Check each axis, looking for the axis on which the penetration is least deep.
  float min_depth = _rightHalfVec.length() - fabs(relPt[0]);
  if (min_depth < 0)
    return false;
  normal._coords[0] = _body->getPos()[0] * ((relPt[0] < 0) ? -1 : 1);

  float depth = _halfHeight - fabs(relPt[1]);
  if (depth < 0)
    return false;
  else if (depth < min_depth)
  {
    min_depth = depth;
    normal._coords[1] = _body->getPos()[1] * ((relPt[1] < 0) ? -1 : 1);
  }

  depth = _dirHalfVec.length() - fabs(relPt[2]);
  if (depth < 0)
    return false;
  else if (depth < min_depth)
  {
    min_depth = depth;
    normal._coords[2] = _body->getPos()[2] * ((relPt[2] < 0) ? -1 : 1);
  }

  // Compile the contact.
  Normal = -normal.getNormalized();
  Penetration = min_depth;
  return true;
} /* End of 'isCollide' function */

/* Colliding box with box function */
bool BoundingBox::isCollide(const BoundingBox *Box) const
{
  math::Vec3f tmpNormal;
  float tmpPenetration;

  for (int i = 0; i < 8; i++)
    if (Box->isCollide(_vertices[i], tmpNormal, tmpPenetration))
      return true;

  return false;
} /* End of 'isCollide' function */

/* Colliding box with plane function */
bool BoundingBox::isCollide(const BoundingPlane *Plane) const
{
  for (int i(0); i < 8; i++)
    if (_vertices[i] * Plane->getNormal() <= Plane->getOffset())
      return true;
  return false;
} /* End of 'isCollide' function */

/* Colliding sphere with sphere function */
std::vector<Contact> BoundingBox::getContactData(const BoundingSphere *Sph) const
{
  // Transform the center of the sphere into box coordinates.
  math::Vec3f center = Sph->getPos();
  math::Matr4f invTrans = _body->getTransormMatrix().getInverse();

  float x = center[0] * invTrans._values[0][0] + center[1] * invTrans._values[1][0] + center[2] * invTrans._values[2][0] + invTrans._values[3][0];
  float y = center[0] * invTrans._values[0][1] + center[1] * invTrans._values[1][1] + center[2] * invTrans._values[2][1] + invTrans._values[3][1];
  float z = center[0] * invTrans._values[0][2] + center[1] * invTrans._values[1][2] + center[2] * invTrans._values[2][2] + invTrans._values[3][2];
  math::Vec3f relCenter = { x, y, z };

  // Early-out check to see if we can exclude the contact.
  float maxX = _rightHalfVec.length();
  float maxY = _halfHeight;
  float maxZ = _dirHalfVec.length();

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
  math::Matr4f trans = _body->getTransormMatrix();
  x = closestPt[0] * trans._values[0][0] + closestPt[1] * trans._values[1][0] + closestPt[2] * trans._values[2][0] + trans._values[3][0];
  y = closestPt[0] * trans._values[0][1] + closestPt[1] * trans._values[1][1] + closestPt[2] * trans._values[2][1] + trans._values[3][1];
  z = closestPt[0] * trans._values[0][2] + closestPt[1] * trans._values[1][2] + closestPt[2] * trans._values[2][2] + trans._values[3][2];
  math::Vec3f closestPtWorld = { x, y, z };

  Contact contact;
  contact._normal = -(center - closestPtWorld).getNormalized();
  contact._position = closestPtWorld;
  contact._penetration = Sph->getRadius() - sqrt(dist);
  contact.calculateContactBasis();

  // Write the appropriate data.
  std::vector<Contact> res;
  res.push_back(contact);
  return res;
} /* End of 'getContactData' function */

/* Colliding sphere with plane function */
std::vector<Contact> BoundingBox::getContactData(const BoundingPlane *Plane) const
{
  std::vector<Contact> res;
  for (int i(0); i < 8; i++)
  {
    float curVertexDist = _vertices[i] * Plane->getNormal();
    if (curVertexDist <= Plane->getOffset())
    {
      // Generate a contact point data
      Contact contact;

      // The contact point is halfway between the vertex and the
      // plane - we multiply the direction by half the separation distance and add the vertex location.
      //contact._position = _vertices[i] +  Plane->getNormal() * (curVertexDist - Plane->getOffset());
      contact._position = _vertices[i];
      contact._normal = Plane->getNormal();
      contact._penetration = Plane->getOffset() - curVertexDist;
      contact.calculateContactBasis();

      res.push_back(contact);
    }
  }

  return res;
} /* End of 'getContactData' function */

/* Colliding sphere with box function */
std::vector<Contact> BoundingBox::getContactData(const BoundingBox *Box) const
{
  std::vector<Contact> res;
  math::Vec3f tmpNormal;
  float tmpPenetration;

  for (int i = 0; i < 8; i++)
    if (isCollide(Box->_vertices[i], tmpNormal, tmpPenetration))
    {
      Contact contact = Contact(Box->_vertices[i], tmpNormal, tmpPenetration);
      contact.calculateContactBasis();

      res.push_back(contact);
    }

  return res;
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
  return std::vector<Contact>();
} /* End of 'getContactData' function */

/* END OF 'bounding_box.cpp' FILE */
