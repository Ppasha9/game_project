/*
 * Game project
 *
 * FILE: gravity.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 27.03.2018
 * NOTE: gravitation force definition file
 */

#include "gravity.h"

using namespace phys;

/* Static const of gravity acceleration on Earth */
const math::Vec3f Gravity::constEarthGravity = { 0, -9.8f, 0 };

/* Default class constructor */
Gravity::Gravity(void) : _gravity(constEarthGravity)
{
} /* End of constructor */

/* Class constructor */
Gravity::Gravity(const math::Vec3f &Vec) : _gravity(Vec)
{
} /* End of constructor */

/* Virtual function for applying force to object */
void Gravity::applyForce(PhysObject &Obj, const float Duration) const
{
  if (!Obj.hasFiniteMass())
    return;

  Obj.addForce(_gravity * Obj.getMass());
} /* End of 'applyForce' function */

/* Virtual destructor */
Gravity::~Gravity(void)
{
} /* End of destructor */

/* END OF 'gravity.cpp' FILE */
