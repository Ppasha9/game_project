/*
 * Game project
 *
 * FILE: quat.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 20.03.2018
 * NOTE: quaternion calculation module
 */

#ifndef _QUAT_H_INCLUDED__
#define _QUAT_H_INCLUDED__
#pragma once

#include <algorithm>
#include "vector.h"

/* Mathematics namespace */
namespace math
{
  /* Template quaternion class definition */
  template <typename Type>
  class Quat
  {
  public:
    union
    {
      /* Holds the data in structure way */
      struct
      {
        /* Real complex component of quaternion */
        Type _r;
        /* Imagine complex components of quaternion */
        Type _i, _j, _k;
      };

      /* Holds the data in array */
      Type _data[4];
    };

  public:
    /* Default class constructor */
    Quat(void) : _r(0), _i(0), _j(0), _k(0)
    {
    } /* End of constructor */

    /* Copying constructor */
    Quat(const Quat &Q) : _r(Q._r), _i(Q._i), _j(Q._j), _k(Q._k)
    {
    } /* End of constructor */

    /* Class component constructor */
    Quat(Type r, Type i, Type j, Type k) : _r(r), _i(i), _j(j), _k(k)
    {
    } /* End of constructor */

    /* Operator '=' for quaternions */
    inline Quat & operator=(const Quat &Q)
    {
      _r = Q._r;
      _i = Q._i;
      _j = Q._j;
      _k = Q._k;

      return *this;
    } /* End of '=' operator */

    /* Normalizing current quaternion function */
    inline void normalize(void)
    {
      Type d = _r * _r + _i * _i + _j * _j + _k * _k;

      /* Check for zero length quaternion, and use non-rotation quaternion in that case */
      if (d == 0)
      {
        r = 1;
        return;
      }

      d = ((Type)1.0) / sqrt(d);
      _r *= d;
      _i *= d;
      _j *= d;
      _k *= d;
    } /* End of 'normalize' function */

    /* Operator *= for quaternions */
    void operator*=(const Quat &Mult)
    {
      Quat q = *this;
      _r = q._r * Mult._r - q._i * Mult._i - q._j * Mult._j - q._k * Mult._k;
      _i = q._r * Mult._i + q._i * Mult._r - q._j * Mult._k - q._k * Mult._j;
      _j = q._r * Mult._j - q._i * Mult._k + q._j * Mult._r - q._k * Mult._i;
      _k = q._r * Mult._k + q._i * Mult._j - q._j * Mult._i + q._k * Mult._r;
    } /* End of '*=' operator */

    /*
     * Adds the given vector to this, scaled by the given amount.
     * This is used to update the orientation quaternion by a rotation
     * and time.
     */
    void addScaledVector(const Vector<3, Type> &Vec, Type scale)
    {
      Quat q(0, Vec[0] * scale, Vec[1] * scale, Vec[2] * scale);
      q *= *this;
      _r += q._r * ((Type)0.5);
      _i += q._i * ((Type)0.5);
      _j += q._j * ((Type)0.5);
      _k += q._k * ((Type)0.5);
    } /* End of 'addScaledVector' function */
  }; /* End of 'Quat' class */
}; /* End of 'math' namespace */

#endif /* _QUAT_H_INCLUDED__ */

/* END OF 'quat.h' FILE */
