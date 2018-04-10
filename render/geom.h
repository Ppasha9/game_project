/*
 * Game project
 *
 * FILE: geom.h
 * AUTHORS:
 *   Vasilyev Peter,
 *   Kozlov Ilya
 * LAST UPDATE: 11.04.2018
 * NOTE: geometry directx resource handle file
 */

#pragma once

#include <d3d11.h>

#include "..\def.h"
#include "resource.h"
#include "res_ptr.h"

/* Render handle namespace */
namespace render
{
  /* Geometry resource class */
  class Geom : public Resource
  {
    friend class Render;
    friend class ResPtr<Geom>;

  private:
    unsigned long _nooV;
    unsigned long _nooI;

    ID3D11Buffer *_vertexBuffer;
    ID3D11Buffer *_indexBuffer;

    /* Create geometry function */
    Geom( const string &GeomName ) : Resource(GeomName)
    {
    } /* End of 'Geom' function */
  }; /* End of 'Geom' class */

  using GeomPtr = ResPtr<Geom>;
} /* end of 'render' namespace */

/* END OF 'geom.h' FILE */
