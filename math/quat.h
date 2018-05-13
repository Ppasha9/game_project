/*
 * Game project
 *
 * FILE: quat.h
 * AUTHORS:
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018
 * NOTE: quaternion calculation module
 */

#ifndef _QUAT_H_INCLUDED__
#define _QUAT_H_INCLUDED__
#pragma once

#include <algorithm>
#include "vector.h"

#ifndef _DEBUG
#define __MATH_ASM__
#endif

/* Mathematics namespace */
namespace math
{
  /* Template quaternion class definition */
  template <typename Type>
  class Quat
  {
  public:
    /* Vector part */
    Vector<3, Type> _vec;
    /* Scalar part */
    Type _scalar;

    /* Default constructor. */
    Quat(void) : _scalar(1), _vec(0)
    {
    } /* End of constructor */

    /* Reset transformation quaternion data function. */
    inline Quat & setUnit(void)
    {
      _scalar = 1;
      _vec = Vector<3, Type>(0);
      return *this;
    } /* End of 'setUnit' function */

    /* Constructor by parts. */
    Quat(const Vector<3, Type> &NewV, Type NewS) : _vec(NewV), _scalar(NewS)
    {
    } /* End of constructor */

    /* Constructor by rotation axis and angle. */
    Quat(Type AngleInDegree, const Vector<3, Type> &Axis)
    {
      double halfAngleInRadians = Deg2Rad(AngleInDegree / 2);
      Type sinVal, cosVal;

#ifdef __MATH_ASM__
      _asm {
        /* FST(0) Angle (from degree to radian) */
        fld  halfAngleInRadians

        /* FST(0) - cos, FST(1) - sin */
        fsincos

        fstp cosVal    /* cos -> cosVal */
        fstp sinVal    /* sin -> sinVal */
      }
#else /* __MATH_ASM__ */
      sinVal = sin(halfAngleInRadians);
      cosVal = cos(halfAngleInRadians);
#endif /* __MATH_ASM__ */

      _scalar = cosVal;
      _vec = Axis.getNormalized() * sinVal;
    } /* End of constructor */

    /* Evaluation quaternion square function. */
    inline Type getSquare(void) const
    {
      return _scalar * _scalar + _vec.norm();
    } /* End of 'getSquare' function */

    /* Comparing of two quaternions function. */
    inline bool operator==(const Quat &Q) const
    {
      return Threshold > fabs(_scalar - Q._scalar) && Threshold > fabs((_vec - Q._vec).length());
    } /* End of '==' operator */

    /* Comparing of two quaternions function. */
    inline bool operator!=(const Quat &Q) const
    {
      return Threshold < fabs(_scalar - Q._scalar) || Threshold < fabs((_vec - Q._vec).length());
    } /* End of '!=' operator */

    /* Per components addition of two quaternions function. */
    inline Quat operator+(const Quat &Q) const
    {
      return Quat(_vec + Q._vec, _scalar + Q._scalar);
    } /* End of '+' operator */

    /* Per components subtraction of two quaternions function. */
    inline Quat operator-(const Quat &Q) const
    {
      return Quat(_vec - Q._vec, _scalar - Q._scalar);
    } /* End of '-' operator */

    /* Quaternion multiplication by number function. */
    inline Quat operator*(Type A) const
    {
      return Quat(_vec * A, _scalar * A);
    } /* End of '*' operator */

    /* Multiplication of two quaternions function. */
    inline Quat operator*(const Quat &Q) const
    {
      return Quat(_vec * Q._scalar + Q._vec * _scalar + (_vec & Q._vec), _scalar * Q._scalar - (_vec * Q._vec));
    } /* End of '*' operator */

    /* Per components addition of quaternion with assignment function. */
    inline Quat & operator+=(const Quat &Q)
    {
      _scalar += Q._scalar;
      _vec += Q._vec;

      return *this;
    } /* End of '+=' operator */

    /* Per components subtraction of quaternion with assignment function. */
    inline Quat & operator-=(const Quat &Q)
    {
      _scalar -= Q._scalar;
      _vec -= Q._vec;

      return *this;
    } /* End of '-=' operator */

    /* Multiplication with assignment by number function. */
    inline Quat & operator*=(Type A)
    {
      _scalar *= A;
      _vec *= A;

      return *this;
    } /* End of '*=' operator */

    /* Multiplication of quaternion with assignment function. */
    inline Quat & operator*=(const Quat &Q)
    {
      Type tmp = _scalar;
      _scalar = _scalar * Q._scalar - (_vec * Q._vec);
      _vec = _vec * Q._scalar + Q._vec * tmp + (_vec & Q._vec);

      return *this;
    } /* End of '*=' operator */

    /* Normalize quaternion function. */
    inline Quat & normalize(void)
    {
      _vec.normalize();
      return *this;
    } /* End of 'normalize' function */

    /* Getting normalized quaternion function. */
    inline Quat getNormalized(void) const
    {
      return Quat(_vec.getNormalized(), _scalar);
    } /* End of 'getNormalized' function */

    /* Negate quaternion function. */
    inline Quat & negate(void)
    {
      _vec = -_vec;
      return *this;
    } /* End of 'conjugate' function */

    /* Getting negated quaternion function. */
    inline Quat getNegated(void) const
    {
      return Quat(-_vec, _scalar);
    } /* End of 'getNegated' function */

    /* Inverse quaternion function. */
    inline Quat & inverse(void)
    {
      return negate() * (1 / getSquare());
    } /* End of 'Inverse' function */

    /* Getting inversed quaternion function. */
    inline Quat getInversed(void) const
    {
      return Quat(-_vec * (1 / getSquare()), _scalar * (1 / getSquare()));
    } /* End of 'getInversed' function */

    /* Transform quaternion to matrix function. */
    inline Matrix<3, 3, Type> toMatr3x3(void) const
    {
      float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
      float s = 2.0f / getSquare();
      x2 = _vec[0] * s;    y2 = _vec[1] * s;    z2 = _vec[2] * s;
      xx = _vec[0] * x2;   xy = _vec[0] * y2;   xz = _vec[0] * z2;
      yy = _vec[1] * y2;   yz = _vec[1] * z2;   zz = _vec[2] * z2;
      wx = _scalar * x2;   wy = _scalar * y2;   wz = _scalar * z2;

      math::Matrix<3, 3, Type> res;
      res.setIdentity();

      res._values[0][0] = 1.0f - (yy + zz);
      res._values[1][0] = xy - wz;
      res._values[2][0] = xz + wy;

      res._values[0][1] = xy + wz;
      res._values[1][1] = 1.0f - (xx + zz);
      res._values[2][1] = yz - wx;

      res._values[0][2] = xz - wy;
      res._values[1][2] = yz + wx;
      res._values[2][2] = 1.0f - (xx + yy);

      return res;
    } /* End of 'toMatr3x3' function */

    /* Transform quaternion to matrix function. */
    inline Matrix<4, 4, Type> toMatr4x4(void) const
    {
      float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
      float s = 2.0f / getSquare();
      x2 = _vec[0] * s;    y2 = _vec[1] * s;    z2 = _vec[2] * s;
      xx = _vec[0] * x2;   xy = _vec[0] * y2;   xz = _vec[0] * z2;
      yy = _vec[1] * y2;   yz = _vec[1] * z2;   zz = _vec[2] * z2;
      wx = _scalar * x2;   wy = _scalar * y2;   wz = _scalar * z2;

      math::Matrix<4, 4, Type> res;
      res.setIdentity();

      res._values[0][0] = 1.0f - (yy + zz);
      res._values[1][0] = xy - wz;
      res._values[2][0] = xz + wy;

      res._values[0][1] = xy + wz;
      res._values[1][1] = 1.0f - (xx + zz);
      res._values[2][1] = yz - wx;

      res._values[0][2] = xz - wy;
      res._values[1][2] = yz + wx;
      res._values[2][2] = 1.0f - (xx + yy);

      res._values[3][3] = 1;

      return res;
    } /* End of 'toMatr4x4' function */

    /* Vector rotate with quaternion function. */
    inline Vector<3, Type> rotateVector(Vector<3, Type> &Vec) const
    {
      return Vec = (getInversed() * Quat(Vec, 0) * (*this))._vec;
    } /* End of 'rotateVector' function */

    /* Static function for spherical linear interpolation. */
    inline static Quat slerp(const Quat &Q1, const Quat &Q2, Type T)
    {
      Type phi = acos(fabs(Q1._scalar * Q2._scalar + (Q1._vec * Q2._vec))),
        sinPhi = sqrt(1 - fabs(Q1._scalar * Q2._scalar + (Q1._vec * Q2._vec)) * fabs(Q1._scalar * Q2._scalar + (Q1._vec * Q2._vec))),
        sinPhiT = sin(phi * T), sinPhi1T = sin(phi * (1 - T));

      return Q1 * (sinPhi1T / sinPhi) + Q2 * (sinPhiT / sinPhi);
    } /* End of 'slerp' function */

    /*
     * Adds the given vector to this, scaled by the given amount.
     * This is used to update the orientation quaternion by a rotation and time.
     */
    inline void addScaledVector(const Vector<3, Type> &Vector, Type Scale)
    {
      Quat q(Vector * Scale, 0);
      q *= *this;
      _scalar += q._scalar * (Type)0.5;
      _vec += q._vec * (Type)0.5;
    } /* End of 'addScaledVector' function */
  }; /* End of 'Quat' class */
}; /* End of 'math' namespace */

#endif /* _QUAT_H_INCLUDED__ */

/* END OF 'quat.h' FILE */
