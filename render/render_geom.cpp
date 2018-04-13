/*
 * Game project
 *
 * FILE: render_geom.cpp
 * AUTHORS:
 *   Vasilyev Peter,
 *   Kozlov Ilya
 * LAST UPDATE: 13.04.2018
 * NOTE: render geometry resource handle implementation file
 */

#include "render.h"
#include "..\geometry\geometry.h"

using namespace render;

/* Create geometry render resource from file function */
GeomPtr Render::createGeom( const string &GeomName )
{
  // return createGeom(GeomName, geom::Geom().LoadObj(string("bin\\models\\").append(GeomName)));

  return nullptr;
} /* End of 'Render::createGeom' function */

/* Create geometry function */
GeomPtr Render::createGeom( const string &GeomName, const geom::Geom &Geometry )
{
  D3D11_BUFFER_DESC v_buffer_desc, i_buffer_desc;
  D3D11_SUBRESOURCE_DATA v_data, i_data;
  HRESULT result;

  GeomPtr tmp;
  if ((tmp = getGeom(GeomName))._resource != nullptr)
    return tmp;

  Geom *G = new Geom(GeomName);

  G->_nooV = Geometry.getNumOfV();
  G->_nooI = Geometry.getNumOfI();

  // Set up vertex buffer description
  v_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
  v_buffer_desc.ByteWidth = sizeof(geom::Vertex) * G->_nooV;
  v_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  v_buffer_desc.CPUAccessFlags = 0;
  v_buffer_desc.MiscFlags = 0;
  v_buffer_desc.StructureByteStride = 0;

  // Give subresource structure pointer to vertex data
  std::vector<geom::Vertex> vv = Geometry.getVertices();
  v_data.pSysMem = vv.data();
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
  std::vector<unsigned long> iv = Geometry.getIndices();
  i_data.pSysMem = iv.data();
  i_data.SysMemPitch = 0;
  i_data.SysMemSlicePitch = 0;

  // Create the index buffer.
  result = _device->CreateBuffer(&i_buffer_desc, &i_data, &G->_indexBuffer);
  if (FAILED(result))
  {
    delete G;
    return nullptr;
  }

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

  stride = sizeof(geom::Vertex);
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
