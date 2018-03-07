/*
  Game project

  FILE: prim.h
  AUTHOR:
    Vasilyev Peter
  LAST UPDATE: 07.03.2018
  NOTE: geometry primitive handle file
 */

#pragma once

#include "material.h"
#include "geom.h"

/* Render handle namespace */
namespace render
{
  /* Geometry primitive resource class */
  class prim
  {
    friend class render;

  private:
    string Name;

    geom *Geometry;
    shader *Shader;
    material *Material;
    // World matrix

    /* Create primitive function */
    prim( /* params */ );
  }; /* End of 'prim' class */
}; /* end of 'render' namespace */

/* END OF 'prim.h' FILE */
