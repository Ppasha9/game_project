/*
 * Game project
 *
 * FILE: render_geom.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: render geometry resource handle implementation file
 */

#include "render.h"

using namespace render;

/* Create geometry function */
// Geom * createGeom( const geometry &Geomery, const string &GeomName );
GeomPtr Render::createGeom( const string &GeomName )
{
  Vertex *v;
  unsigned long *i;
  D3D11_BUFFER_DESC v_buffer_desc, i_buffer_desc;
  D3D11_SUBRESOURCE_DATA v_data, i_data;
  HRESULT result;

  GeomPtr tmp;
  if ((tmp = getGeom(GeomName))._resource != nullptr)
    return tmp;

  Geom *G = new Geom(GeomName);

  // Set the number of vertices in the vertex array.
  G->_nooV = 3;

  // Set the number of indices in the index array.
  G->_nooI = 3;

  // Create the vertex array
  v = new Vertex[G->_nooV];
  if (!v)
  {
    delete G;
    return nullptr;
  }

  // Create the index array
  i = new unsigned long[G->_nooI];
  if (!i)
  {
    delete G;
    return nullptr;
  }

  v[0]._pos[0] = -1;
  v[0]._pos[1] = -1;
  v[0]._pos[2] = 0;

  v[1]._pos[0] = 1;
  v[1]._pos[1] = -1;
  v[1]._pos[2] = 0;

  v[2]._pos[0] = 0;
  v[2]._pos[1] = 1;
  v[2]._pos[2] = 0;

  v[0]._norm[0] = 0;
  v[0]._norm[1] = 0;
  v[0]._norm[2] = 1;

  v[1]._norm[0] = 0;
  v[1]._norm[1] = 0;
  v[1]._norm[2] = 1;

  v[2]._norm[0] = 0;
  v[2]._norm[1] = 0;
  v[2]._norm[2] = 1;

  v[0]._tex[0] = 0;
  v[0]._tex[1] = 30;

  v[1]._tex[0] = 15;
  v[1]._tex[1] = 0;

  v[2]._tex[0] = 30;
  v[2]._tex[1] = 30;

  i[0] = 0;
  i[1] = 1;
  i[2] = 2;

  // Set up vertex buffer description
  v_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  v_buffer_desc.ByteWidth = sizeof(Vertex) * G->_nooV;
  v_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  v_buffer_desc.CPUAccessFlags = 0;
  v_buffer_desc.MiscFlags = 0;
  v_buffer_desc.StructureByteStride = 0;

  // Give subresource structure pointer to vertex data
  v_data.pSysMem = v;
  v_data.SysMemPitch = 0;
  v_data.SysMemSlicePitch = 0;

  // Create vertex buffer
  result = _device->CreateBuffer(&v_buffer_desc, &v_data, &G->_vertexBuffer);
  if (FAILED(result))
  {
    delete G;
    return nullptr;
  }

  // Set up index buffer description
  i_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  i_buffer_desc.ByteWidth = sizeof(unsigned long) * G->_nooI;
  i_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
  i_buffer_desc.CPUAccessFlags = 0;
  i_buffer_desc.MiscFlags = 0;
  i_buffer_desc.StructureByteStride = 0;

  // Give subresource structure pointer to index data
  i_data.pSysMem = i;
  i_data.SysMemPitch = 0;
  i_data.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = _device->CreateBuffer(&i_buffer_desc, &i_data, &G->_indexBuffer);
  if (FAILED(result))
  {
    delete G;
    return nullptr;
  }

  delete[] i;
  delete[] v;

  _geometries.add(GeomName, G);

  return G;
} /* End of 'Render::createGeom' function */

/* Get geometry interface function */
GeomPtr Render::getGeom( const string &GeomName ) const
{
  return _geometries.get(GeomName);
} /* End of 'Render::getGeom' function */

/* Draw geometry function */
void Render::drawGeom( Geom *Geom )
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof(Vertex);
  offset = 0;
  _deviceContext->IASetVertexBuffers(0, 1, &Geom->_vertexBuffer, &stride, &offset);
  _deviceContext->IASetIndexBuffer(Geom->_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
  _deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  _deviceContext->DrawIndexed(Geom->_nooI, 0, 0);
} /* End of 'Render::drawGeom' function */

/* Realease geometry function */
void Render::releaseGeom( Render *Rnd, Geom *G )
{
  Rnd->releaseRes<ID3D11Buffer>(G->_vertexBuffer);
  Rnd->releaseRes<ID3D11Buffer>(G->_indexBuffer);
} /* End of 'Render::releaseGeom' function */

/* Realease geometry function */
void Render::releaseGeom( GeomPtr &G )
{
  _geometries.release(G);
} /* End of 'Render::releaseGeom' function */

/* END OF 'render_geom.cpp' FILE */
