/*
 * Game project
 *
 * FILE: const_buffer.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 01.04.2018
 * NOTE: constant buffer handle file
 */

#pragma once

#include <d3d11.h>
#include <directxmath.h>

#include "material.h"

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
      math::Matr4f _world;
      math::Matr4f _view;
      math::Matr4f _proj;

      math::Vec4f _cameraPos;
      math::Vec4f _cameraDir;

      Material::Coeffs _mtlCoeffs;

      math::Vec4f _lightPos;
      math::Vec4f _lightColor;
    };

    ID3D11Buffer *_buffer;
    Data _data;

  public:
    /* Create constant bufer function */
    ConstBuffer( void ) {}
  }; /* End of 'ConstBuffer' class */
}; /* end of 'render' namespace */

/* END OF 'const_buffer.h' FILE */
