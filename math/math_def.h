/* 
 * Game project 
 * 
 * FILE: main.cpp 
 * AUTHORS:
 *   Lebed Pavel,
 *   Denisov Pavel
 * LAST UPDATE: 22.04.2018 
 * NOTE: math definitions module 
 */

#ifndef _MATH_DEF_H_INCLUDED__
#define _MATH_DEF_H_INCLUDED__
#pragma once

#include <vector>
#include "matrix.h"
#include "vector.h"
#include "quat.h"

/* Mathematics's namespace */
namespace math
{
  /// Some useful constants
  const double PI            = 3.141592653589793;
  const double PI_2          = 1.57079632679;
  const double E             = 2.718281828459045;
  const double Degree2Radian = 0.01745329251994329576;
  const double Radian2Degree = 57.29577951308232087684;
  const double Threshold     = 1e-6;

  /// Some Types predefinition
  // Vector predifinitions
  using Vec2f = Vector<2, float>;
  using Vec3f = Vector<3, float>;
  using Vec3d = Vector<3, double>;
  using Vec4f = Vector<4, float>;
  using Vec4d = Vector<4, double>;
  using Vec4uc = Vector<4, unsigned char>;
  // Matrix predifinitions
  using Matr3f = Matrix<3, 3, float>;
  using Matr3d = Matrix<3, 3, double>;
  using Matr4f = Matrix<4, 4, float>;
  using Matr4d = Matrix<4, 4, double>;
  // Quaternion predefinitions
  using Quatf = Quat<float>;
  using Quatd = Quat<double>;
  // Color predefinitions
  using Colorf = Vector<4, float>;
  using Colord = Vector<4, double>;

  /// Some useful functions
  /* Integer power during link function */
  constexpr int PowI( int X, int K )
  {
    return K == 0 ? 1 : X * PowI(X, K - 1);
  } /* End of 'PowI' function */

  /* Convert degrees to radians function. */
  template<typename Type> Type Deg2Rad(Type AngleInDegree)
  {
    return (Type)(AngleInDegree * Degree2Radian);
  } /* End of 'Deg2Rad' function */

  /* Convert radians to degrees function. */
  template<typename Type> Type Rad2Deg(Type AngleInRadian)
  {
    return (Type)(AngleInRadian * Radian2Degree);
  } /* End of 'Rad2Deg' function */
}; /* End of 'math' namespace */

#endif /* _MATH_DEF_H_INCLUDED__ */

/* END OF 'math_def.h' FILE */
