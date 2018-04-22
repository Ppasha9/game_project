/*
 * Game project
 *
 * FILE: gravity.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 27.03.2018
 * NOTE: gravitation force declaration file
 */

#ifndef _GRAVITY_H_INCLUDED__
#define _GRAVITY_H_INCLUDED__
#pragma once

#include "../force.h"

/* Physics namespace */
namespace phys
{
  /* Gravity force class */
  class Gravity : public Force
  {
  private:
    /* Holds the acceleration due to gravity vector */
    math::Vec3f _gravity;

  public:
    /* Static const of gravity acceleration on Earth */
    static const math::Vec3f constEarthGravity;

    /* Default class constructor */
    Gravity(void);

    /* Class constructor */
    Gravity(const math::Vec3f &Vec);

    /* Virtual function for applying force to object */
    virtual void applyForce(PhysObject &Obj, const float Duration) const;

    /* Virtual destructor */
    virtual ~Gravity(void);
  }; /* End of 'Gravity' class */
}; /* End of 'phys' namespace */

#endif /* _GRAVITY_H_INCLUDED__ */

/* END OF 'gravity.h' FILE */
