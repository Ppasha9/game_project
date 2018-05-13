/*
 * Game project
 *
 * FILE: phys_object.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: physics object handle declaration file
 */

#ifndef _PHYS_OBJECT_H_INCLUDED__
#define _PHYS_OBJECT_H_INCLUDED__
#pragma once

#include "../../math/math_def.h"

/* Physics namespace */
namespace phys
{
  /* Forward declaration */
  class Force;

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

    /* Holds the linear acceleration of the rigid body in world space */
    math::Vec3f _acceleration;

    /* Holds the angular velocity, or rotation, or the rigid body in world space. */
    math::Vec3f _rotation;

    /*
     * Holds the inverse of the body’s inertia tensor.
     * As long as the tensor is finite, it will be invertible.
     * The inverse tensor is used for similar reasons as those
     * for the use of inverse mass.
     *
     * The inertia tensor, unlike the other variables that define a rigid body, is given in body space.
     */
    math::Matr3f _inverseInertiaTensor;
    /* Holds inverse inetria tensor in world space for integration */
    math::Matr3f _iitWorld;

    /* Holds the force accumulator for physics integration */
    math::Vec3f _forceAccum;
    /* Holds the torque accumulator for physics integration */
    math::Vec3f _torqueAccum;

    /*
     * Holds the amount of damping applied to angular motion.
     * Damping is required to remove energy added through numerical instability in the integrator.
     */
    float _angularDamping;

    /* Holds the amount of damping applied to linear motion. */
    float _linearDamping;

  private:
    /* Calculating internal data from state data function */
    void calculateDerivedData(void);

    /* Adding force to a point of object function */
    void addForceAtPoint(const math::Vec3f &Force, const math::Vec3f &Point);

  public:
    /* Class constructor */
    PhysObject(const math::Vec3f &Pos, const float InverseMass, const float LinDamping, const float AngDamping);

    /* Setting inverse inertia tensor function */
    void setInertiaTensor(const math::Matr3f &InertiaTensor);

    /*
     * Adds the given force to the given point on the rigid body.
     * The direction of the force is given in world coordinates,
     * but the application point is given in body space. This is
     * useful for spring forces, or other forces fixed to the body.
     */
    void addForceAtBodyPoint(const math::Vec3f &Force, const math::Vec3f &Point);

    /* Adding force to center mass of object function */
    void addForce(const math::Vec3f &Force);

    /* Adding torque to body function. */
    void addTorque(const math::Vec3f &Torque);

    /* Adding impulse function */
    void addImpulse(const math::Vec3f &Impulse);

    /* Clear accumulators function */
    void clearAccums(void);

    /* Integration function */
    void integrate(float Duration);

    /* Getting point of body in world space function */
    math::Vec3f getPointInWorldSpace(const math::Vec3f &Point) const;

    /* Does the object have finite mass or not */
    bool hasFiniteMass(void) const;

    /* Getting object mass function */
    float getMass(void) const;

    /* Getting object inverse mass function */
    float getInverseMass(void) const;

    /* Getting angular damping function */
    float getAngDamping(void) const;

    /* Getting linear damping function */
    float getLinDamping(void) const;

    /* Getting object position function */
    math::Vec3f getPos(void) const;

    /* Getting inverse inertia tensor in world coordinates function */
    math::Matr3f getIITWorld(void) const;

    /* Getting inverse inertia tensor in local coordinates function */
    math::Matr3f getInverseInertia(void) const;

    /* Getting rotation vector function */
    math::Vec3f getRotation(void) const;

    /* Getting velocity vector function */
    math::Vec3f getVelocity(void) const;

    /* Adding to velocity function */
    void addVelocity(const math::Vec3f &AddVel);

    /* Adding to rotation function */
    void addRotation(const math::Vec3f &AddRot);

    /* Adding to position function */
    void addPosition(const math::Vec3f &AddPos);

    /* Setting object inverse mass function */
    /* NOTE: it can throw an exception */
    void setInverseMass(const float InverseMass);

    /* Reseting all parametrs function */
    void reset(const math::Vec3f &Pos);

    /* Setting linear damping function */
    void setLinearDamping(const float LinearDamping);

    /* Setting angular damping function */
    void setAngularDamping(const float AngularDamping);

    /* Getting transformation matrix of object for rendering function */
    math::Matr4f getTransormMatrix(void) const;

    /* Applying new force function */
    void applyForce(const Force *Force, const float Duration);
  }; /* End of 'PhysObject' class */
}; /* End of 'phys' namespace */

#endif /* _PHYS_OBJECT_H_INCLUDED__ */

/* END OF 'phys_object.h' FILE */
