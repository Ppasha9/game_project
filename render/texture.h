/*
 * Game project
 *
 * FILE: texture.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: texture resource handle file
 */

#pragma once

#include "..\def.h"
#include "resource.h"
#include "res_ptr.h"

/* Render handle namespace */
namespace render
{
  /* Texture resource class */
  class Texture : Resource
  {
    friend class Render;
    friend class ResPtr<Texture>;

  private:
    // params

    /* Create texture function */
    Texture( const string &TexName ) : Resource(TexName) {}
  }; /* End of 'Texture' class */

  using TexturePtr = ResPtr<Texture>;
}; /* end of 'render' namespace */

/* END OF 'texture.h' FILE */
