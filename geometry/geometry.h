/*
 * Game project
 *
 * FILE: geometry.h
 * AUTHORS:
 *   Kozlov Ilya
 * LAST UPDATE: 22.04.2018
 * NOTE: geometry handle class declaration
 */

#ifndef _GEOMETRY_H_INCLUDED__
#define _GEOMETRY_H_INCLUDED__
#pragma once

#include <vector>

#include "..\def.h"

/* Geometry namespace */
namespace geom
{
  /* 3D-space vertex struct */
  struct Vertex
  {
    math::Vec3f _pos;
    math::Vec3f _norm;
    math::Vec2f _tex;

    /* Class constructor */
    Vertex( const math::Vec3f &Pos = {0, 0, 0}, const math::Vec3f &Norm = {0, 0, 0}, const math::Vec2f &Tex = { 0, 0 }) : _pos(Pos), _norm(Norm), _tex(Tex)
    {
    } /* End of constructor */
  }; /* End of 'Vertex' structure */

  /* Geometry class */
  class Geom
  {
  private:
    unsigned long _nooV;
    unsigned long _nooI;
    std::vector<unsigned long> _indices;
    std::vector<Vertex> _vertices;

    /* Auto normalize function */
    void autoNormal();

    /* Interpolate from a to b by t function */
    inline float interpolate( float a, float b, float t );

    /* Create plane geom object by pos, two normilized directions and width, height */
    Geom & createPlane( const math::Vec3f & Pos, const math::Vec3f & RightNorm, const math::Vec3f & UpNorm, const math::Vec3f & Norm, float W, float H );
  public:
    /* Default geometry class constructor */
    Geom() = default;

    /*
     * Getters
     */

    /* Number of vertices getter */
    unsigned long getNumOfV() const
    {
      return _nooV;
    } /* End of 'getNumOfV' function */

    /* Number of indices getter */
    unsigned long getNumOfI() const
    {
      return _nooI;
    } /* End of 'getNumOfI' function */

    /* Vertices vector getter */
    std::vector<Vertex> getVertices() const
    {
      return _vertices;
    } /* End of 'getVertices' function */

    /* Indices vector getter */
    std::vector<unsigned long> getIndices() const
    {
      return _indices;
    } /* End of 'getIndices' function */

    /*
     * Triangle
     */

    /* Create triangle geom object by vertices and indices vectors */
    Geom & createTriangle( const std::vector<Vertex> & Vertices, const std::vector<unsigned long> & Indices );

    /* Create triangle geom object by three vertices */
    Geom & createTriangle( const Vertex & V1, const Vertex & V2, const Vertex & V3 );

    /* Create triangle geom object by three vertices positions + autonormal */
    Geom & createTriangle( const math::Vec3f & Pos1, const math::Vec3f & Pos2, const math::Vec3f & Pos3 );

    /*
     * Trimesh
     */

    /* Creating the triemsh geometry by array of vetrices and indices */
    Geom & createTrimesh(const std::vector<Vertex> &Vertices, const std::vector<unsigned long> &Indices);

    /* Creating the triemsh geometry by array of vetrices and indices */
    Geom & createTrimesh(const int NumV, const int NumI, const Vertex *Vertices, const unsigned long *Indices);

    /*
     * Plane
     */

    /* Create plane geom object by three vertices */
    Geom & createPlane( const Vertex & V1, const Vertex & V2, const Vertex & V3, const Vertex & V4 );

    /* Create plane geom object by four vertices positions + autonormal */
    Geom & createPlane( const math::Vec3f & Pos1, const math::Vec3f & Pos2, const math::Vec3f & Pos3, const math::Vec3f & Pos4 );

    /* Create plane geom object by two vertices */
    Geom & createPlane( const Vertex & LeftBottom, const Vertex & RightTop );

    /* Create plane geom object by two vertices positions and normal */
    Geom & createPlane( const math::Vec3f & LeftBottom, const math::Vec3f & RightTop, const math::Vec3f & Norm );

    /* Create plane geom object by pos, two directions and width, height */
    Geom & createPlane( const math::Vec3f & Pos, math::Vec3f & Right, math::Vec3f & Up, float W, float H );

    /*
     * Sphere
     */

    /* Create sphere geom object */
    Geom & createSphere( const math::Vec3f & Center, float R = 1, int W = 5, int H = 4 );

    /*
     * Box
     */

    /* Create box geom object */
    Geom & createBox( const math::Vec3f & Center, float L );

    /*
     * Object
     */

    /* Load geom object from .obj file */
    Geom & loadObj( const string & name );

    /* Geometry class destructor */
    ~Geom()
    {
    } /* End of '~Geom' function */
  }; /* End of 'Geom' class */
} /* End of 'geom' namespace */

#endif /* _GEOMETRY_H_INCLUDED__ */

/* END OF 'geometry.h' FILE */
