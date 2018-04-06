/*
 * Game project
 *
 * FILE: shader.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 06.04.2018
 * NOTE: shader resource handle file
 */

#pragma once

#include <d3d11.h>

#include "..\def.h"
#include "resource.h"
#include "res_ptr.h"

/* Render handle namespace */
namespace render
{
  /* Shader resource class */
  class Shader : public Resource
  {
    friend class Render;
    friend class ResPtr<Shader>;

  private:
    ID3D11VertexShader *_vertexShader;
    ID3D11PixelShader  *_pixelShader;
    ID3D11InputLayout  *_inputLayout;

    /* Create shader function */
    Shader( const string &ShName ) : Resource(ShName) {}
  }; /* End of 'Shader' class */

  using ShaderPtr = ResPtr<Shader>;
}; /* end of 'render' namespace */

/* END OF 'shader.h' FILE */
