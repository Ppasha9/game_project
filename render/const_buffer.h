/*
 * Game project
 *
 * FILE: const_buffer.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 25.03.2018
 * NOTE: constant buffer handle file
 */

#pragma once

#include <d3d11.h>
#include <directxmath.h>

/* Render handle namespace */
namespace render
{
  /* Shader constant buffer class */
  class ConstBuffer
  {
    friend class Render;

  private:
    struct Data
    {
      DirectX::XMMATRIX _world;
      DirectX::XMMATRIX _view;
      DirectX::XMMATRIX _proj;
    };

    ID3D11Buffer *_buffer;
    Data _data;

  public:
    /* Create constant bufer function */
    ConstBuffer( void ) {}
  }; /* End of 'ConstBuffer' class */
}; /* end of 'render' namespace */

/* END OF 'const_buffer.h' FILE */
