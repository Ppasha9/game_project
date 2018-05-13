/*
 * Game project
 *
 * FILE: collision_detector.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: detection system definition file
 */

#include "../../../render/render.h"
#include "../../../render/timer/timer.h"
#include "collision_detector.h"

using namespace phys;

/* Does the pairs are equivalent or not */
bool ContactDetector::isEqual(const BoundingVolumePair &FPair, const BoundingVolumePair &SPair) const
{
  return (FPair.first == SPair.first && FPair.second == SPair.second) ||
    (FPair.second == SPair.first && FPair.first == SPair.second);
} /* End of 'isEqual' function */

/* Does the vector contain certain pair */
bool ContactDetector::contain(const CollidingObjectsVector &Vector, const BoundingVolumePair &Pair) const
{
  for (auto elem : Vector)
    if (isEqual(elem, Pair) || isEqual(elem, BoundingVolumePair(Pair.second, Pair.first)))
      return true;

  return false;
} /* End of 'contain' function */

/* Adding new bounding volume function */
void ContactDetector::addVolume(const std::string &Name, BoundingVolume *Volume)
{
  _boundingVolumes.insert(std::pair<std::string, BoundingVolume *>(Name, Volume));
} /* End of 'addVolume' function */

/* Response function */
CollidingObjectsVector ContactDetector::response() const
{
  CollidingObjectsVector vector;
  for (auto &it1 : _boundingVolumes)
    for (auto &it2 : _boundingVolumes)
    {
      if (it2 == it1)
        continue;

      if (it1.second->isCollide(it2.second))
      {
        BoundingVolumePair pair(it1.second, it2.second);
        if (!contain(vector, pair))
          vector.push_back(pair);
      }
    }

  return vector;
} /* End of 'response' function */

/* Class destructor */
ContactDetector::~ContactDetector(void)
{
  _boundingVolumes.clear();
} /* End of destructor */

/* Getting physics object transformation matrix for rendering function */
const math::Matr4f ContactDetector::getObjectMatrix(const std::string &Name) const
{
  std::map<std::string, BoundingVolume *>::const_iterator res = _boundingVolumes.find(Name);

  if (res == _boundingVolumes.end())
    return math::Matr4f().setIdentity();

  return res->second->getMatr();
} /* End of 'getObjectMatrix' function */

/* Apply force function */
void ContactDetector::applyForceToObj(const std::string &ObjName, const Force *Force)
{
  _boundingVolumes[ObjName]->applyForceToObj(Force);
} /* End of 'applyForceToObj' function */

/* Integrate function */
void ContactDetector::integrate(void)
{
  render::Timer &timer = render::Timer::getInstance();
  for (auto &elem : _boundingVolumes)
    elem.second->integrate(timer._deltaTime);
} /* End of 'integrate' function */

/* Getting the pointer to physics object function */
PhysObject * ContactDetector::getObject(const std::string &Name)
{
  return _boundingVolumes[Name]->getPhysObjectPointer();
} /* End of 'getObject' function */

/* Drawing debug primitives function */
void ContactDetector::debugDraw(void) const
{
  render::Render &rnd = render::Render::getInstance();
  for (BoundingVolumeMap::const_iterator it = _boundingVolumes.begin(); it != _boundingVolumes.end(); it++)
    rnd.drawPrim(rnd.getPrim(it->second->getPrimName()), getObjectMatrix(it->first));
} /* End of 'debugDraw' function */

/* END OF 'collision_detector.cpp' FILE */
