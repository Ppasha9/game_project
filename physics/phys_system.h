/*
 * Game project
 *
 * FILE: phys_system.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 18.03.2018
 * NOTE: physics system simulation declaration file
 */

#ifndef _PHYS_SYSTEM_H_INCLUDED__
#define _PHYS_SYSTEM_H_INCLUDED__
#pragma once

#include "collision\resolution\collision_resolver.h"

 /* Physics namespace */
namespace phys
{
  /* Physics system class */
  class PhysicsSystem : public ContactDetector, public ContactGenerator, public ContactResolver
  {
  private:

  public:
    /* Default class constructor */
    PhysicsSystem(void) = default;

    /* Register new object function */
    void registerObject(int Id, const bounding_volume_type VolumeType);

    /* Response function */
    void response(float Duration) const;

    /* Class destructor */
    ~PhysicsSystem(void);
  }; /* End of 'PhysicsSystem' class */
}; /* End of 'phys' namespace */

#endif /* _PHYS_SYSTEM_H_INCLUDED__ */

/* END OF 'phys_system.h' FILE */
