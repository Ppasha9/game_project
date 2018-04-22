/*
 * Game project
 *
 * FILE: force.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 27.03.2018
 * NOTE: force declaration file
 */

#ifndef _FORCE_H_INCLUDED__
#define _FORCE_H_INCLUDED__
#pragma once

#include "../phys_object/phys_object.h"

/* Physics namespace */
namespace phys
{
  /* Force interface class */
  class Force
  {
  public:
    /* Default class constructor */
    Force(void) = default;

    /* Virtual function for applying force to object */
    virtual void applyForce(PhysObject &Obj, const float Duration) const
    {}

    /* Virtual destructor */
    virtual ~Force(void)
    {}
  }; /* End of 'Force' class */
}; /* End of 'phys' namespace */

#endif /* _FORCE_H_INCLUDED__ */

/* END OF 'force.h' FILE */
