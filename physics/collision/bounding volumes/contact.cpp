/*
 * Game project
 *
 * FILE: contact.cpp
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: contact class definition file
 */

#include "bounding_volume.h"

using namespace phys;

/* Default class constructor */
Contact::Contact(void) : _position({ 0, 0, 0 }), _normal({ 0, 0, 0 }), _penetration(0), _basisMatrix(1)
{
} /* End of constructor */

/* Class constructor */
Contact::Contact(const math::Vec3f &Pos, const math::Vec3f &Normal, const float Penetration) : _position(Pos), _normal(Normal), _penetration(Penetration), _basisMatrix(1)
{
} /* End of constructor */

/*
 * Constructs an arbitrary orthonormal basis for the contact.
 * This is stored as a 3x3 matrix, where each vector is a column
 * (in other words the matrix transforms contact space into world
 * space). The x direction is generated from the contact normal,
 * and the y and z directions are set so they are at right angles to
 * it.
 */
void Contact::calculateContactBasis(void)
{
  math::Vec3f contactTangent[2];

  // Check whether the Z axis is nearer to the X or Y axis.
  if (fabs(_normal[0]) > fabs(_normal[1]))
  {
    // Scaling factor to ensure the results are normalized.
    const float s = 1.0F / sqrt(_normal[2] * _normal[2] + _normal[0] * _normal[0]);
    // The new X axis is at right angles to the world Y axis.
    contactTangent[0]._coords[0] = _normal[2] * s;
    contactTangent[0]._coords[1] = 0;
    contactTangent[0]._coords[2] = -_normal[0] * s;
    // The new Y axis is at right angles to the new X and Z axes.
    contactTangent[1]._coords[0] = _normal[1] * contactTangent[0]._coords[0];
    contactTangent[1]._coords[1] = _normal[2] * contactTangent[0]._coords[0] - _normal[0] * contactTangent[0]._coords[2];
    contactTangent[1]._coords[2] = -_normal[1] * contactTangent[0]._coords[0];
  }
  else
  {
    // Scaling factor to ensure the results are normalized.
    const float s = 1.0F / sqrt(_normal[2] * _normal[2] + _normal[1] * _normal[1]);
    // The new X axis is at right angles to the world X axis.
    contactTangent[0]._coords[0] = 0;
    contactTangent[0]._coords[1] = -_normal[2] * s;
    contactTangent[0]._coords[2] = _normal[1] * s;
    // The new Y axis is at right angles to the new X and Z axes.
    contactTangent[1]._coords[0] = _normal[1] * contactTangent[0]._coords[2] - _normal[2] * contactTangent[0]._coords[1];
    contactTangent[1]._coords[1] = -_normal[0] * contactTangent[0]._coords[2];
    contactTangent[1]._coords[2] = _normal[0] * contactTangent[0]._coords[1];
  }

  // Make a matrix from the three vectors.
  _basisMatrix._values[0][0] = _normal[0];
  _basisMatrix._values[1][0] = _normal[1];
  _basisMatrix._values[2][0] = _normal[2];

  _basisMatrix._values[0][1] = contactTangent[0]._coords[0];
  _basisMatrix._values[1][1] = contactTangent[0]._coords[1];
  _basisMatrix._values[2][1] = contactTangent[0]._coords[2];

  _basisMatrix._values[0][2] = contactTangent[1]._coords[0];
  _basisMatrix._values[1][2] = contactTangent[1]._coords[1];
  _basisMatrix._values[2][2] = contactTangent[1]._coords[2];
} /* End of 'calculateContactBasis' function */

/* END OF 'contact.cpp' FILE */
