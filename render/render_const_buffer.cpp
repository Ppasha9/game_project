/*
 * Game project
 *
 * FILE: render_const_buffer.cpp
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 01.04.2018
 * NOTE: render constant buffer handle implementation file
 */

#include <cassert>

#include "render.h"

using namespace render;
using namespace DirectX;

/* Create shader constant buffer function */
void Render::initConstBuffer( void )
{
  D3D11_BUFFER_DESC buffer_desc;
  HRESULT result;

  buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
  buffer_desc.ByteWidth = sizeof(ConstBuffer::Data);
  buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  buffer_desc.MiscFlags = 0;
  buffer_desc.StructureByteStride = 0;

  result = _device->CreateBuffer(&buffer_desc, NULL, &_constBuffer._buffer);

  assert(!FAILED(result));
} /* End of 'Render::initConstBuffer' function */

/* Update const buffer function */
void Render::updateConstBuffer( void )
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mapped_resource;
  ConstBuffer::Data *data_ptr;
  unsigned int buffer_number;

  // Lock constant buffer so to write in it
  result = _deviceContext->Map(_constBuffer._buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
  assert(!FAILED(result));

  // Get pointer to constant buffer data
  data_ptr = (ConstBuffer::Data *)mapped_resource.pData;

  // Copy data into constant buffer
  *data_ptr = _constBuffer._data;

  //XMStoreFloat4x4(&data_ptr->_world, XMMatrixTranspose(XMLoadFloat4x4(&data_ptr->_world)));
  //XMStoreFloat4x4(&data_ptr->_view, XMMatrixTranspose(XMLoadFloat4x4(&data_ptr->_view)));
  //XMStoreFloat4x4(&data_ptr->_proj, XMMatrixTranspose(XMLoadFloat4x4(&data_ptr->_proj)));
  data_ptr->_world = data_ptr->_world.getTranspose();
  data_ptr->_view = data_ptr->_view.getTranspose();
  data_ptr->_proj = data_ptr->_proj.getTranspose();

  // Unlock constant buffer
  _deviceContext->Unmap(_constBuffer._buffer, 0);

  // Set position of constant buffer in vertex shader
  buffer_number = 0;

  // Set updated constant buffer in shaders
  _deviceContext->VSSetConstantBuffers(buffer_number, 1, &_constBuffer._buffer);
  _deviceContext->PSSetConstantBuffers(buffer_number, 1, &_constBuffer._buffer);
} /* End of 'Render::updateConstBuffer' function */

/* Release shader constant buffer function */
void Render::releaseConstBuffer( void )
{
  releaseRes<ID3D11Buffer>(_constBuffer._buffer);
} /* End of 'Render::releaseConstBuffer' function */

/* END OF 'render_const_buffer.cpp' FILE */
