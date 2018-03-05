/*
  Game project

  FILE: vec.h
  AUTHOR:
    Denisov Pavel
  LAST UPDATE: 06.03.2018
  NOTE: vec's class module
 */

#ifndef _VEC_H_INCLUDED__
#define _VEC_H_INCLUDED__
#pragma once

#include <cmath>
#include "math_def.h"

/* Mathematics's namespace */
namespace math
{
  /* Template vector class */
  template <typename T>
  class Vec
  {
  public:
    /* Vector components */
    T _x, _y, _z;

    /* Default class constructor */
    Vec(void) : _x(), _y(), _z()
    {
    } /* End of constructor */

    /* Class construtor */
    Vec(T _x, T _y, T _z) : _x(_x), _y(_y), _z(_z)
    {
    } /* End of constructor */

    /* Copying constructor */
    Vec(Vec &v) : _x(v._x), _y(v._y), _z(v._z)
    {
    } /* End of constructor */

    /* Getting length of the vector function */
    T length(void) const
    {
      return sqrt(_x * _x + _y * _y + _z * _z);
    } /* End of 'length' function */

    /* Getting squared length of the vector function */
    T squareLength(void) const
    {
      return _x * _x + _y * _y + _z * _z;
    } /* End of 'length' function */

    /* Normalize this vector */
    Vec & normalizing(void)
    {
      T len = length();
      _x /= len;
      _y /= len;
      _z /= len;

      return *this;
    } /* End of 'normalizing' function */

    /* Get the normalized vector */
    Vec normalized(void) const
    {
      T len = length();
      return Vec(_x / len, _y / len, _z / len);
    } /* End of 'normalized' function */

    /* Operator + for vectors */
    Vec operator+(const Vec &v) const
    {
      return Vec(_x + v._x, _y + v._y, _z + v._z);
    } /* End of '+' operator */

    /* Operator += for vectors */
    Vec & operator+=(const Vec &v)
    {
      _x += v._x;
      _y += v._y;
      _z += v._z;

      return *this;
    } /* End of '+=' operator */

    /* Operator - for vectors */
    Vec operator-(const Vec &v) const
    {
      return Vec(_x - v._x, _y - v._y, _z - v._z);
    } /* End of '-' operator */

    /* Operator -= for vectors */
    Vec & operator-=(const Vec &v)
    {
      _x -= v._x;
      _y -= v._y;
      _z -= v._z;

      return *this;
    } /* End of '-=' operator */

    /* Operator ! for vectors (get length) */
    T operator!(void) const
    {
      return length();
    } /* End of '!' operator */

    /* Operator & for vectors (dot product) */
    T operator&(const Vec &v) const
    {
      return _x * v._x + _y * v._y + _z * v._z;
    } /* End of '&' operator */

    /* Operator % for vectors (cross product) */
    Vec operator%(const Vec &v) const
    {
      return Vec(_y * v._z - v._y * _z, v._x * _z - _x * v._z, _x * v._y - v._x * _y);
    } /* End of '%' operator */

    /* Operator * on scalar for vectors */
    Vec operator*(T num) const
    {
      return Vec(_x * num, _y * num, _z * num);
    } /* End of '*' operator */

    /* Operator *= on scalar for vectors */
    Vec & operator*=(T num)
    {
      _x *= num;
      _y *= num;
      _z *= num;

      return *this;
    } /* End of '*=' operator */

    /* Operator / on scalar for vectors */
    Vec operator/(T num) const
    {
      return Vec(_x / num, _y / num, _z / num);
    } /* End of '/' operator */

    /* Operator /= on scalar for vectors */
    Vec & operator/=(T num)
    {
      _x /= num;
      _y /= num;
      _z /= num;

      return *this;
    } /* End of '/=' operator */

    /* Operator = for vectors */
    Vec & operator=(Vec &v)
    {
      _x = v._x;
      _y = v._y;
      _z = v._z;

      return *this;
    } /* End of '=' operator */

    /* Operator == for vectors */
    int operator==(const Vec &v) const
    {
      return
        threshold > fabs(_x - v._x) &&
        threshold > fabs(_y - v._y) &&
        threshold > fabs(_z - v._z);
    } /* End of '==' operator */

    /* Operator != for vectors */
    int operator!=(const Vec &v) const
    {
      return
        threshold < fabs(_x - v._x) ||
        threshold < fabs(_y - v._y) ||
        threshold < fabs(_z - v._z);
    }/* End of 'operator!=' function */

   /* Negate vector (itself) function. */
    Vec & Negate(void)
    {
      _x = -_x;
      _y = -_y;
      _z = -_z;

      return *this;
    } /* End of 'Negate' function */

    /* Multiplication by matrix function. */
    Vec operator*(const Matr<T> &matr) const
    {
      Vec ret(_x * matr.a[0][0] + _y * matr.a[1][0] + _z * matr.a[2][0] + matr.a[3][0],
              _x * matr.a[0][1] + _y * matr.a[1][1] + _z * matr.a[2][1] + matr.a[3][1],
              _x * matr.a[0][2] + _y * matr.a[1][2] + _z * matr.a[2][2] + matr.a[3][2]);

      T w = _x * matr.a[0][3] + _y * matr.a[1][3] + _z * matr.a[2][3] + matr.a[3][3];
      ret /= w;
      return ret;
    } /* End of 'operator*' function */

    /* Multiplication by matrix with assignment function. */
    Vec & operator*=(const Matr<T> &matr)
    {
      T a, b;

      a = _x * matr.a[0][0] + _y * matr.a[1][0] + _z * matr.a[2][0] + matr.a[3][0];
      b = _x * matr.a[0][1] + _y * matr.a[1][1] + _z * matr.a[2][1] + matr.a[3][1];
      _z = _x * matr.a[0][2] + _y * matr.a[1][2] + _z * matr.a[2][2] + matr.a[3][2];
      _x = a;
      _y = b;

      return *this;
    } /* End of 'operator*=' function */

    /* Cross product with assignment function. */
    Vec & operator%=(const Vec &v)
    {
      T a, b;

      a = _y * v._z - v._y * _z;
      b = v._x * _z - _x * v._z;
      _z = _x * v._y - v._x * _y;
      _x = a;
      _y = b;

      return *this;
    } /* End of 'operator%=' function */

    /* Get vector coordinate reference by index function */
    T & operator[](int index)
    {
      return *(&_x + index);
    } /* End of 'operator[]' function */

    /*** Vector transformation functions ***/

    /***
     * Apply specified transformation to vector (self-transform) functions
     ***/

     /* Translate vector function. */
    Vec & translate(T dx, T dy, T dz)
    {
      _x += dx;
      _y += dy;
      _z += dz;

      return *this;
    } /* End of 'translate' function */

    /* Translate vector function. */
    Vec & translate(const Vec &shift)
    {
      return translate(shift._x, shift._y, shift._z);
    } /* End of 'translate' function */
  }; /* End of 'Vec' class */
}; /* End of 'math' namespace */

#endif /* _VEC_H_INCLUDED__ */

/* END OF 'vec.h' FILE */
