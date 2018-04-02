/*
 * Game project
 *
 * FILE: bounding_sphere.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: sphere bounding volume declaration file
 */

#ifndef _BOUNDING_SPHERE_H_INCLUDED__
#define _BOUNDING_SPHERE_H_INCLUDED__
#pragma once

#include "bounding_volume.h"

/* Physics namespace */
namespace phys
{
  /* Forward declaration */
  class BoundingBox;
  class BoundingPlane;

  /* Sphere bounding volume class */
  class BoundingSphere : public BoundingVolume
  {
  private:
    /* Radius of the sphere */
    float _radius;

  public:
    /* Default class constructor */
    BoundingSphere(void) = default;

    /* Class constructor */
    BoundingSphere(PhysObject &Obj, const float Radius);

    /* Getting radius of the sphere function */
    float getRadius(void) const;

    /* Colliding sphere with sphere function */
    virtual bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding sphere with plane function */
    virtual bool isCollide(const BoundingPlane *Plane) const;

    /* Colliding sphere with box function */
    virtual bool isCollide(const BoundingBox *Box) const;
  }; /* End of 'BoundingSphere' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_SPHERE_H_INCLUDED__ */

/* END OF 'bounding_sphere.h' FILE */
