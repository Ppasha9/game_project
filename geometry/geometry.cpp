/*
* Game project
*
* FILE: geometry.cpp
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 13.04.2018
* NOTE: geometry handle implementation
*/

#include "geometry.h"

using namespace math;

/* Auto normalize function */
void geom::Geom::autoNormal()
{
  /* Reset all vertex normals */
  for (unsigned long i = 0; i < _nooV; i++)
    _vertices[i]._norm = {0, 0, 0};

  /* Evaluate all facets normals */
  for (unsigned long i = 0; i < _nooI; i += 3)
  {
    /* Obtain normal for first facet */
    Vec3f fn = (_vertices[_indices[i + 1]]._pos - _vertices[_indices[i]]._pos) & (_vertices[_indices[i + 2]]._pos - _vertices[_indices[i]]._pos);

    /* Add normal to every facet vertex */
    _vertices[_indices[i]]._norm += fn;
    _vertices[_indices[i + 1]]._norm += fn;
    _vertices[_indices[i + 2]]._norm += fn;
  }
  /* Normalize vertex normals */
  for (unsigned long i = 0; i < _nooV; i++)
    _vertices[i]._norm.normalize();
} /* End of 'geom::Geom::autoNormal' function */

/* Interpolate from a to b by t function */
inline float geom::Geom::interpolate( float a, float b, float t )
{
  return a * (1 - t) + b * t;
} /* End of 'geom::Geom::interpolate' function */

/* Create triangle geom object by vertices and indices vectors */
geom::Geom & geom::Geom::createTriangle( const std::vector<Vertex> & Vertices, const std::vector<unsigned long> & Indices )
{
  _nooV = _nooI = 3;
  _vertices = Vertices;
  _indices = Indices;

  return *this;
} /* End of 'geom::Geom::createTriangle' function */

/* Create triangle geom object by three vertices */
geom::Geom & geom::Geom::createTriangle( const Vertex & V1, const Vertex & V2, const Vertex & V3 )
{
  if (!_vertices.empty())
    _vertices.clear();
  if (!_indices.empty())
    _indices.clear();

  _nooV = _nooI = 3;

  _vertices.push_back(V1);
  _vertices.push_back(V2);
  _vertices.push_back(V3);

  _indices.push_back(0);
  _indices.push_back(1);
  _indices.push_back(2);

  return *this;
} /* End of 'geom::Geom::createTriangle' function */

/* Create triangle geom object by three vertices positions */
geom::Geom & geom::Geom::createTriangle( const math::Vec3f & Pos1, const math::Vec3f & Pos2, const math::Vec3f & Pos3 )
{
  Vertex v1 = Vertex(Pos1);
  Vertex v2 = Vertex(Pos2);
  Vertex v3 = Vertex(Pos3);

  createTriangle(v1, v2, v3);

  autoNormal();

  return *this;
} /* End of 'geom::Geom::createTriangle' function */

geom::Geom & geom::Geom::createPlane( const Vertex & V1, const Vertex & V2, const Vertex & V3, const Vertex & V4 )
{
  if (!_vertices.empty())
    _vertices.clear();
  if (!_indices.empty())
    _indices.clear();

  _nooV = 4;
  _nooI = 6;

  /* Triangle vertices */
  _vertices.push_back(V1);
  _vertices.push_back(V2);
  _vertices.push_back(V3);
  _vertices.push_back(V4);

  /* Triangle indices */
  _indices.push_back(0);
  _indices.push_back(1);
  _indices.push_back(2);
  _indices.push_back(0);
  _indices.push_back(2);
  _indices.push_back(3);

  return *this;
} /* End of 'geom::Geom::createPlane' function */

geom::Geom & geom::Geom::createPlane( const math::Vec3f & Pos1, const math::Vec3f & Pos2, const math::Vec3f & Pos3, const math::Vec3f & Pos4 )
{
  Vertex v1 = Vertex(Pos1);
  Vertex v2 = Vertex(Pos2);
  Vertex v3 = Vertex(Pos3);
  Vertex v4 = Vertex(Pos4);

  createPlane(v1, v2, v3, v4);

  autoNormal();

  return *this;
} /* End of 'geom::Geom::createPlane' function */

/* Create plane geom object by two vertices */
geom::Geom & geom::Geom::createPlane( const Vertex & LeftBottom, const Vertex & RightTop )
{
  Vertex v2(LeftBottom);
  Vertex v4(RightTop);
  math::Vec3f diag = RightTop._pos - LeftBottom._pos;

  math::Vec3f tmp = diag & LeftBottom._norm;
  v2._pos = LeftBottom._pos + tmp + (diag - tmp) * 0.5;
  v2._tex = {1, 1};

  tmp = LeftBottom._norm & diag;
  v4._pos = LeftBottom._pos + tmp + (diag - tmp) * 0.5;
  v4._tex = {0, 0};

  return createPlane(LeftBottom, v2, RightTop, v4);
} /* End of 'geom::Geom::createPlane' function */

/* Create plane geom object by two vertices positions and normal */
geom::Geom & geom::Geom::createPlane( const math::Vec3f & LeftBottom, const math::Vec3f & RightTop, const math::Vec3f & Norm )
{
  Vertex v1(LeftBottom, Norm, {1, 0});
  Vertex v3(RightTop, Norm, {0, 1});

  return createPlane(v1, v3);
} /* End of 'geom::Geom::createPlane' function */

/* Create plane geom object by pos, two directions and width, height */
geom::Geom & geom::Geom::createPlane( const math::Vec3f & Pos, math::Vec3f & Right, math::Vec3f & Up, float W, float H )
{
  Right.normalize();
  Up.normalize();

  math::Vec3f norm = Right & Up;
  norm.normalize();

  return createPlane(Pos, Right, Up, norm, W, H);
} /* End of 'geom::Geom::createPlane' function */

  /* Create plane geom object by pos, two normilized directions and width, height */
geom::Geom & geom::Geom::createPlane( const math::Vec3f & Pos, const math::Vec3f & RightNorm, const math::Vec3f & UpNorm, const math::Vec3f & Norm, float W, float H )
{
  Vertex lb(Pos, Norm, {1, 0});
  Vertex rb(Pos + RightNorm * W, Norm, {1, 1});
  Vertex rt(rb._pos + UpNorm * H, Norm, {0, 1});
  Vertex lt(Pos + UpNorm * H, Norm, {0, 0});

  return createPlane(lb, rb, rt, lt);
} /* End of 'geom::Geom::createPlane' function */

/* Create sphere geom object */
geom::Geom & geom::Geom::createSphere( const math::Vec3f & Center, float R, int W, int H )
{
  /* Evaluate data count */
  _nooV = (H + 1) * (W + 1);
  _nooI = W * H * 6;

  _indices = std::vector<unsigned long>(_nooI);
  _vertices = std::vector<Vertex>(_nooV);

  /* Settint vertices texture coordinates */
  for (int i = 0; i <= H; i++)
    for (int j = 0; j <= W ; j++)
      _vertices[i * (W + 1) + j]._tex = {1.0f * j / W, 1.0f * i / H};

  /* Setting indices */
  for (int i = 0; i < H; i++)
    for (int j = 0; j < W; j++)
    {
      /*  |\
       *  |_\
       */
      _indices[i * W * 6 + 6 * j] = i * (W + 1) + j;
      _indices[i * W * 6 + 6 * j + 1] = (i + 1) * (W + 1) + j;
      _indices[i * W * 6 + 6 * j + 2] = (i + 1) * (W + 1) + j + 1;

      /*  __
       *  \ |
       *   \|
       */
      _indices[i * W * 6 + 6 * j + 3] = (i + 1) * (W + 1) + j + 1;
      _indices[i * W * 6 + 6 * j + 4] = i * (W + 1) + j + 1;
      _indices[i * W * 6 + 6 * j + 5] = i * (W + 1) + j;
    }

  /* Getting vetrices positions */
  for (int i = 0; i <= H; i++)
  {
    float theta = interpolate(0, (float)PI, (float)i / H);

    for (int j = 0; j <= W; j++)
    {
      float phi = interpolate(0, 2 * (float)PI, (float)j / W);
      Vec3f v = {sin(theta) * sin(phi), cos(theta), sin(theta) * cos(phi)};

      _vertices[i * (W + 1) + j]._pos = v * R + Center;
      _vertices[i * (W + 1) + j]._norm = v.normalize();
    }
  }

  //autoNormal();

  return *this;
} /* End of 'geom::Geom::createSphere' function */

/* END OF 'geometry.cpp' FILE */
