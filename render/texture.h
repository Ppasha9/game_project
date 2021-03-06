/*
 * Game project
 *
 * FILE: texture.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 09.04.2018
 * NOTE: texture resource handle file
 */

#pragma once

#include <d3d11.h>

#include "..\def.h"
#include "resource.h"
#include "res_ptr.h"

/* Render handle namespace */
namespace render
{
  /* Texture resource class */
  class Texture : public Resource
  {
    friend class Render;
    friend class ResPtr<Texture>;

  private:
    ID3D11Texture2D *_texBuffer;
    ID3D11ShaderResourceView *_texView;

    /* Create texture function */
    Texture( const string &TexName ) : Resource(TexName) {}
  }; /* End of 'Texture' class */

  using TexturePtr = ResPtr<Texture>;
}; /* end of 'render' namespace */

/* END OF 'texture.h' FILE */
