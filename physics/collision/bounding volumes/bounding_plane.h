/*
 * Game project
 *
 * FILE: bounding_plane.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 02.05.2018
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
    /* Two boundary points of the plane */
    math::Vec3f _fPoint;
    math::Vec3f _sPoint;
    /* Offset of the plane.
     * Distance to a (0, 0, 0) point
     */
    float _offset;

  public:
    /* Default class constructor */
    BoundingPlane(void) = default;

    /* Class constructor */
    BoundingPlane(PhysObject *Obj, const math::Vec3f &Normal, const math::Vec3f &FPoint, const math::Vec3f &SPoint, const std::string &ObjName);

    /* Getting plane normal function */
    math::Vec3f getNormal(void) const;

    /* Getting plane offset function */
    math::Vec3f getFPoint(void) const;

    /* Getting plane offset function */
    math::Vec3f getSPoint(void) const;

    /* Getting plane offset function */
    float getOffset(void) const;

    /*
     * Collision functions.
     */

    /* Virtual function of colliding to bounding volumes */
    virtual bool isCollide(const BoundingVolume *Volume) const;

    /* Collide plane with sphere function */
    bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding plane with box function */
    bool isCollide(const BoundingBox *Box) const;

    /* Colliding plane with box function */
    bool isCollide(const BoundingPlane *Plane) const;

    /*
     * Contact functions.
     */

    /* Colliding sphere with sphere function */
    std::vector<Contact> getContactData(const BoundingSphere *Sph) const;

    /* Colliding sphere with plane function */
    std::vector<Contact> getContactData(const BoundingPlane *Plane) const;

    /* Colliding sphere with box function */
    std::vector<Contact> getContactData(const BoundingBox *Box) const;

    /* Virtual function of getting the colliding data */
    virtual std::vector<Contact> getContactData(const BoundingVolume *Volume) const;
  }; /* End of 'BoundingPlane' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_PLANE_H_INCLUDED__ */

/* END OF 'bounding_plane.h' FILE */
