/*
 * Game project
 *
 * FILE: geom.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 24.03.2018
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
  /* 3D-space vertex structure */
  struct Vertex
  {
    float _pos[3];
    float _norm[3];
    float _tex[2];
  }; /* End of 'Vertex' function */

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
