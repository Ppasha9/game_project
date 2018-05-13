/*
 * Game project
 *
 * FILE: prim.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 12.05.2018
 * NOTE: geometry primitive handle file
 */

#pragma once

#include <directxmath.h>

#include "material.h"
#include "shader.h"
#include "geom.h"

/* Render handle namespace */
namespace render
{
  /* Geometry primitive resource class */
  class Prim : public Resource
  {
    friend class Render;
    friend class ResPtr<Prim>;

  public:
    enum struct FillMode
    {
      SOLID,
      WIREFRAME
    };

    enum struct ProjMode
    {
      FRUSTUM,
      SCREENSPACE_PIXEL,
      SCREENSPACE_UNORM
    };

  private:
    GeomPtr _geometry;
    ShaderPtr _shader;
    MaterialPtr _material;

    FillMode _fillMode;
    ProjMode _projMode;

    /* Create primitive function */
    Prim( const string &PrimName ) : Resource(PrimName) {}
  }; /* End of 'Prim' class */

  using PrimPtr = ResPtr<Prim>;
}; /* end of 'render' namespace */

/* END OF 'prim.h' FILE */
