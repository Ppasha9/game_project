/*
* Game project
*
* FILE: geometry.cpp
* AUTHORS:
*   Kozlov Ilya
* LAST UPDATE: 17.04.2018
* NOTE: geometry handle implementation
*/

#include <fstream>
#include <cstdio>

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
    /* Obtain normal for facet */
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

/* Check if vertex is already added in array */
bool geom::Geom::find( Vertex & v )
{
  for (auto vert : _vertices)
    if (vert == v)
      return true;

  return false;
} /* End of 'geom::Geom::find' function */

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

/* Create box geom object */
geom::Geom & geom::Geom::createBox( const math::Vec3f & Center, float L )
{
  _nooV = _nooI = 36;

  _vertices = std::vector<Vertex>(_nooV);

  if (!_indices.empty())
    _indices.clear();

  /* Top */
  _vertices[0]._pos =  {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  _vertices[1]._pos =  {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  _vertices[2]._pos =  {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  _vertices[3]._pos =  _vertices[0]._pos;
  _vertices[4]._pos =  _vertices[2]._pos;
  _vertices[5]._pos =  {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  /* Right */
  _vertices[6]._pos =  {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};
  _vertices[7]._pos =  {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[8]._pos =  {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  _vertices[9]._pos =  _vertices[6]._pos;
  _vertices[10]._pos = _vertices[8]._pos;
  _vertices[11]._pos = {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  /* Front */
  _vertices[12]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};
  _vertices[13]._pos = {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};
  _vertices[14]._pos = {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  _vertices[15]._pos = _vertices[12]._pos;
  _vertices[16]._pos = _vertices[14]._pos;
  _vertices[17]._pos = {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  /* Left */
  _vertices[18]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[19]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};
  _vertices[20]._pos = {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] + 0.5f};
  _vertices[21]._pos = _vertices[18]._pos;
  _vertices[22]._pos = _vertices[20]._pos;
  _vertices[23]._pos = {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  /* Back */
  _vertices[24]._pos = {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[25]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[26]._pos = {Center._coords[0] - 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  _vertices[27]._pos = _vertices[24]._pos;
  _vertices[28]._pos = _vertices[26]._pos;
  _vertices[29]._pos = {Center._coords[0] + 0.5f, Center._coords[1] + 0.5f, Center._coords[2] - 0.5f};
  /* Bottom */
  _vertices[30]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[31]._pos = {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] - 0.5f};
  _vertices[32]._pos = {Center._coords[0] + 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};
  _vertices[33]._pos = _vertices[30]._pos;
  _vertices[34]._pos = _vertices[32]._pos;
  _vertices[35]._pos = {Center._coords[0] - 0.5f, Center._coords[1] - 0.5f, Center._coords[2] + 0.5f};

  /* Norms */
  _vertices[0]._norm =  _vertices[1]._norm =  _vertices[2]._norm =  _vertices[3]._norm =  _vertices[4]._norm =  _vertices[5]._norm =  { 0,  1,  0}; // top
  _vertices[6]._norm =  _vertices[7]._norm =  _vertices[8]._norm =  _vertices[9]._norm =  _vertices[10]._norm = _vertices[11]._norm = { 1,  0,  0}; // right
  _vertices[12]._norm = _vertices[13]._norm = _vertices[14]._norm = _vertices[15]._norm = _vertices[16]._norm = _vertices[17]._norm = { 0,  0,  1}; // front
  _vertices[18]._norm = _vertices[19]._norm = _vertices[20]._norm = _vertices[21]._norm = _vertices[22]._norm = _vertices[23]._norm = {-1,  0,  0}; // left
  _vertices[24]._norm = _vertices[25]._norm = _vertices[26]._norm = _vertices[27]._norm = _vertices[28]._norm = _vertices[29]._norm = { 0,  0, -1}; // back
  _vertices[30]._norm = _vertices[31]._norm = _vertices[32]._norm = _vertices[33]._norm = _vertices[34]._norm = _vertices[35]._norm = { 0, -1,  0}; // bottom

   /* Top */
  _vertices[0]._tex =  {0.25f, 1 / 3.0f};
  _vertices[1]._tex =  {0.50f, 1 / 3.0f};
  _vertices[2]._tex =  {0.50f, 0 / 3.0f};
  _vertices[3]._tex =  _vertices[0]._tex;
  _vertices[4]._tex =  _vertices[2]._tex;
  _vertices[5]._tex =  {0.25f, 0 / 3.0f};
  /* Right */
  _vertices[6]._tex =  {0.50f, 2 / 3.0f};
  _vertices[7]._tex =  {0.75f, 2 / 3.0f};
  _vertices[8]._tex =  {0.50f, 1 / 3.0f};
  _vertices[9]._tex =  _vertices[6]._tex;
  _vertices[10]._tex = _vertices[8]._tex;
  _vertices[11]._tex = {0.50f, 1 / 3.0f};
  /* Front */
  _vertices[12]._tex = {0.25f, 2 / 3.0f};
  _vertices[13]._tex = {0.50f, 2 / 3.0f};
  _vertices[14]._tex = {0.50f, 1 / 3.0f};
  _vertices[15]._tex = _vertices[12]._tex;
  _vertices[16]._tex = _vertices[14]._tex;
  _vertices[17]._tex = {0.25f, 1 / 3.0f};
  /* Left */
  _vertices[18]._tex = {0.00f, 2 / 3.0f};
  _vertices[19]._tex = {0.25f, 2 / 3.0f};
  _vertices[20]._tex = {0.25f, 1 / 3.0f};
  _vertices[21]._tex = _vertices[18]._tex;
  _vertices[22]._tex = _vertices[20]._tex;
  _vertices[23]._tex = {0.25f, 1 / 3.0f};
  /* Back */
  _vertices[24]._tex = {0.75f, 2 / 3.0f};
  _vertices[25]._tex = {1.00f, 2 / 3.0f};
  _vertices[26]._tex = {1.00f, 1 / 3.0f};
  _vertices[27]._tex = _vertices[24]._tex;
  _vertices[28]._tex = _vertices[26]._tex;
  _vertices[29]._tex = {0.75f, 1 / 3.0f};
  /* Bottom */
  _vertices[30]._tex = {0.25f, 3 / 3.0f};
  _vertices[31]._tex = {0.50f, 3 / 3.0f};
  _vertices[32]._tex = {0.50f, 2 / 3.0f};
  _vertices[33]._tex = _vertices[30]._tex;
  _vertices[34]._tex = _vertices[32]._tex;
  _vertices[35]._tex = {0.25f, 2 / 3.0f};

  for (int i = 0; i < 36; i++)
  {
    _vertices[i]._pos *= L;
    _indices.push_back(i);
  }

  return *this;
} /* End of 'geom::Geom::createBox' function */

/* Load geom object from .obj file */
geom::Geom & geom::Geom::loadObj( const string & name )
{
  std::ifstream in;
  string tmp;
  float x, y, z;

  std::vector<math::Vec3f> poss;
  std::vector<math::Vec3f> norms;
  std::vector<math::Vec2f> texs;

  string full_name = "bin\\objects\\";
  full_name.append(name);
  full_name.append(".obj");

  in.open(full_name);

  while (!in.eof())
  {
    string type;


    in >> type;

    if (type == "v")        // vertice coordinates
    {
      in >> x >> y >> z;
      poss.push_back(math::Vec3f({x, y, z}));
    }
    else if (type == "vn")  // vertice normal
    {
      in >> x >> y >> z;
      norms.push_back(math::Vec3f({x, y, z}));
    }
    else if (type == "vt")  // texture coordinates
    {
      in >> x >> y;
      texs.push_back(math::Vec2f({x, 1 - y}));
    }
    else if (type == "f")   // facets
    {
      int p_ind, t_ind, n_ind;

      for (int i = 0; i < 3; i++)
      {
        string str;
        in >> str;

        sscanf(str.c_str(), "%i/%i/%i", &p_ind, &t_ind, &n_ind);

        _vertices.push_back(Vertex(poss[p_ind - 1], norms[n_ind - 1], texs[t_ind - 1]));
      }
    }
    else
      std::getline(in, tmp);

  }

  in.close();

  for (unsigned long i = 0; i < _vertices.size(); i++)
    _indices.push_back(i);

  _nooI = _nooV = _vertices.size();

  return *this;
} /* End of 'geom::Geom::loadObj' function */

/* END OF 'geometry.cpp' FILE */
