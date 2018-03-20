/*
 * Game project
 *
 * FILE: phys_object.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 20.03.2018
 * NOTE: physics object handle declaration file
 */

#ifndef _PHYS_OBJECT_H_INCLUDED__
#define _PHYS_OBJECT_H_INCLUDED__
#pragma once

#include "../../math/math_def.h"

/* Physics namespace */
namespace phys
{
  /* Physics object class */
  class PhysObject
  {
  private:
    /*
     * Holds the inverse of the mass of the rigid body. It is more
     * useful to hold the inverse mass because integration is simpler,
     * and because in real time simulation it is more useful to have
     * bodies with infinite mass (immovable) than zero mass (completely
     * unstable in numerical simulation).
     */
    float _inverseMass;

    /* Holds the linear position of the rigid body in world space. */
    math::Vec3f _position;

    /* Holds the angular orientation of the rigid body in world space. */
    math::Quatf _orientation;

    /* Holds the linear velocity of the rigid body in world space. */
    math::Vec3f _velocity;

    /* Holds the angular velocity, or rotation, or the rigid body in world space. */
    math::Vec3f _rotation;

    /*
     * Holds a transform matrix for converting body space into world
     * space and vice versa. This can be achieved by calling the
     * getPointIn*Space functions.
     */
    math::Matr4f _transformMatrix;

    /* Holds the id of current object in global space */
    int _id;

    /*
     * Holds the inverse of the body’s inertia tensor.
     * As long as the tensor is finite, it will be invertible.
     * The inverse tensor is used for similar reasons as those
     * for the use of inverse mass.
     *
     * The inertia tensor, unlike the other variables that define
     * a rigid body, is given in body space.
    */
    math::Matr3f _inverseInertiaTensor;

    /* Calculating internal data from state data function */
    void calculateDerivedData(void);

  public:
    /* Default class constructor */
    PhysObject(int id);

    /* Setting inverse inertia tensor function */
    void setInertiaTensor(const math::Matr3f &inertiaTensor);
  }; /* End of 'PhysObject' class */
}; /* End of 'phys' namespace */

#endif /* _PHYS_OBJECT_H_INCLUDED__ */

/* END OF 'phys_object.h' FILE */
