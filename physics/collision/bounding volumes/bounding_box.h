/*
 * Game project
 *
 * FILE: bounding_box.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 02.05.2018
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
    /* Vector of direction hals-size of box */
    math::Vec3f _dirHalfVec;
    /* Vector of right hals-size of box */
    math::Vec3f _rightHalfVec;
    /* Half of the height of the box */
    float _halfHeight;

    /* Boundary vertices of the box */
    math::Vec3f _vertices[8];

    /* Update current vertices position function */
    void updateVertices(void);

    /* Setting the vertices to start position function */
    void setStartVertices(void);

    /* Getting the closest vertex to certain point function */
    math::Vec3f getClosestVertexToPoint(const math::Vec3f &Point) const;

    /* Collidding with the point */
    bool isCollide(const math::Vec3f &Point, math::Vec3f &Normal, float &Penetration) const;

  public:
    /* Default class constructor */
    BoundingBox(void) = default;

    /* Class constructor */
    BoundingBox(PhysObject *Obj, const math::Vec3f &DirHalfVec, const math::Vec3f &RightHalfVec, const float HalfHeight, const std::string &ObjName);

    /*
     * Informative functions.
     */

    /* Getting direction half-size of box function */
    math::Vec3f getDirHalfVec(void) const;

    /* Getting right half-size of box function */
    math::Vec3f getRightHalfVec(void) const;

    /* Getting half of the height of the box */
    float getHalfHeight(void) const;

    /* Integrate volume's physics object */
    virtual void integrate(const float Duration);

    /* Getting boundary vertex position function */
    math::Vec3f getVertexPos(const int Index) const;

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
