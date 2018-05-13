/*
 * Game project
 *
 * FILE: collision_detector.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: detection system declaration file
 */

#ifndef _COLLISION_DETECTOR_H_INCLUDED__
#define _COLLISION_DETECTOR_H_INCLUDED__
#pragma once

#include <vector>
#include <map>
#include "../bounding volumes/bounding_volume.h"

/* Physics namespace */
namespace phys
{
  /* Bounding volumes pair */
  using BoundingVolumePair = std::pair<BoundingVolume *, BoundingVolume *>;
  /* Result of detector response function: pairs of colliding objects id */
  using CollidingObjectsVector = std::vector<BoundingVolumePair>;

  /* Contact detector class */
  class ContactDetector
  {
  private:
    /* Bounding volume map definition */
    using BoundingVolumeMap = std::map<std::string, BoundingVolume *>;

    /* Vector of bounding volumes */
    BoundingVolumeMap _boundingVolumes;

    /* Does the pairs are equivalent or not */
    bool isEqual(const BoundingVolumePair &FPair, const BoundingVolumePair &SPair) const;

    /* Does the vector contain certain pair */
    bool contain(const CollidingObjectsVector &Vector, const BoundingVolumePair &Pair) const;

  public:
    /* Default class constructor */
    ContactDetector(void) = default;

    /* Adding new bounding volume function */
    void addVolume(const std::string &Name, BoundingVolume *Volume);

    /* Response function */
    CollidingObjectsVector response(void) const;

    /* Getting physics object transformation matrix for rendering function */
    const math::Matr4f getObjectMatrix(const std::string &Name) const;

    /* Apply force function */
    void applyForceToObj(const std::string &ObjName, const Force *Force);

    /* Getting the pointer to physics object function */
    PhysObject * getObject(const std::string &Name);

    void remove(const string &Name);

    /* Integrate function */
    void integrate(void);

    /* Drawing debug primitives function */
    void debugDraw(void) const;

    /* Class destructor */
    ~ContactDetector(void);
  }; /* End of 'ContactDetector' class */
}; /* End of 'phys' namespace */

#endif /* _COLLISION_DETECTOR_H_INCLUDED__ */

/* END OF 'collision_detector.h' FILE */
