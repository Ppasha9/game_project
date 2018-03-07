/*
 * Game project
 *
 * FILE: prim.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: geometry primitive handle file
 */

#pragma once

#include "material.h"
#include "shader.h"
#include "geom.h"

/* Render handle namespace */
namespace render
{
  /* Geometry primitive resource class */
  class Prim
  {
    friend class Render;

  private:
    string _name;

    Geom *_geometry;
    Shader *_shader;
    Material *_material;
    // World matrix

    /* Create primitive function */
    Prim( /* params */ );
  }; /* End of 'Prim' class */
}; /* end of 'render' namespace */

/* END OF 'prim.h' FILE */
