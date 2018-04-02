/*
 * Game project
 *
 * FILE: bounding_box.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: box bounding volume declaration file
 */

#ifndef _BOUNDING_BOX_H_INCLUDED__
#define _BOUNDING_BOX_H_INCLUDED__
#pragma once

#include "bounding_volume.h"

/* Physics namespace */
namespace phys
{
  /* Forward declaration */
  class BoundingSphere;
  class BoundingPlane;

  /* Box bounding volume class */
  class BoundingBox : public BoundingVolume
  {
  private:
    /* Vector of half size of box */
    math::Vec3f _halfSize;

  public:
    /* Default class constructor */
    BoundingBox(void) = default;

    /* Class constructor */
    BoundingBox(PhysObject &Obj, const math::Vec3f &HalfSize);

    /* Getting size of box function */
    math::Vec3f getHalfSize(void) const;

    /* Setting new size of box function */
    void setHalfSize(const math::Vec3f HalfSize);

    /* Colliding box with sphere function */
    virtual bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding box with box function */
    virtual bool isCollide(const BoundingBox *Box) const;

    /* Colliding box with plane function */
    virtual bool isCollide(const BoundingPlane *Plane) const;
  }; /* End of 'BoundingBox' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_BOX_H_INCLUDED__ */

/* END OF 'bounding_box.h' FILE */
