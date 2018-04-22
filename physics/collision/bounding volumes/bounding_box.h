/*
 * Game project
 *
 * FILE: bounding_box.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
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
    BoundingBox(PhysObject *Obj, const math::Vec3f &HalfSize);

    /* Getting size of box function */
    math::Vec3f getHalfSize(void) const;

    /* Setting new size of box function */
    void setHalfSize(const math::Vec3f HalfSize);

    /*
     * Collision functions.
     */

    /* Virtual function of colliding to bounding volumes */
    virtual bool isCollide(const BoundingVolume *Volume) const;

    /* Colliding box with sphere function */
    bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding box with box function */
    bool isCollide(const BoundingBox *Box) const;

    /* Colliding box with plane function */
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
  }; /* End of 'BoundingBox' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_BOX_H_INCLUDED__ */

/* END OF 'bounding_box.h' FILE */
