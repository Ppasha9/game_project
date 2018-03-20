/*
 * Game project
 *
 * FILE: phys_object.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 20.03.2018
 * NOTE: physics object handle definition file
 */

#include "phys_object.h"

/*
 * Inline function that creates the transform matrix
 * from position and orientation.
 */
static inline void s_calculateTransformMatrix(math::Matr4f &TransformMatr, const math::Vec3f &Pos, const math::Quatf &Orientation)
{
  TransformMatr._values[0][0] = 1 - 2 * Orientation._j * Orientation._j - 2 * Orientation._k * Orientation._k;
  TransformMatr._values[0][1] = 2 * Orientation._i * Orientation._j - 2 * Orientation._r * Orientation._k;
  TransformMatr._values[0][2] = 2 * Orientation._i * Orientation._k + 2 * Orientation._r * Orientation._j;
  TransformMatr._values[0][3] = Pos[0];
  TransformMatr._values[1][0] = 2 * Orientation._i * Orientation._j + 2 * Orientation._r * Orientation._k;
  TransformMatr._values[1][1] = 1 - 2 * Orientation._i * Orientation._i - 2 * Orientation._k * Orientation._k;
  TransformMatr._values[1][2] = 2 * Orientation._j * Orientation._k - 2 * Orientation._r * Orientation._i;
  TransformMatr._values[1][3] = Pos[1];
  TransformMatr._values[2][0] = 2 * Orientation._i * Orientation._k - 2 * Orientation._r * Orientation._j;
  TransformMatr._values[2][1] = 2 * Orientation._j * Orientation._k + 2 * Orientation._r * Orientation._i;
  TransformMatr._values[2][2] = 1 - 2 * Orientation._i * Orientation._i - 2 * Orientation._j * Orientation._j;
  TransformMatr._values[2][3] = Pos[2];
} /* End of 's_calculateTransformMatrix' function */

/* Calculating internal data from state data function */
void phys::PhysObject::calculateDerivedData(void)
{
  /* Calulate the transform matrix */
  s_calculateTransformMatrix(_transformMatrix, _position, _orientation);
} /* End of 'calculateDerivedData' function */

/* Setting inverse inertia tensor function */
void phys::PhysObject::setInertiaTensor(const math::Matr3f &inertiaTensor)
{
  _inverseInertiaTensor = math::Matr3f(inertiaTensor.getInverse());
} /* End of 'setInertiaTensor' function */

/* END OF 'phys_object.cpp' FILE */
