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
    /* Vector of bounding volumes */
    std::vector<BoundingVolume *> _boundingVolumes;

  public:
    /* Default class constructor */
    ContactDetector(void) = default;

    /* Adding new bounding volume function */
    void addVolume(BoundingVolume *Volume);

    /* Response function */
    CollidingObjectsVector response(void) const;

    /* Class destructor */
    ~ContactDetector(void);
  }; /* End of 'ContactDetector' class */
}; /* End of 'phys' namespace */

#endif /* _COLLISION_DETECTOR_H_INCLUDED__ */

/* END OF 'collision_detector.h' FILE */
