/* 
 * Game project 
 * 
 * FILE: Vector.h 
 * AUTHORS:
 * Lebed Pavel
 * LAST UPDATE: 07.03.2018 
 * NOTE: Vector calculation module 
 */

#pragma once

#include <algorithm>
#include <initializer_list>

/* Mathematics's namespace */
namespace math
{
  template <size_t Metrix, typename Type>
  class Vector
  {
  private:
    // Coordinate in Metrix size space
    Type _coords[Metrix];
  public:
    Vector( void ) = default;

    /* Constructor with all default values */
    Vector( Type Def )
    {
      for (size_t i = 0; i < Metrix; i++)
        _coords[i] = Def;
    } /* End of 'Vector' function */

    /* Constructor with all set params */
    Vector( const std::initializer_list<Type> &Init )
    {
      std::copy(Init.begin(), Init.end(), _coords);
    } /* End of 'Vector' function */

    /* Sum two Vectors */
    inline Vector operator+( const Vector &Vec ) const
    {
      Vector res;

      for (size_t i = 0; i < Metrix; i++)
        res._coords[i] = _coords[i] + Vec._coords[i];
      return res;
    } /* End of 'operator+' function */

    /* Negate two Vectors */
    inline Vector operator-( const Vector &Vec ) const
    {
      Vector res;

      for (size_t i = 0; i < Metrix; i++)
        res._coords[i] = _coords[i] - Vec._coords[i];
      return res;
    } /* End of 'operator-' function */

    /* Sum two Vectors in self */
    inline Vector & operator+=( const Vector &Vec )
    {
      for (size_t i = 0; i < Metrix; i++)
        _coords[i] += Vec._coords[i];
      return *this;
    } /* End of 'operator+=' function */

    /* Negate two Vectors in self */
    inline Vector & operator-=( const Vector &Vec )
    {
      for (size_t i = 0; i < Metrix; i++)
        _coords[i] -= Vec._coords[i];
      return *this;
    } /* End of 'operator-=' function */

    /* Negate Vector */
    inline Vector operator!( void ) const
    {
      Vector res;

      for (size_t i = 0; i < Metrix; i++)
        res._coords[i] = -(_coords[i]);
      return res;
    } /* End of 'operator!' function */

    /* Second norm calculate */
    inline Type norm( void ) const
    {
      return *this * *this;
    } /* End of 'norm' function */

    /* Vector set normalized */
    inline void normalize( void )
    {
      Type len = length();

      if (len == 0)
        return;

      for (size_t i = 0; i < Metrix; i++)
        _coords[i] /= len;
    } /* End of 'normalize' function */

    /* Length calculate */
    inline Type length( void ) const
    {
      return sqrt(norm());
    } /* End of 'length' function */

    /* Scalar multiply */
    inline Type operator*( const Vector &Vec ) const
    {
      Type res = 0;

      for (size_t i = 0; i < Metrix; i++)
        res += _coords[i] * Vec._coords[i];
      return res;
    } /* End of 'operator*' function */

    /* Number multiply */
    inline Vector operator*( Type Num ) const
    {
      Vector res;

      for (size_t i = 0; i < Metrix; i++)
        res = _coords[i] * Num;
      return res;
    } /* End of 'operator*' function */

    /* Number multiply in self */
    inline void operator*=( Type Num )
    {
      for (size_t i = 0; i < Metrix; i++)
        _coords[i] *= Num;
    } /* End of 'operator*=' function */

    /* Cross multiply */
    inline Vector operator&( const Vector &Vec );

    /* Clumb Vector values */
    inline void clump( Type Min, Type Max )
    {
      for (size_t i = 0; i < Metrix; i++)
        if (_coords[i] < Min)
          _coords[i] = Min;
        else if (_coords[i] > Max)
          _coords[i] = Max;
    } /* End of 'clump' function */

    /* Value get */
    inline Type operator[]( size_t Id ) const
    {
      // TO_DO Assert or exception for user not hit
      return _coords[Id];
    } /* End of 'operator[]' function */
  };

  /* Cross multiply */
  template <>
  inline Vector<3, float> Vector<3, float>::operator&( const Vector &Vec )
  {
    return Vector{_coords[1] * Vec._coords[2] - _coords[2] * Vec._coords[1],
                  _coords[2] * Vec._coords[0] - _coords[0] * Vec._coords[2],
                  _coords[0] * Vec._coords[1] - _coords[1] * Vec._coords[0]};
  } /* End of 'Vector<3, float>::operator&' function */
  
  /* Cross multiply */
  template <>
  inline Vector<3, double> Vector<3, double>::operator&( const Vector &Vec )
  {
    return Vector{_coords[1] * Vec._coords[2] - _coords[2] * Vec._coords[1],
                  _coords[2] * Vec._coords[0] - _coords[0] * Vec._coords[2],
                  _coords[0] * Vec._coords[1] - _coords[1] * Vec._coords[0]};
  } /* End of 'Vector<3, float>::operator&' function */
}; /* End of 'math' namespace */

/* END OF 'vec.h' FILE */
