/*
  Game project

  FILE: matr.h
  AUTHOR:
    Denisov Pavel
  LAST UPDATE: 06.03.2018
  NOTE: matrix's class module
*/

#ifndef _MATR_H_INCLUDED__
#define _MATR_H_INCLUDED__
#pragma once

/* Mathematics namespace */
namespace math
{
  /* Template matrix class */
  template <typename T>
  class Matr
  {
  public:
    T _a[4][4];

    /* Class constructor */
    Matr(T a00, T a01, T a02, T a03,
         T a10, T a11, T a12, T a13,
         T a20, T a21, T a22, T a23,
         T a30, T a31, T a32, T a33)
    {
      _a[0][0] = a00;
      _a[0][1] = a01;
      _a[0][2] = a02;
      _a[0][3] = a03;

      _a[1][0] = a10;
      _a[1][1] = a11;
      _a[1][2] = a12;
      _a[1][3] = a13;

      _a[2][0] = a20;
      _a[2][1] = a21;
      _a[2][2] = a22;
      _a[2][3] = a23;

      _a[3][0] = a30;
      _a[3][1] = a31;
      _a[3][2] = a32;
      _a[3][3] = a33;
    } /* End of constructor */

    /* Getting the determinant function */
    T det(void) const
    {
    } /* End of 'det' function */
  }; /* End of 'Matr' class */
}; /* End of 'math' namespace */

#endif /* _MATR_H_INCLUDED__ */

/* END OF 'matr.h' FILE */
