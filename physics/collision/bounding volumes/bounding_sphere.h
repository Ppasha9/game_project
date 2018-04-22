/*
 * Game project
 *
 * FILE: bounding_sphere.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
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
    BoundingSphere(PhysObject *Obj, const float Radius);

    /* Getting radius of the sphere function */
    float getRadius(void) const;

    /* Getting position of the sphere function */
    math::Vec3f getPos(void) const;

    /*
     * Collision functions.
     */

    /* Colliding sphere with sphere function */
    bool isCollide(const BoundingSphere *Sph) const;

    /* Colliding sphere with plane function */
    bool isCollide(const BoundingPlane *Plane) const;

    /* Colliding sphere with box function */
    bool isCollide(const BoundingBox *Box) const;

    /* Virtual function of colliding to bounding volumes */
    virtual bool isCollide(const BoundingVolume *Volume) const;

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

    /* Debug function for setting primitive */
    //virtual void setPrimitive(const std::string &Name, const render::Geom &Geom);
  }; /* End of 'BoundingSphere' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_SPHERE_H_INCLUDED__ */

/* END OF 'bounding_sphere.h' FILE */
