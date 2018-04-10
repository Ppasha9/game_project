/*
 * Game project
 *
 * FILE: bounding_volume.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 01.04.2018
 * NOTE: basic bounding volume declaration file
 */

#ifndef _BOUNDING_VOLUME_H_INCLUDED__
#define _BOUNDING_VOLUME_H_INCLUDED__
#pragma once

#include "../../../math/math_def.h"
#include "../../phys_object/phys_object.h"

/* Physics namespace */
namespace phys
{
  /* Bounding volume type enum */
  enum struct bounding_volume_type
  {
    SPHERE,
    BOX,
    PLANE
  }; /* End of 'bounding_volume_type' enumeration */

  /* Bounding volume class */
  class BoundingVolume
  {
  private:
    /* The physic object */
    PhysObject &_body;
    /* The offset of primitive due to physic object */
    math::Matr4f _offset;

  public:
    /* Default class constructor */
    BoundingVolume(void) = default;

    /* Class constructor */
    BoundingVolume(PhysObject &Obj) : _body(Obj)
    {
    } /* End of constructor */

    /* Class constructor */
    BoundingVolume(PhysObject &Obj, math::Matr4f &Matr) : _body(Obj), _offset(Matr)
    {
    } /* End of constructor */

    /* Virtual function of colliding to bounding volumes */
    virtual bool isCollide(const BoundingVolume *Volume) const
    {
      return false;
    } /* End of 'isCollide' function */
  }; /* End of 'BoundingVolume' class */
}; /* End of 'phys' namespace */

#endif /* _BOUNDING_VOLUME_H_INCLUDED__ */

/* END OF 'bounding_volume.h' FILE */
