/* 
 * Game project 
 * 
 * FILE: main.cpp 
 * AUTHORS:
 * Lebed Pavel
 * LAST UPDATE: 07.03.2018 
 * NOTE: math definitions module 
 */

#ifndef _MATH_DEF_H_INCLUDED__
#define _MATH_DEF_H_INCLUDED__
#pragma once

#include "matrix.h"
#include "Vector.h"

/* Mathematics's namespace */
namespace math
{
  /// Some useful constants
  const double PI = 3.141592653589793;
  const double PI_2 = 1.57079632679;

  /// Some Types predefinition
  // Vector predifinitions
  using Vec3f = Vector<3, float>;
  using Vec3d = Vector<3, double>;
  // Matrix predifinitions
  using Matr3f = Matrix<3, 3, float>;
  using Matr3d = Matrix<3, 3, double>;

  /// Some useful functions
  /* Integer power during link function */
  constexpr int PowI( int X, int K )
  {
    return K == 0 ? 1 : X * PowI(X, K - 1);
  } /* End of 'PowI' function */
}; /* End of 'math' namespace */

#endif /* _MATH_DEF_H_INCLUDED__ */

/* END OF 'math_def.h' FILE */
