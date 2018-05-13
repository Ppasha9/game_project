/* 
 * Game project 
 * 
 * FILE: matrix.h 
 * AUTHORS:
 *   Lebed Pavel
 * LAST UPDATE: 22.04.2018 
 * NOTE: matrix calculation module 
 */

#pragma once

#include <cstring>
#include <initializer_list>

#include "vector.h"

/* Mathematics namespace */
namespace math
{
#define MAX(A, B) ((A) < (B) ? (B) : (A))

  /* Template matrix class */
  template <size_t Rows, size_t Columns, typename Type>
  class Matrix
  {
  public:
    Type _values[Rows][Columns];
  public:
    /* Constrictor with no initialize */
    Matrix() = default;

    /* Constrictor as diagonal matrix */
    Matrix( Type DiagValue )
    {
      setDiag(DiagValue);
    } /* End of 'Matrix' function */

    Matrix( const std::initializer_list<Type> &Init )
    {
      auto It = Init.begin();
      for (size_t i = 0; i < Rows; i++)
        for (size_t j = 0; j < Columns; j++)
        {
          _values[i][j] = *It;
          It++;
        }
    } /* End of 'Matrix' function */

    /* Set matrix as diagonal */
    inline Matrix & setDiag( Type DiagValue )
    {
      setZero();

      size_t max = MAX(Rows, Columns);
      for (size_t i = 0; i < max; i++)
        _values[i][i] = DiagValue;

      return *this;
    } /* End of 'setDiag' function */

    inline void setZero( void )
    {
      // Quick zero initialize
      std::memset(_values, 0, sizeof(Type) * Rows * Columns);
    } /* End of 'setZero' function */

    /* Set matrix as identity */
    inline Matrix & setIdentity( void )
    {
      setDiag(1);

      return *this;
    } /* End of 'setIdentity' function */

    /* Set to diagonal using standart operations */
    inline void setDiag( void )
    {
      if (Rows != Columns)
        return;

      // Gauss method get to diagonal
      for (size_t i = 0; i < Columns; i++)
      {
        for (size_t j = 0; j < Rows; j++)
          if (j != i)
          {
            Type div = _values[j][i] / _values[i][i];
            for (size_t k = 0; k < Columns; k++)
              _values[j][k] -= _values[i][k] * div;
            _values[j][i] = 0;
          }
      }
    } /* End of 'SetDiag' function */

    /* Determinant calcualte */
    inline Type Det( void ) const
    {
      if (Rows != Columns)
        return 0;
      Matrix matr(*this);

      // Gauss metod
      matr.setDiag();
      
      Type res = 1;
      for (size_t j = 0; j < Rows; j++)
        res *= matr._values[j][j];
      return res;
    } /* End of 'Det' function */

    /* Set matrix like the inverse of  */

    /* Matrix multiply */
    template <size_t Column2>
    inline Matrix<Rows, Column2, Type> operator*( const Matrix<Columns, Column2, Type> &Matr ) const
    {
      Matrix res;
      
      res.setZero();
      for (size_t i = 0; i < Rows; i++)
        for (size_t j = 0; j < Column2; j++)
          for (size_t k = 0; k < Columns; k++)
            res._values[i][j] += _values[i][k] * Matr._values[k][j];

      return res;
    } /* End of 'operator*' function */

    /* Matrix multiply */
    inline Matrix & operator*=( const Matrix &Matr )
    {
      if (Rows != Columns)
        return *this;

      Matrix copy(*this);
      setZero();
      // Want to do operation with self
      const Matrix *matr = &Matr;
      if (matr == this)
         matr = &copy;

      for (size_t i = 0; i < Rows; i++)
        for (size_t j = 0; j < Columns; j++)
          for (size_t k = 0; k < Columns; k++)
            _values[i][j] += copy._values[i][k] * matr->_values[k][j];

      return *this;
    } /* End of 'operator*' function */

    inline Vector<Rows, Type> operator*( const Vector<Rows, Type> &Vec ) const
    {
      if (Rows != Columns)
        return Vector<Rows, Type>(0);

      Vector<Rows, Type> res(0);
      for (size_t i = 0; i < Rows; i++)
        for (size_t j = 0; j < Rows; j++)
          res._coords[i] += _values[i][j] * Vec._coords[j];
      return res;
    } /* End of 'operator*' function */

    inline Matrix getInverse( void ) const
    {
      if (Rows != Columns)
        return Matrix(1);

      // Gauss method get to diagonal
      Matrix res(1); // Identity matrix create
      Matrix copy(*this);
      for (size_t i = 0; i < Columns; i++)
      {
        // Divide current row
        Type div = copy._values[i][i];
        for (size_t k = 0; k < Columns; k++)
        {
          copy._values[i][k] /= div;
          // Do same to identity
          res._values[i][k] /= div;
        }
        copy._values[i][i] = 1;
        // Set zeroz
        for (size_t j = 0; j < Rows; j++)
          if (j != i)
          {
            Type div = copy._values[j][i];
            for(size_t k = 0; k < Columns; k++)
            {
              copy._values[j][k] -= copy._values[i][k] * div;
              // Do same to identity
              res._values[j][k] -= res._values[i][k] * div;
            }
          }
      }
      return res;
    } /* End of 'getInverse' function */

    inline void inversize( void )
    {
      *this = getInverse();
    } /* End of 'inversize' function */

    inline Matrix<Columns, Rows, Type> getTranspose(void) const
    {
      Matrix<Columns, Rows, Type> res;

      for (size_t j = 0; j < Rows; j++)
        for (size_t i = 0; i < Columns; i++)
          res._values[i][j] = _values[j][i];
      return res;
    } /* End of 'getTranspose' function */

    inline static Matrix<Rows, Columns, Type> getScale(const Vector<Rows, Type> &Svec)
    {
      Matrix<Columns, Rows, Type> res;                
      res.setZero();

      for (size_t j = 0; j < Rows; j++)
          res._values[j][j] = Svec[j];
      return res;
    } /* End of 'getTranspose' function */

    inline static Matrix<4, 4, Type> getRotateX(double Angle, bool IsDegree = true)
    {
      Matrix<4, 4, Type> res(1);
      double rad = Angle;

      if (IsDegree)
        rad = Degree2Radian(Angle);
      double cosa = cos(rad), sina = sin(rad);

      res._values[1][1] = cosa;
      res._values[1][2] = -sina;
      res._values[2][1] = sina;
      res._values[2][2] = cosa;
      return res;
    } /* End of 'getRotateX' function */

    inline static Matrix<4, 4, Type> getRotateY(double Angle, bool IsDegree = true)
    {
      Matrix<4, 4, Type> res(1);
      double rad = Angle;

      if (IsDegree)
        rad = Degree2Radian(Angle);
      double cosa = cos(rad), sina = sin(rad);

      res._values[0][0] = cosa;
      res._values[0][2] = sina;
      res._values[2][0] = -sina;
      res._values[2][2] = cosa;
      return res;
    } /* End of 'getRotateY' function */

    inline static Matrix<4, 4, Type> getRotateZ(double Angle, bool IsDegree = true)
    {
      Matrix<4, 4, Type> res(1);
      double rad = Angle;

      if (IsDegree)
        rad = Degree2Radian(Angle);
      double cosa = cos(rad), sina = sin(rad);

      res._values[0][0] = cosa;
      res._values[0][1] = -sina;
      res._values[1][0] = sina;
      res._values[1][1] = cosa;
      return res;
    } /* End of 'getRotateZ' function */

    /* Setting a translate matrix by vector function */
    inline static Matrix<4, 4, Type> getTranslate(const Vector<3, Type> &Vec)
    {
      Matrix<4, 4, Type> res(1);
      res.setDiag(1);
      res._values[3][0] = Vec[0];
      res._values[3][1] = Vec[1];
      res._values[3][2] = Vec[2];
      return res;
    } /* End of 'getTranslate' function */

    /* Setting a translate matrix by components function */
    inline static Matrix<4, 4, Type> getTranslate(const Type Dx, const Type Dy, const Type Dz)
    {
      Matrix<4, 4, Type> res(1);
      res.setDiag(1);
      res._values[3][0] = Dx;
      res._values[3][1] = Dy;
      res._values[3][2] = Dz;
      return res;
    } /* End of 'getTranslate' function */
  }; /* End of 'Matrix' class */

  /// Determinant realizations
  /* Determinant calcualte */
  template <>
  inline float Matrix<2, 2, float>::Det( void ) const
  {
    return _values[0][0] * _values[1][1] -
           _values[0][1] * _values[1][0];
  } /* End of 'Matrix<2, 2, float>::Det' function */

  /* Determinant calcualte */
  template <>
  inline double Matrix<2, 2, double>::Det( void ) const
  {
    return _values[0][0] * _values[1][1] -
           _values[0][1] * _values[1][0];
  } /* End of 'Matrix<2, 2, float>::Det' function */
  /* Determinant calcualte */
  template <>
  inline float Matrix<3, 3, float>::Det( void ) const
  {
    return _values[0][0] * _values[1][1] * _values[2][2] -
           _values[0][0] * _values[2][1] * _values[1][2] +
           _values[0][1] * _values[1][2] * _values[2][0] -
           _values[0][1] * _values[2][2] * _values[1][0] +
           _values[0][2] * _values[1][0] * _values[2][1] -
           _values[0][2] * _values[2][0] * _values[1][1];
  } /* End of 'Matrix<2, 2, float>::Det' function */

  /* Determinant calcualte */
  template<>
  inline double Matrix<3, 3, double>::Det( void ) const
  {
    return _values[0][0] * _values[1][1] * _values[2][2] -
           _values[0][0] * _values[2][1] * _values[1][2] +
           _values[0][1] * _values[1][2] * _values[2][0] -
           _values[0][1] * _values[2][2] * _values[1][0] +
           _values[0][2] * _values[1][0] * _values[2][1] -
           _values[0][2] * _values[2][0] * _values[1][1];
  } /* End of 'Matrix<2, 2, float>::Det' function */
}; /* End of 'math' namespace */

/* END OF 'matrix.h' FILE */
