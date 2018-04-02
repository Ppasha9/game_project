/*
 * Game project
 *
 * FILE: bounding_plane.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: plane bounding volume declaration file
 */

#ifndef _BOUNDING_PLANE_H_INCLUDED__
#define _BOUNDING_PLANE_H_INCLUDED__
#pragma once

#include "bounding_volume.h"

/* Physics namespace */
namespace phys
{
  /* Forward declaration */
  class BoundingSphere;
  class BoundingBox;

  /* Plane bounding volume class */
  class BoundingPlane : public BoundingVolume
  {
  private:
    /* Plane's normal vector */
    math::Vec3f _normal;
    /* Offset of the plane */
    float _offset;

  public:
    /* Default class constructor */
    BoundingPlane(void) = default;

    /* Class constructor */
    BoundingPlane(PhysObject &Obj, const math::Vec3f &Normal, const float Offset);

    /* Getting plane normal function */
    math::Vec3f getNormal(void) const;

    /* Getting plane offset function */
    float getOffset(void) const;

    /* Collide plane with sphere function */
    virtual bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding plane with box function */
    virtual bool isCollide(const BoundingBox *Box) const;
  }; /* End of 'BoundingPlane' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_PLANE_H_INCLUDED__ */

/* END OF 'bounding_plane.h' FILE */
