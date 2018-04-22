/*
 * Game project
 *
 * FILE: bounding_volume.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: basic bounding volume declaration file
 */

#ifndef _BOUNDING_VOLUME_H_INCLUDED__
#define _BOUNDING_VOLUME_H_INCLUDED__
#pragma once

#include "../../../math/math_def.h"
#include "../../phys_object/phys_object.h"
#include "../../../render/timer/timer.h"

/* Physics namespace */
namespace phys
{
  /* Contact data class */
  class Contact
  {
  public:
    /* Position of the contact */
    math::Vec3f _position;
    /* Normal of point of the contact */
    math::Vec3f _normal;
    /* Value of penetration */
    float _penetration;
    /* Basis matrix */
    math::Matr3f _basisMatrix;

    /* Default class constructor */
    Contact(void);

    /* Class constructor */
    Contact(const math::Vec3f &Pos, const math::Vec3f &Normal, const float Penetration);

    /*
     * Constructs an arbitrary orthonormal basis for the contact.
     * This is stored as a 3x3 matrix, where each vector is a column
     * (in other words the matrix transforms contact space into world
     * space). The x direction is generated from the contact normal,
     * and the y and z directions are set so they are at right angles to
     * it.
     */
    void calculateContactBasis(void);
  }; /* End of 'Contact' class */

  /* Bounding volume type enum */
  enum struct bounding_volume_type
  {
    SPHERE,
    BOX,
    PLANE,
    VOLUMES_AMOUNT
  }; /* End of 'bounding_volume_type' enumeration */

  /* Bounding volume class */
  class BoundingVolume
  {
  protected:
    /* The physic object */
    PhysObject *_body;
    /* The offset of primitive due to physic object */
    math::Matr4f _offset;
    /* The bounding primitive (for debug) */
    //render::PrimPtr _primitive;

  public:
    /* Type of the bounding volume */
    bounding_volume_type _type;

    /* Default class constructor */
    BoundingVolume(void) = default;

    /* Class constructor */
    BoundingVolume(PhysObject *Obj, const bounding_volume_type Type = bounding_volume_type::SPHERE) : _body(Obj), _type(Type)
    {
    } /* End of constructor */

    /* Class constructor */
    BoundingVolume(PhysObject *Obj, math::Matr4f &Matr, const bounding_volume_type Type = bounding_volume_type::SPHERE) : _body(Obj), _offset(Matr), _type(Type)
    {
    } /* End of constructor */

    /* Integrate volume's physics object */
    void integrate(const float Duration)
    {
      _body->integrate(Duration);
    } /* End of 'integrate' function */

    /* Getting body transform matrix function */
    const math::Matr4f getMatr(void) const
    {
      return _body->getTransormMatrix();
    } /* End of 'getMatr' function */

    /* Getting body pointer function */
    PhysObject * getPhysObjectPointer(void) const
    {
      return _body;
    } /* End of 'getPhysObjectPointer' function */

    /* Drawing debug primitive function */
    void draw(void)
    {
      //render::Render &Rnd = render::Render::getInstance();
      //Rnd.draw
    } /* End of 'draw' function */

    /* Apply force function */
    void applyForceToObj(const Force *Force)
    {
      render::Timer &timer = render::Timer::getInstance();
      _body->applyForce(Force, (float)timer._deltaTime);
    } /* End of 'applyForceToObj' function */

    /* Virtual function of colliding to bounding volumes */
    virtual bool isCollide(const BoundingVolume *Volume) const
    {
      return false;
    } /* End of 'isCollide' function */

    /* Virtual function of getting the colliding data */
    virtual std::vector<Contact> getContactData(const BoundingVolume *Volume) const
    {
      return std::vector<Contact>();
    } /* End of 'getContactData' function */

    /* Debug function for setting primitive */
    //virtual void setPrimitive(const std::string &Name, const render::Geom &Geom)
    //{
    //} /* End of 'setPrimitive' function */
  }; /* End of 'BoundingVolume' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_VOLUME_H_INCLUDED__ */

/* END OF 'bounding_volume.h' FILE */
