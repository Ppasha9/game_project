/*
 * Game project
 *
 * FILE: geom.h
 * AUTHORS:
 *   Vasilyev Peter
 * LAST UPDATE: 07.03.2018
 * NOTE: geometry directx resource handle file
 */

#pragma once

#include "..\def.h"

/* Render handle namespace */
namespace render
{
  /* Geometry resource class */
  class Geom
  {
    friend class Render;

  private:
    string _name;
    // params

    /* Create geometry function */
    Geom( /* params */ );
  }; /* End of 'Geom' class */
} /* end of 'render' namespace */

/* END OF 'geom.h' FILE */
